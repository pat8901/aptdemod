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
#include <string.h>
#include <linux/limits.h>

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

void print_buffer_double(double *buffer, int length)
{
    for (int i = 0; i < length; i++)
    {
        printf("index %d: %f\n", i, buffer[i]);
    }
}

void memory_copy_practice()
{
    int *old_ptr = (int *)malloc(sizeof(int) * 10);
    long int *new_ptr = (long int *)malloc(sizeof(long int) * 10);
    for (int i = 0; i < 10; i++)
    {
        old_ptr[i] = (2 * i) + 1;
    }

    for (int i = 0; i < 10; i++)
    {
        printf("%d: %d size: %ld\n", i, old_ptr[i], sizeof(old_ptr[i]));
    }
    printf("=========\n");
    for (int i = 0; i < 10; i++)
    {
        printf("%d: %d\n", i, new_ptr[i]);
    }
    printf("======\n");

    // memcpy(new_ptr, old_ptr, sizeof(int) * 10);
    for (int i = 0; i < 10; i++)
    {
        new_ptr[i] = (long int)old_ptr[i];
    }

    for (int i = 0; i < 10; i++)
    {
        printf("%d: %d size: %ld\n", i, new_ptr[i], sizeof(new_ptr[i]));
    }
}