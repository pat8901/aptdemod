// https://www.sigidwiki.com/wiki/Automatic_Picture_Transmission_(APT)
// https://en.wikipedia.org/wiki/Automatic_picture_transmission
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sndfile.h>
#include <linux/limits.h>
#include <fftw3.h>
#include "apt.h"
#include "algebra.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    SF_INFO sfinfo;
    SF_INFO *ptr = &sfinfo;
    SNDFILE *sndfile;

    get_path();
    sfinfo.format = 0;
    const char *file_path = "./documentation/test_audio/20210720111842.wav";

    // Opening audio file.
    sndfile = sf_open(file_path, SFM_READ, &sfinfo);
    if (!sndfile)
    {
        printf("Failed to open file: %s\n", sf_strerror(NULL));
        return -1;
    }

    // down_sample(sndfile, ptr);
    sf_close(sndfile);

    // double *mem_ptr = get_4160_sample();
    // double *mem_ptr_11025 = get_11025_sample();

    // fast_fourier_transform(mem_ptr, 4160);
    // fft_test();
    // fftw_test_11025(mem_ptr_11025, 11025);
    // memory_copy_practice();

    /* 8-13 Tests*/
    // after_filter_11025(mem_ptr_11025, 11025);
    // double *buffer = am_demod_11025(mem_ptr_11025, 11025);
    // create_audio_single(buffer);
    create_audio();

    // free(mem_ptr);
    // free(mem_ptr_11025);

    return 0;
}

int down_sample(SNDFILE *sndfile, SF_INFO *sfinfo)
{
    sf_count_t frames = sfinfo->frames;
    sf_count_t count = 0;
    sf_count_t high_frames_amount = 11025;
    sf_count_t low_frame_amount = 4160;
    int sample_rate = sfinfo->samplerate;
    float seek_rate = (float)sample_rate / 4160.0;
    float *buffer_11025 = (float *)malloc(high_frames_amount * sizeof(float));

    // init write output file for 4160Hz downsample
    SF_INFO sfinfo_4160;
    SNDFILE *sndfile_4160;
    sfinfo_4160.samplerate = 4160;
    sfinfo_4160.channels = 1;
    sfinfo_4160.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;

    const char *file_path_4160 = "./documentation/output/test.wav";
    sndfile_4160 = sf_open(file_path_4160, SFM_WRITE, &sfinfo_4160);
    if (!sndfile_4160)
    {
        printf("Failed to open file: %s\n", sf_strerror(NULL));
        return -1;
    }

    // Downsample from 11025Hz to 4160Hz using Linear Interpolation.
    while (count < frames)
    {

        printf("count: %d\n", count);
        sf_count_t start_frame = sf_seek(sndfile, count, SEEK_SET);
        sf_count_t frames_requested = sf_readf_float(sndfile, buffer_11025, high_frames_amount);

        // indicator that the last chunk is less than required amount of frames to create 2 lines of an APT image.
        // The program has either neared the end of the audio file or the file did not have enough information to begin with.
        if (frames_requested != 11025)
        {
            printf("Remaining chuck-size: %d\n", frames_requested);
        }

        // This is the dynamic length of our down-sampled audio buffer. This is necessary in order to get every last drop of data
        // from our wav audio file.
        int downsample_length = (int)(frames_requested / seek_rate);
        printf("Downsample length: %d\n", downsample_length);
        // Look into the mechanics in how this works more with indexing and overflowing
        float *buffer_4160 = (float *)malloc(downsample_length * sizeof(float));

        for (int i = 0; i < downsample_length; i++)
        {
            // x value
            float input_index = (float)i * seek_rate;
            //  x_0, x_1 values
            sf_count_t position_1 = (sf_count_t)input_index;
            sf_count_t position_2 = position_1 + 1;

            //  y_0, y_1 values
            float sample_1 = buffer_11025[position_1];
            float sample_2 = buffer_11025[position_2];
            // not used - will get rid of later.
            float mu = input_index - position_1;

            float result = linear_interpolate(position_1, position_2, input_index, sample_1, sample_2);
            // printf("================================\n");
            // printf("4160Hz index: %d\n", i);
            // printf("11025Hz indexes: %d(%f), %d(%f)\n", position_1, sample_1, position_2, sample_2);
            // printf("interpolated value: %f\n", result);
            buffer_4160[i] = result;
        }
        // print_buffer_4160(buffer_4160);

        sf_count_t frames_written = sf_writef_float(sndfile_4160, buffer_4160, downsample_length);
        printf("Frames written %d\n", frames_written);
        count = count + frames_requested;
        free(buffer_4160);
    }
    printf("=================\n");
    printf("Finished!\n");
    printf("Count: %d\n", count);
    free(buffer_11025);
    sf_close(sndfile_4160);
    return 0;
}

/*  Takes a pointer to memory buffer and creates an audio file.
    This is a test function and designed to create only one second of data.
*/
void create_audio_single(double *buffer)
{
    sf_count_t buffer_length = 11025;

    // init write output file for 11025Hz downsample
    SF_INFO sfinfo;
    SNDFILE *sndfile;
    sfinfo.samplerate = 11025;
    sfinfo.channels = 1;
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;

    // Set up output path for audio file
    const char *audio_output_path = "./documentation/output/test_11025.wav";
    sndfile = sf_open(audio_output_path, SFM_WRITE, &sfinfo);
    if (!sndfile)
    {
        printf("Failed to open file: %s\n", sf_strerror(NULL));
        return -1;
    }

    // Write buffer out to file.
    sf_count_t frames_written = sf_writef_double(sndfile, buffer, buffer_length);
    printf("Frames written %d\n", frames_written);

    printf("=================\n");
    printf("Finished!\n");
    sf_close(sndfile);
    return 0;
}

/* Demodulates full 11025 Hz audio file*/
void create_audio()
{
    // init for 11025 Hz audio input file
    SF_INFO sfinfo_input;
    SNDFILE *sndfile_input;
    sfinfo_input.format = 0;

    const char *file_path = "./documentation/test_audio/20210720111842.wav";
    // Opening input audio file.
    sndfile_input = sf_open(file_path, SFM_READ, &sfinfo_input);
    if (!sndfile_input)
    {
        printf("Failed to open file: %s\n", sf_strerror(NULL));
        return -1;
    }
    sf_count_t frames = sfinfo_input.frames;
    sf_count_t count = 0;
    sf_count_t buffer_length = 11025;

    // init write output file for 11025Hz downsample
    SF_INFO sfinfo_output;
    SNDFILE *sndfile_output;
    sfinfo_output.samplerate = 11025;
    sfinfo_output.channels = 1;
    sfinfo_output.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;

    // Set up output path for audio file
    const char *audio_output_path = "./documentation/output/test_full_11025.wav";
    sndfile_output = sf_open(audio_output_path, SFM_WRITE, &sfinfo_output);
    if (!sndfile_output)
    {
        printf("Failed to open file: %s\n", sf_strerror(NULL));
        return -1;
    }

    // Initializing audio reading

    while (count < frames)
    {
        printf("count: %d\n", count);
        sf_count_t start_frame = sf_seek(sndfile_input, count, SEEK_SET);
        double *input_buffer = (double *)fftw_malloc(sizeof(double) * 11025);
        sf_count_t frames_requested = sf_readf_double(sndfile_input, input_buffer, 11025);
        printf("Frames read: %ld\n", frames_requested);

        // Getting the demodulated buffer
        double *intermediate_buffer = am_demod_11025(input_buffer, 11025);

        // Writing demodulated buffer to audio output file.
        sf_count_t frames_written = sf_writef_double(sndfile_output, intermediate_buffer, buffer_length);
        printf("Frames written %d\n", frames_written);

        count += frames_requested;
        free(input_buffer);
        free(intermediate_buffer);
    }

    printf("=================\n");
    printf("Finished!\n");
    sf_close(sndfile_input);
    sf_close(sndfile_output);
    return 0;
}

double *get_4160_sample()
{
    SF_INFO file_info;
    SNDFILE *file;

    file_info.format = 0;
    const char *path = "./documentation/output/test.wav";

    file = sf_open(path, SFM_READ, &file_info);

    // TODO: How big should I make each index?
    double *mem_ptr = (double *)fftw_malloc(sizeof(double) * 4160);

    sf_count_t start_frame = sf_seek(file, 4160 * 10, SEEK_SET);
    sf_count_t frames_requested = sf_readf_double(file, mem_ptr, 4160);
    printf("Frames read: %ld\n", frames_requested);

    return mem_ptr;
}

double *get_11025_sample()
{
    SF_INFO file_info;
    SNDFILE *file;

    file_info.format = 0;
    const char *path = "./documentation/test_audio/20210720111842.wav";
    file = sf_open(path, SFM_READ, &file_info);

    double *mem_ptr = (double *)fftw_malloc(sizeof(double) * 11025);

    sf_count_t start_frame = sf_seek(file, 0, SEEK_SET);
    sf_count_t frames_requested = sf_readf_double(file, mem_ptr, 11025);
    printf("Frames read: %ld\n", frames_requested);

    return mem_ptr;
}

void print_buffer_4160(float *buffer)
{
    for (int i = 0; i < 4160; i++)
    {
        printf("index %d: %f\n", i, buffer[i]);
    }
}
