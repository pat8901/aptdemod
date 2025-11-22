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

#ifndef Image_H
#define Image_H

#include <stdbool.h>

typedef struct OptionFlags
{
    bool file_flag;
    bool verbose_flag;
    bool generate_flag;
} OptionFlags;

typedef struct BitMapFileHeader
{
    uint16_t signature;
    uint32_t file_size;
    uint32_t reserved;
    uint32_t data_offset;
} BitMapFileHeader;

typedef struct BitMapInfoHeader
{
    uint32_t size;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bits_per_pixel;
    uint32_t compression;
    uint32_t image_size;
    uint32_t x_pixels_per_m;
    uint32_t y_pixels_per_m;
    uint32_t colors_used;
    uint32_t important_colors;
} BitMapInfoHeader;

typedef struct BitMapColorTable
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t reserved;
} BitMapColorTable;

// *Deprecated* Each line is 0.50 seconds long
typedef struct Line
{
    int time;
    int data;
    uint8_t channel_A_sync[39];
    uint8_t channel_A_space[47];
    uint8_t channel_A_image[909];
    uint8_t channel_A_telemetry[45];
    uint8_t channel_B_sync[39];
    uint8_t channel_B_space[47];
    uint8_t channel_B_image[909];
    uint8_t channel_B_telemetry[45];
} Line;

// Main image creation functions
int create_image(int width);
int create_image_reverse(OptionFlags *ptr_flags, const char *audio_path, char *output_path, int width);
void create_test_image(double *buffer, int width, int height);
void create_color_test_image();

// Byte writing to image functions
void write_file_header(FILE *image, BitMapFileHeader *file_header);
void write_info_header(FILE *image, BitMapInfoHeader *InfoHeader);
void write_color_table(FILE *image);

// Bit operation functions
void parse_word(FILE *image, uint16_t value);
void parse_dword(FILE *image, uint32_t value);

#endif