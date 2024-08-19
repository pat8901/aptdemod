#include <stdio.h>
#include <sndfile.h>
#include "image.h"

write_file_header(FILE *image, BitMapFileHeader *file_header)
{
    fputc((file_header->signature & 0xFF00) >> (2 * 4), image);
    fputc((file_header->signature & 0x00FF), image);

    parse_dword(image, file_header->file_size);
    parse_dword(image, file_header->reserved);
    parse_dword(image, file_header->data_offset);
}

write_info_header(FILE *image, BitMapInfoHeader *InfoHeader)
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

write_color_table(FILE *image)
{
    for (int i = 0; i < 256; i++)
    {
        fputc(i, image);
        fputc(i, image);
        fputc(i, image);
        fputc(0, image);
    }
}

parse_word(FILE *image, uint16_t value)
{
    fputc((value & 0x00FF), image);
    fputc((value & 0xFF00) >> (2 * 4), image);
}

parse_dword(FILE *image, uint32_t value)
{
    fputc((value & 0x000000FF), image);
    fputc((value & 0x0000FF00) >> (2 * 4), image);
    fputc((value & 0x00FF0000) >> (4 * 4), image);
    fputc((value & 0xFF000000) >> (6 * 4), image);
}