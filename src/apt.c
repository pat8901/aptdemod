// https://www.sigidwiki.com/wiki/Automatic_Picture_Transmission_(APT)
// https://en.wikipedia.org/wiki/Automatic_picture_transmission
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sndfile.h>
#include <linux/limits.h>
#include "apt.h"

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

    printf("Frame amount: %ld\n", ptr->frames);
    printf("Sample rate: %d Hz\n", ptr->samplerate);
    printf("Format: %d\n", ptr->format);
    printf("Channels: %d\n\n", sfinfo.channels);

    seek(sndfile, ptr);

    return 0;
}

// https://www.cuemath.com/linear-interpolation-formula/
float linear_interpolate(sf_count_t x_0, sf_count_t x_1, float x, float y_0, float y_1, float mu)
{
    // two different ways to achieve the same result.
    float y = y_0 + ((x - x_0) * ((y_1 - y_0) / (x_1 - x_0)));
    float result = (y_0 * (1 - mu)) + (y_1 * mu);
    // printf("y = %f | result = %f\n", y, result);
    return y;
}

// TODO:
// correct the name of this function and determine a way to dynamically size buffer to insert appropriate number of samples
// to match the size of a byte. Ideally I would want a sample rate of 4160. This would give me a byte per sample.
int seek(SNDFILE *sndfile, SF_INFO *sfinfo)
{
    sf_count_t frames = sfinfo->frames;
    sf_count_t count = 0;
    sf_count_t high_frames_amount = 11025;
    sf_count_t low_frame_amount = 4160;
    int sample_rate = sfinfo->samplerate;
    float seek_rate = (float)sample_rate / 4160.0;
    float *buffer_11025 = (float *)malloc(high_frames_amount * sizeof(float));
    // float *buffer_4160 = (float *)malloc(low_frame_amount * sizeof(float));

    // init write output file for 4160Hz downsample
    SF_INFO sfinfo_4160;
    SNDFILE *sndfile_4160;
    sfinfo_4160.samplerate = 4160;
    sfinfo_4160.channels = 1;
    sfinfo_4160.format = 65538;
    const char *file_path_4160 = "./documentation/output/test.wav";
    sndfile_4160 = sf_open(file_path_4160, SFM_WRITE, &sfinfo_4160);
    if (!sndfile_4160)
    {
        printf("Failed to open file: %s\n", sf_strerror(NULL));
        return -1;
    }

    // Downsample from 11025Hz to 4160Hz using Linear Interpolation.
    // TODO: need to figure out how to grab the last chunk of frames if less than 4160.
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
        float *buffer_4160 = (float *)malloc((sf_count_t)downsample_length * sizeof(float));

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

            float result = linear_interpolate(position_1, position_2, input_index, sample_1, sample_2, mu);
            // printf("================================\n");
            // printf("4160Hz index: %d\n", i);
            // printf("11025Hz indexes: %d(%f), %d(%f)\n", position_1, sample_1, position_2, sample_2);
            // printf("interpolated value: %f\n", result);
            buffer_4160[i] = result;
        }
        // print_buffer_4160(buffer_4160);

        // to implement: write buffer to new file
        count = count + frames_requested;
        free(buffer_4160);
    }
    printf("=================\n");
    printf("Finished!\n");
    printf("Count: %d\n", count);
    free(buffer_11025);
    return 0;
}

void read_samples(SNDFILE *sndfile, SF_INFO *sfinfo)
{
    sf_count_t frames = sfinfo->frames;
    sf_count_t seek_rate = 1;
    int sample_rate = sfinfo->samplerate;
    float old_buffer[11025];
    float buffer[4160];
    double scaling_factor = (double)sample_rate / 4160.0;
    float *buffer_ptr = &buffer;

    printf("Scale rate: %d\n", (sf_count_t)scaling_factor);
    printf((sf_count_t)scaling_factor);

    for (int i = 0; i < 10; i += seek_rate)
    {
        // sf_count_t current_frame = sf_seek(sndfile, seek_rate, SEEK_CUR);
        sf_count_t frame_data = sf_read_float(sndfile, buffer, 1);
        printf("Frame ID: %d Data: %f\n", i, buffer[0]);
    }
}

void print_buffer_4160(float *buffer)
{
    for (int i = 0; i < 4160; i++)
    {
        printf("index %d: %f\n", i, buffer[i]);
    }
}

void get_path()
{
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("Current working dir: %s\n", cwd);
    }
    else
    {
        perror("getcwd() error");
    }
}
