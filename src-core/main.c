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
#include "apt.h"
#include "image.h"

void menu_help();
void command_verbose();

int main(int argc, char *argv[])
{
    if (argc >= 2)
    {
        // Check for help and ignore all other commands
        for (int i = 1; i < argc; i++)
        {
            if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0)
            {
                menu_help();
                return 1;
            }
        }

        // Process commands
        for (int i = 1; i < argc; i++)
        {
            if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0)
            {
                command_verbose();
            }
            else
            {
                printf("Unknown command: %s\n", argv[i]);
            }
        }
    }
    else
    {
        printf("No commands given\n");
    }

    // Create demodulated audio file
    // create_audio();

    // Create weather satellite image from APT signal
    // create_image(5512);

    // create_image_reverse("./documentation/samples/audio/20210720111842.wav", "output/images/apt_image_reverse_1.bmp", 5512);
    // create_image_reverse("documentation/samples/audio/NOAA1920190808-070600.wav", "output/images/apt_image_reverse_2.bmp", 5512);

    return 0;
}

void menu_help()
{
    printf("*** Help Menu ***\n");
}

void command_verbose()
{
    printf("verbose=1\n");
}