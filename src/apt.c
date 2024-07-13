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

// TODO correct the name of this function and determine a way to dynamically size buffer to insert appropriate number of samples
//      to match the size of a byte. Ideally I would want a sample rate of 4160. This would give me a byte per sample.
void seek(SNDFILE *sndfile, SF_INFO *sfinfo)
{
    // sf_count_t my_frame = sf_seek(sndfile, 1, SEEK_CUR);
    // printf("Frame ID: %ld\n", my_frame);
    // my_frame = sf_seek(sndfile, 1, SEEK_CUR);
    // printf("Frame ID: %ld\n", my_frame);
    // my_frame = sf_seek(sndfile, 1, SEEK_CUR);
    // printf("Frame ID: %ld\n", my_frame);
    // my_frame = sf_seek(sndfile, 1, SEEK_CUR);
    // printf("Frame ID: %ld\n", my_frame);
    // my_frame = sf_seek(sndfile, -2, SEEK_END);
    // printf("Frame ID: %ld\n", my_frame);

    sf_count_t frames = sfinfo->frames;
    sf_count_t seek_rate = 1;
    int sample_rate = sfinfo->samplerate;
    float buffer[1];
    float *buffer_ptr = &buffer;

    for (int i = 0; i < 256; i += seek_rate)
    {
        // sf_count_t current_frame = sf_seek(sndfile, seek_rate, SEEK_CUR);
        sf_count_t frame_data = sf_read_float(sndfile, buffer, 1);
        printf("Frame ID: %ld Data: %f\n", i, buffer[0]);
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
