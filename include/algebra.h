#ifndef Algebra_H
#define Algebra_H

float linear_interpolate(sf_count_t x_0, sf_count_t x_1, float x, float y_0, float y_1);
void finite_impulse_response();
void fast_fourier_transform(double *input_signal, int input_length);
void fft_test();
void fftw_test_11025(double *input_signal, int input_length);
void hilbert_transform();
void after_filter_11025(double *input_signal, int input_length);
void passband_filter(fftw_complex *signal, int from, int to);
double *am_demod_11025(double *input_signal, int input_length);

#endif