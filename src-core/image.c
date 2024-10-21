/*
    NOAA-APT - Lightweight APT decoder for NOAA weather satellites
    Copyright (C) 2024 Patrick O'Brien-Seitz

    NOAA-APT is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    NOAA-APT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <sndfile.h>
#include <stdlib.h>
#include <fftw3.h>
#include <math.h>
#include "demod.h"
#include "utils.h"
#include "image.h"

/* Create APT image from normalized demodulated 11025hz audio.
   Can only handle 11025Hz audio files.

   int width - The amount of pixels for the output image*/
int create_image(int width)
{
    /* Initializing 11025hz audio input file */
    SF_INFO sfinfo_input;
    SNDFILE *sndfile_input;
    sfinfo_input.format = 0;
    sf_count_t count = 0;

    /* Opening 11025hz audio input file. */
    const char *file_path = "./documentation/samples/audio/20210720111842.wav";
    sndfile_input = sf_open(file_path, SFM_READ, &sfinfo_input);
    if (!sndfile_input)
    {
        printf("Failed to open file: %s\n", sf_strerror(NULL));
        return -1;
    }

    /* Calculates the height of the output image given
       the number of frames and the width. */
    sf_count_t frames = sfinfo_input.frames;
    int height = ceil((double)(frames / 5512));

    /* Initialize and open bmp image file. */
    FILE *image;
    image = fopen("output/images/apt_image.bmp", "w+");

    /* Build bmp file header */
    BitMapFileHeader header = {
        .signature = 0x424D,
        .file_size = sizeof(BitMapFileHeader) + sizeof(BitMapInfoHeader) + (256 * sizeof(BitMapColorTable)) + (width * height),
        .reserved = 0,
        .data_offset = sizeof(BitMapFileHeader) + sizeof(BitMapInfoHeader) + (256 * sizeof(BitMapColorTable)),
    };
    BitMapFileHeader *header_ptr = &header;

    /* Build bmp info header. */
    BitMapInfoHeader InfoHeader = {
        .size = sizeof(BitMapInfoHeader),
        .width = width,
        .height = height,
        .planes = 1,
        .bits_per_pixel = 8,
        .compression = 0,
        .image_size = width * height,
        .x_pixels_per_m = 0,
        .y_pixels_per_m = 0,
        .colors_used = 256,
        .important_colors = 256,
    };
    BitMapInfoHeader *InfoHeader_ptr = &InfoHeader;

    /* Write bmp headers to the image. */
    write_file_header(image, header_ptr);
    write_info_header(image, InfoHeader_ptr);
    write_color_table(image);

    /* Get frame buffers of demodulated APT audio and write to image. */
    while (count < frames)
    {
        /* Seek to the requested 11025hz frame and
           read a selected amount of samples into a frame buffer. */
        printf("count: %d\n", count);
        sf_count_t start_frame = sf_seek(sndfile_input, count, SEEK_SET);
        double *input_buffer = (double *)fftw_malloc(sizeof(double) * 11025);
        sf_count_t frames_requested = sf_readf_double(sndfile_input, input_buffer, 11025);
        printf("Frames read: %ld\n", frames_requested);

        /* Demodulate 11025hz input frame buffer. */
        double *intermediate_buffer = am_demodulate(input_buffer, 11025, ON);

        /* Write pixel data to image. */
        for (int i = 0; i < 11024; i++)
        {
            uint32_t pixel_data = intermediate_buffer[i] * 255;
            fputc(pixel_data, image);
        }

        count += frames_requested;

        /* Clean up. */
        free(input_buffer);
        free(intermediate_buffer);
    }

    printf("=================\n");
    printf("Finished!\n");

    /* Clean up. */
    sf_close(sndfile_input);
    fclose(image);

    return 0;
}

/* Create APT image from normalized demodulated 11025hz audio in reverse.
   This is necessary for bmp files. Can only handle 11025Hz audio files.

   int width - The amount of pixels for the output image*/
int create_image_reverse(OptionFlags *ptr_flags, char *path, char *output_name, int width)
{

    /* Analyze flags to set options for program execution. */
    if (ptr_flags->generate_flag == true)
    {
        printf("generate=1\n");
    }
    if (ptr_flags->verbose_flag == true)
    {
        printf("verbose=1\n");
    }

    /* Initialize 11025hz audio input file. */
    SF_INFO sfinfo_input;
    SNDFILE *sndfile_input;
    sfinfo_input.format = 0;
    sf_count_t count = 0;

    /* Open 11025hz input audio file for reading. */
    const char *file_path = path;
    sndfile_input = sf_open(file_path, SFM_READ, &sfinfo_input);
    if (!sndfile_input)
    {
        printf("Failed to open file: %s\n", sf_strerror(NULL));
        return -1;
    }

    /* Calculates the height of the output image given
       the number of frames and the width. */
    sf_count_t frames = sfinfo_input.frames;
    int height = ceil((double)(frames / 5512));

    /* Initialize and open bmp image file. */
    FILE *image;
    image = fopen(output_name, "w+");

    /* Build bmp file header. */
    BitMapFileHeader header = {
        .signature = 0x424D,
        .file_size = sizeof(BitMapFileHeader) + sizeof(BitMapInfoHeader) + (256 * sizeof(BitMapColorTable)) + (width * height),
        .reserved = 0,
        .data_offset = sizeof(BitMapFileHeader) + sizeof(BitMapInfoHeader) + (256 * sizeof(BitMapColorTable)),
    };
    BitMapFileHeader *header_ptr = &header;

    /* Build bmp info header. */
    BitMapInfoHeader InfoHeader = {
        .size = sizeof(BitMapInfoHeader),
        .width = width,
        .height = height,
        .planes = 1,
        .bits_per_pixel = 8,
        .compression = 0,
        .image_size = width * height,
        .x_pixels_per_m = 0,
        .y_pixels_per_m = 0,
        .colors_used = 256,
        .important_colors = 256,
    };
    BitMapInfoHeader *InfoHeader_ptr = &InfoHeader;

    /* Write bmp headers to the image. */
    write_file_header(image, header_ptr);
    write_info_header(image, InfoHeader_ptr);
    write_color_table(image);

    /* Get frame buffers of demodulated APT audio and write to image. */
    while (frames - 11025 >= 0)
    {
        /* Seek to the requested 11025hz frame and
           read a selected amount of samples into a frame buffer. */
        frames -= 11025;
        printf("frames remaining: %ld\n", frames);
        sf_count_t start_frame = sf_seek(sndfile_input, frames, SEEK_SET);
        double *input_buffer = (double *)fftw_malloc(sizeof(double) * 11025);
        sf_count_t frames_requested = sf_readf_double(sndfile_input, input_buffer, 11025);
        printf("Frames read: %ld\n", frames_requested);

        /* Demodulate 11025hz input frame buffer. */
        double *intermediate_buffer = am_demodulate(input_buffer, 11025, OFF);

        /* Write pixel data to image. */
        for (int i = 0; i < 11024; i++)
        {
            uint32_t pixel_data = intermediate_buffer[i] * 255;
            fputc(pixel_data, image);
        }

        count += frames_requested;

        /* Clean up. */
        free(input_buffer);
        free(intermediate_buffer);
    }

    /* Processing the remaining frames less than 11025, if they exist. */
    printf("Frames remaining: %ld\n", frames);
    if (frames != 0)
    {
        /* Seek to the requested 11025hz frame and
           read a selected amount of samples into a frame buffer. */
        sf_count_t start_frame = sf_seek(sndfile_input, 0, SEEK_SET);
        double *input_buffer = (double *)fftw_malloc(sizeof(double) * 11025);
        sf_count_t frames_requested = sf_readf_double(sndfile_input, input_buffer, frames);
        printf("Frames read: %ld\n", frames_requested);

        /* Demodulate 11025hz input frame buffer. */
        /* TODO: How would you demod the signal if length less than 11025? pad with 0s? */
        double *intermediate_buffer = am_demodulate(input_buffer, 11025, OFF);

        /* Write pixel data to image. */
        for (int i = 0; i < 11024; i++)
        {
            uint32_t pixel_data = intermediate_buffer[i] * 255;
            fputc(pixel_data, image);
        }

        count += frames_requested;

        /* Clean up. */
        free(input_buffer);
        free(intermediate_buffer);
    }

    printf("=================\n");
    printf("Finished!\n");

    /* Clean up. */
    sf_close(sndfile_input);
    fclose(image);

    return 0;
}

/* Test function:

   Takes 1 second of audio and outputs an image using bit masking
   and bit shifting to get the correct byte. BMP is using little endian */
void create_test_image(double *buffer, int width, int height)
{
    FILE *image;
    image = fopen("documentation/images/test_11025_image.bmp", "w+");

    // Build file header
    BitMapFileHeader header = {
        .signature = 0x424D,
        .file_size = sizeof(BitMapFileHeader) + sizeof(BitMapInfoHeader) + (256 * sizeof(BitMapColorTable)) + (width * height),
        .reserved = 0,
        .data_offset = sizeof(BitMapFileHeader) + sizeof(BitMapInfoHeader) + (256 * sizeof(BitMapColorTable)),
    };
    BitMapFileHeader *header_ptr = &header;

    // Write file header to image
    write_file_header(image, header_ptr);

    // Build info header
    BitMapInfoHeader InfoHeader = {
        .size = sizeof(BitMapInfoHeader),
        .width = width,
        .height = height,
        .planes = 1,
        .bits_per_pixel = 8,
        .compression = 0,
        .image_size = width * height,
        .x_pixels_per_m = 0,
        .y_pixels_per_m = 0,
        .colors_used = 256,
        .important_colors = 256,
    };
    BitMapInfoHeader *InfoHeader_ptr = &InfoHeader;

    // Write info header to image
    write_info_header(image, InfoHeader_ptr);

    // Write color table to image
    write_color_table(image);

    // Write pixel data to image
    for (int i = 0; i < 11024; i++)
    {
        uint32_t pixel_value = buffer[i] * 255;
        printf("%d: %d\n", i, pixel_value);
        fputc(pixel_value, image);
    }

    fclose(image);
}

/* Test function.

   Creates a 500x500 color bmp image. */
void create_color_test_image()
{
    char header[54] = {
        0x42,
        0x4D,
        0xE6,
        0x71,
        0x0B,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x36,
        0x00,
        0x00,
        0x00,
        0x28,
        0x00,
        0x00,
        0x00,
        0xF4,
        0x01,
        0x00,
        0x00,
        0xF4,
        0x01,
        0x00,
        0x00,
        0x01,
        0x00,
        0x18,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0xB0,
        0x71,
        0x0B,
        0x00,
        0xC4,
        0x0E,
        0x00,
        0x00,
        0xC4,
        0x0E,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
    };
    FILE *image;
    image = fopen("documentation/images/test_image.bmp", "w+");

    // Writing header to image
    for (int i = 0; i < 54; i++)
    {
        fputc(header[i], image);
    }

    // Writing colors to image
    for (int i = 0; i < (100 * 500); i++)
    {
        fputc(255, image);
        fputc(0, image);
        fputc(0, image);
    }
    for (int i = 0; i < (100 * 500); i++)
    {
        fputc(0, image);
        fputc(0, image);
        fputc(255, image);
    }
    for (int i = 0; i < (100 * 500); i++)
    {
        fputc(160, image);
        fputc(0, image);
        fputc(0, image);
    }
    for (int i = 0; i < (100 * 500); i++)
    {
        fputc(0, image);
        fputc(120, image);
        fputc(120, image);
    }
    for (int i = 0; i < (100 * 500); i++)
    {
        fputc(120, image);
        fputc(120, image);
        fputc(0, image);
    }
    fclose(image);
}

/* Writes file header struct to a bmp image file. */
void write_file_header(FILE *image, BitMapFileHeader *file_header)
{
    fputc((file_header->signature & 0xFF00) >> (2 * 4), image);
    fputc((file_header->signature & 0x00FF), image);

    parse_dword(image, file_header->file_size);
    parse_dword(image, file_header->reserved);
    parse_dword(image, file_header->data_offset);
}

/* Writes info header struct to bmp image file. */
void write_info_header(FILE *image, BitMapInfoHeader *InfoHeader)
{
    parse_dword(image, InfoHeader->size);
    parse_dword(image, InfoHeader->width);
    parse_dword(image, InfoHeader->height);

    parse_word(image, InfoHeader->planes);
    parse_word(image, InfoHeader->bits_per_pixel);

    parse_dword(image, InfoHeader->compression);
    parse_dword(image, InfoHeader->image_size);
    parse_dword(image, InfoHeader->x_pixels_per_m);
    parse_dword(image, InfoHeader->y_pixels_per_m);
    parse_dword(image, InfoHeader->colors_used);
    parse_dword(image, InfoHeader->important_colors);
}

/* Writes color table to bmp image file.
   This is for 8-bit gray scale images. */
void write_color_table(FILE *image)
{
    for (int i = 0; i < 256; i++)
    {
        fputc(i, image);
        fputc(i, image);
        fputc(i, image);
        fputc(0, image);
    }
}

/* Parses 2 bytes of data from bmp struct and writes to image file. */
void parse_word(FILE *image, uint16_t value)
{
    fputc((value & 0x00FF), image);
    fputc((value & 0xFF00) >> (2 * 4), image);
}

/* Parses 4 bytes of data from bmp struct and writes to image file. */
void parse_dword(FILE *image, uint32_t value)
{
    fputc((value & 0x000000FF), image);
    fputc((value & 0x0000FF00) >> (2 * 4), image);
    fputc((value & 0x00FF0000) >> (4 * 4), image);
    fputc((value & 0xFF000000) >> (6 * 4), image);
}