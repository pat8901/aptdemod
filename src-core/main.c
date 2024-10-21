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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <sndfile.h>
#include "audio.h"
#include "image.h"
#include <getopt.h>

void menu_about();
void menu_help();
void command_verbose();
void command_generate();

int main(int argc, char *argv[])
{

    static struct option const long_options[] =
        {
            {"verbose", no_argument, NULL, 'v'},
            {"generate", no_argument, NULL, 'g'},
            {"about", no_argument, NULL, 'a'},
            {"help", no_argument, NULL, 'h'},
            {NULL, 0, NULL, 0},
        };

    int opt = 0;

    // 1. Parse args and set args attributes in a struct
    // use "cat" as inspiration
    while ((opt = getopt_long(argc, argv, "vgah", long_options, NULL)) != -1)
    {
        switch (opt)
        {
        case 'v':
            command_verbose();
            break;
        case 'g':
            command_generate();
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

    // 2. Analyze arg struct

    // 3. Run program with given args

    /*
    // Create demodulated audio file
    create_audio();

    // Create weather satellite image from APT signal
    create_image(5512);

    create_image_reverse("./documentation/samples/audio/20210720111842.wav", "output/images/apt_image_reverse_1.bmp", 5512);
    create_image_reverse("documentation/samples/audio/NOAA1920190808-070600.wav", "output/images/apt_image_reverse_2.bmp", 5512);
    */

    return 0;
}

void menu_about()
{
    printf("*** About Menu ***\n");
}

void menu_help()
{ 
    printf("Usage: noaa-apt [OPTION] [FILE]\n");
    printf("Amplitude demodulates audio files into weather satellite images.\n\n");

    printf("%-15s   %s\n","-h, --help","Displays help menu, providing usage and miscellaneous information.");
    printf("%-15s   %s\n","-a, --about","Displays details about the program, including license information.");
    printf("%-15s   %s\n","-v, --verbose","Provide additional program information as it runs.");
    printf("%-15s   %s\n","-f [FILE], --file [FILE]","This option is followed by a file path argument which tells the program what audio file to process.\n");

    printf("%s\n    %s\n","Example:","noaa-apt -v -f /path/to/audio/file/audio.wav");
}

void command_verbose()
{
    printf("verbose=1\n");
}

/* Sets flag to generate reports during program execution.*/
void command_generate()
{
    printf("generate=1\n");
}