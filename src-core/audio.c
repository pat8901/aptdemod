/*
    AptDemod - Lightweight APT decoder for NOAA weather satellites
    Copyright (C) 2025 pat8901

    AptDemod is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    AptDemod is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

// https://www.sigidwiki.com/wiki/Automatic_Picture_Transmission_(APT)
// https://en.wikipedia.org/wiki/Automatic_picture_transmission

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sndfile.h>
#include <linux/limits.h>
#include <fftw3.h>
#include <stdlib.h>
#include "audio.h"
#include "demod.h"
#include "utils.h"
#include "image.h"

/* Down samples a 11025hz buffer to a 4160hz buffer. */
int down_sample(SNDFILE *sndfile, SF_INFO *sfinfo)
{
    /* Gathering 11025hz info and creating a buffer for it. */
    sf_count_t frames = sfinfo->frames;
    sf_count_t count = 0;
    sf_count_t high_frames_amount = 11025;
    /* sf_count_t low_frame_amount = 4160; */
    int sample_rate = sfinfo->samplerate;
    float seek_rate = (float)sample_rate / 4160.0;
    float *buffer_11025 = (float *)malloc(high_frames_amount * sizeof(float));

    /* Initialize output file for 4160hz down sampled audio. */
    SF_INFO sfinfo_4160;
    SNDFILE *sndfile_4160;
    sfinfo_4160.samplerate = 4160;
    sfinfo_4160.channels = 1;
    sfinfo_4160.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;

    /* Opening 4160hz output file. */
    const char *file_path_4160 = "./output/audio/test.wav";
    sndfile_4160 = sf_open(file_path_4160, SFM_WRITE, &sfinfo_4160);
    if (!sndfile_4160)
    {
        printf("Failed to open file: %s\n", sf_strerror(NULL));
        return -1;
    }

    /* Downsample audio buffers from 11025Hz to 4160Hz using Linear Interpolation. */
    while (count < frames)
    {
        /* Seek to the requested 11025hz frame and
           read a selected amount of samples into a frame buffer */
        printf("count: %ld\n", count);
        sf_seek(sndfile, count, SEEK_SET);
        sf_count_t frames_requested = sf_readf_float(sndfile, buffer_11025, high_frames_amount);

        /* Indicator that the last chunk is less than the required amount
           of frames to create 2 lines of an APT image.
           The program has neared the end of the audio file. */
        if (frames_requested != 11025)
        {
            printf("Remaining chuck-size: %ld\n", frames_requested);
        }

        /* Getting a dynamic length to create an 4160hz audio buffer.
           A dynamic buffer is needed to get every last drop of data
           from our wav audio file. */
        int downsample_length = (int)(frames_requested / seek_rate);
        printf("Downsample length: %d\n", downsample_length);
        float *buffer_4160 = (float *)malloc(downsample_length * sizeof(float));

        /*  Linear interpolate 11025hz to 4160hz and place result in 4160hz buffer. */
        for (int i = 0; i < downsample_length; i++)
        {
            /* x value */
            float input_index = (float)i * seek_rate;
            /* x_0, x_1 values */
            sf_count_t position_1 = (sf_count_t)input_index;
            sf_count_t position_2 = position_1 + 1;
            /* y_0, y_1 values */
            float sample_1 = buffer_11025[position_1];
            float sample_2 = buffer_11025[position_2];

            float result = linear_interpolate(position_1, position_2, input_index, sample_1, sample_2);
            buffer_4160[i] = result;
        }
        /* *** DEBUG *** */
        // print_buffer_4160(buffer_4160);

        /* Write 4160hz buffer to output file. */
        sf_count_t frames_written = sf_writef_float(sndfile_4160, buffer_4160, downsample_length);
        printf("Frames written %ld\n", frames_written);
        count = count + frames_requested;
        free(buffer_4160);
    }

    /* Displaying down-sampling stats. */
    printf("=================\n");
    printf("Finished!\n");
    printf("Count: %ld\n", count);

    /* Clean up. */
    free(buffer_11025);
    sf_close(sndfile_4160);

    return 0;
}

/* Create a 1 second audio file from a single frame buffer. */
int create_audio_single(double *buffer)
{
    sf_count_t buffer_length = 11025;

    /* Initialize 11025Hz output file. */
    SF_INFO sfinfo;
    SNDFILE *sndfile;
    sfinfo.samplerate = 11025;
    sfinfo.channels = 1;
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;

    /* Opening 11025hz output file. */
    const char *audio_output_path = "./output/audio/test_11025.wav";
    sndfile = sf_open(audio_output_path, SFM_WRITE, &sfinfo);
    if (!sndfile)
    {
        printf("Failed to open file: %s\n", sf_strerror(NULL));
        return -1;
    }

    /* Write 11025hz frame buffer to output file. */
    sf_count_t frames_written = sf_writef_double(sndfile, buffer, buffer_length);

    /* Displaying stats. */
    printf("=================\n");
    printf("Frames written %ld\n", frames_written);
    printf("Finished!\n");

    /* Clean up. */
    sf_close(sndfile);

    return 0;
}

/* Demodulate a full 11025hz audio file and generate a
   new audio file containing the demodulated audio. */
int create_audio()
{
    /* Initialize 11025 Hz audio input file. */
    SF_INFO sfinfo_input;
    SNDFILE *sndfile_input;
    sfinfo_input.format = 0;
    /* DEBUG: 0 = activates writing intermidiate debug file. */
    int print = 0;

    /* Opening 11025hz input audio file. */
    const char *file_path = "./documentation/test_audio/20210720111842.wav";
    sndfile_input = sf_open(file_path, SFM_READ, &sfinfo_input);
    if (!sndfile_input)
    {
        printf("Failed to open file: %s\n", sf_strerror(NULL));
        return -1;
    }
    sf_count_t frames = sfinfo_input.frames;
    sf_count_t count = 0;
    sf_count_t buffer_length = 11025;

    /* Initialize demodulated 11025Hz audio output file. */
    SF_INFO sfinfo_output;
    SNDFILE *sndfile_output;
    sfinfo_output.samplerate = 11025;
    sfinfo_output.channels = 1;
    sfinfo_output.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;

    /* Opening demodulated 11025hz audio output file. */
    const char *audio_output_path = "./output/audio/apt_demodulated.wav";
    sndfile_output = sf_open(audio_output_path, SFM_WRITE, &sfinfo_output);
    if (!sndfile_output)
    {
        printf("Failed to open file: %s\n", sf_strerror(NULL));
        return -1;
    }

    /* Demodulate 11025hz frame buffers and write result to audio output file. */
    while (count < frames)
    {
        /* Seek to the requested 11025hz frame and
           read a selected amount of samples into a frame buffer. */
        printf("count: %ld\n", count);
        sf_seek(sndfile_input, count, SEEK_SET);
        double *input_buffer = (double *)fftw_malloc(sizeof(double) * 11025);
        sf_count_t frames_requested = sf_readf_double(sndfile_input, input_buffer, 11025);
        printf("Frames read: %ld\n", frames_requested);

        /* If activated, generate a debug file holding
           frame buffer data before demodulation. */
        if (print == 0)
        {
            FILE *base = fopen("./documentation/output/plots/11025_base.txt", "w");
            fprintf(base, "Real\n");
            for (int i = 0; i < 11025; i++)
            {
                fprintf(base, "%f\n", input_buffer[i]);
            }
            fclose(base);
            print = 1;
        }

        /* Demodulate 11025hz input frame buffer. */
        double *intermediate_buffer = am_demodulate(input_buffer, 11025, ON);

        /* Writing demodulated frame buffer to audio output file. */
        sf_count_t frames_written = sf_writef_double(sndfile_output, intermediate_buffer, buffer_length);
        count += frames_requested;
        printf("Frames written %ld\n", frames_written);

        /* Clean up. */
        free(input_buffer);
        free(intermediate_buffer);
    }

    printf("=================\n");
    printf("Finished!\n");

    /* Clean up. */
    sf_close(sndfile_input);
    sf_close(sndfile_output);

    return 0;
}

/* Deprecated, used for testing. Gets a single 4160hz frame buffer. */
double *get_4160_sample()
{
    SF_INFO file_info;
    SNDFILE *file;

    file_info.format = 0;
    const char *path = "./documentation/output/test.wav";

    file = sf_open(path, SFM_READ, &file_info);

    // TODO: How big should I make each index?
    double *mem_ptr = (double *)fftw_malloc(sizeof(double) * 4160);

    sf_seek(file, 4160 * 10, SEEK_SET);
    sf_count_t frames_requested = sf_readf_double(file, mem_ptr, 4160);
    printf("Frames read: %ld\n", frames_requested);

    return mem_ptr;
}

/* Deprecated, used for testing. Gets a single 11025hz frame buffer. */
double *get_11025_sample()
{
    SF_INFO file_info;
    SNDFILE *file;

    file_info.format = 0;
    const char *path = "./documentation/test_audio/20210720111842.wav";
    file = sf_open(path, SFM_READ, &file_info);

    double *mem_ptr = (double *)fftw_malloc(sizeof(double) * 11025);

    sf_seek(file, 0, SEEK_SET);
    sf_count_t frames_requested = sf_readf_double(file, mem_ptr, 11025);
    printf("Frames read: %ld\n", frames_requested);

    return mem_ptr;
}
