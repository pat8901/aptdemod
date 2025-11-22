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

#ifndef Audio_H
#define Audio_H

int down_sample(SNDFILE *sndfile, SF_INFO *sfinfo);

int create_audio();
int create_audio_single(double *buffer);

double *get_4160_sample();
double *get_11025_sample();

#endif