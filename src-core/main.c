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

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <sndfile.h>
#include <getopt.h>
#include "audio.h"
#include "image.h"
#include "utils.h"

void menu_about();
void menu_help();

int main(int argc, char *argv[])
{

    OptionFlags flags = {.file_flag = false, .verbose_flag = false, .generate_flag = false};
    OptionFlags *ptr_flags = &flags;
    char audio_path[256];
    int opt;

    static struct option const long_options[] =
        {
            {"file", required_argument, NULL, 'f'},
            {"verbose", no_argument, NULL, 'v'},
            {"generate", no_argument, NULL, 'g'},
            {"about", no_argument, NULL, 'a'},
            {"help", no_argument, NULL, 'h'},
            {NULL, 0, NULL, 0},
        };

    /* Parse args and set args attributes in a struct. */
    while ((opt = getopt_long(argc, argv, "f:vgah", long_options, NULL)) != -1)
    {
        switch (opt)
        {
        case 'f':
            flags.file_flag = true;
            strncpy(audio_path, optarg, 256);
            printf("file path=\"%s\"\n", audio_path);
            break;
        case 'v':
            printf("set verbose flag to true\n");
            flags.verbose_flag = true;
            break;
        case 'g':
            printf("set generate flag to true\n");
            flags.generate_flag = true;
            break;
        case 'a':
            menu_about();
            return 0;
        case 'h':
            menu_help();
            return 0;
        default:
            menu_help();
            return 0;
        }
    }

    /* Analyze option and argument logic. */
    if (!flags.file_flag)
    {
        printf("Please supply a file using -f, --file\n");
        return EXIT_FAILURE;
    }

    /* Run program with given args. */
    // TODO: need to set output directory properly, to avoid seg fault.
    create_image_reverse(ptr_flags, audio_path, "./apt_image.bmp", 5512);

    return EXIT_SUCCESS;
}

void menu_about()
{
    printf("*** About Menu ***\n");
}

void menu_help()
{
    printf("Usage: aptdemod [OPTION] [FILE]\n");
    printf("Amplitude demodulates audio files into weather satellite images.\n\n");

    printf("%-15s   %s\n", "-h, --help", "Displays help menu, providing usage and miscellaneous information.");
    printf("%-15s   %s\n", "-a, --about", "Displays details about the program, including license information.");
    printf("%-15s   %s\n", "-v, --verbose", "Provide additional program information as it runs.");
    printf("%-15s   %s\n", "-f [FILE], --file [FILE]", "This option is followed by a file path argument which tells the program what audio file to process.\n");

    printf("%s\n    %s\n", "Example:", "aptdemod -f /path/to/audio/file/audio.wav");
}
