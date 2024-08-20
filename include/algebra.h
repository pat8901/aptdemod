#ifndef Algebra_H
#define Algebra_H

float linear_interpolate(sf_count_t x_0, sf_count_t x_1, float x, float y_0, float y_1);
void finite_impulse_response();

void fast_fourier_transform(double *input_signal, int input_length);

fftw_complex *am_demod_single_11025(double *input_signal, int input_length);
double *am_demodulate(double *input_signal, int input_length);
void passband_filter(fftw_complex *signal, int from, int to);

#endif