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

#ifndef Demod_H
#define Demod_H

#define OFF 0 /* Turn off stat generation*/
#define ON 1  /* Turn on stat generation*/

double *am_demodulate(double *input_signal, int input_length, int generate_stats);

fftw_complex *am_demod_single_11025(double *input_signal, int input_length, int generate_stats);
void passband_filter(fftw_complex *signal, int from, int to);
void finite_impulse_response();
void fast_fourier_transform(double *input_signal, int input_length);

#endif