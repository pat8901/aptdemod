#ifndef Image_H
#define Image_H

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