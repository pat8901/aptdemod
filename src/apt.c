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

    // Opening audio file
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

// TODO:
// correct the name of this function and determine a way to dynamically size buffer to insert appropriate number of samples
// to match the size of a byte. Ideally I would want a sample rate of 4160. This would give me a byte per sample.
void seek(SNDFILE *sndfile, SF_INFO *sfinfo)
{
    sf_count_t frames = sfinfo->frames;
    sf_count_t high_frames_amount = 11025;
    sf_count_t low_frame_amount = 4160;
    int sample_rate = sfinfo->samplerate;
    double seek_rate = (double)sample_rate / 4160.0;
    float *old_buffer = (float *)malloc(high_frames_amount * sizeof(float));
    float *new_buffer = (float *)malloc(low_frame_amount * sizeof(float));

    printf("Seek rate: %lf\n", seek_rate);

    sf_count_t start_frame = sf_seek(sndfile, 0, SEEK_SET);
    sf_readf_float(sndfile, old_buffer, high_frames_amount);

    for (int i = 0; i < 4160; i++)
    {
        sf_count_t next = (sf_count_t)i * seek_rate;
        float value = old_buffer[i];
        printf("low:%d high:%d\n", i, next);
    }
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

void print_buffer(float *buffer)
{
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
