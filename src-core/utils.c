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

#include <stdio.h>
#include <string.h>
#include <linux/limits.h>
#include <sndfile.h>
#include <stdlib.h>
#include <unistd.h>

// https://www.cuemath.com/linear-interpolation-formula/
float linear_interpolate(sf_count_t x_0, sf_count_t x_1, float x, float y_0, float y_1)
{
    float y = y_0 + ((x - x_0) * ((y_1 - y_0) / (x_1 - x_0)));
    return y;
}

/* Returns the current path from where the user is currently located in the directories. */
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

/* Print a buffer that holds double values. */
void print_buffer_double(double *buffer, int length)
{
    for (int i = 0; i < length; i++)
    {
        printf("index %d: %f\n", i, buffer[i]);
    }
}
