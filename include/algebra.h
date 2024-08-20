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

#ifndef Algebra_H
#define Algebra_H

float linear_interpolate(sf_count_t x_0, sf_count_t x_1, float x, float y_0, float y_1);
void finite_impulse_response();

void fast_fourier_transform(double *input_signal, int input_length);

fftw_complex *am_demod_single_11025(double *input_signal, int input_length);
double *am_demodulate(double *input_signal, int input_length);
void passband_filter(fftw_complex *signal, int from, int to);

#endif