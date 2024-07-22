#include <stdlib.h>
#include <sndfile.h>
// #include <complex.h>
#include <fftw3.h>
#include <math.h>
#include <string.h>

// https://www.cuemath.com/linear-interpolation-formula/
float linear_interpolate(sf_count_t x_0, sf_count_t x_1, float x, float y_0, float y_1)
{
    float y = y_0 + ((x - x_0) * ((y_1 - y_0) / (x_1 - x_0)));
    return y;
}

void finite_impulse_response()
{
}

// https://www.fftw.org/fftw3_doc/Complex-One_002dDimensional-DFTs.html
// https://en.wikipedia.org/wiki/Cooley%E2%80%93Tukey_FFT_algorithm
// TODO: Not sure how to properly memcpy()
void fast_fourier_transform(double *input_signal, int input_length)
{
    // for (int i = 0; i < 4160; i++)
    // {
    //     printf("%d: %f\n", i, input_signal[i]);
    // }

    fftw_complex *garbage = (fftw_complex *)malloc(sizeof(fftw_complex) * input_length);
    fftw_complex *out = (fftw_complex *)malloc(sizeof(fftw_complex) * input_length);

    // TODO: Is there a way to use memcpy() instead?
    // memcpy(garbage, input_signal, sizeof(double) * input_length);
    for (int i = 0; i < 4160; i++)
    {
        garbage[i][0] = (long double)input_signal[i];
    }

    for (int i = 0; i < input_length; i++)
    {
        printf("%d: %f\n", i, garbage[i][1]);
    }

    // TODO: Verify that this is getting the correct results
    fftw_plan plan = fftw_plan_dft_1d(input_length, garbage, out, FFTW_FORWARD, FFTW_ESTIMATE);

    fftw_execute(plan);
    // for (int i = 0; i < 1050; i++)
    // {
    //     printf("%d: %f, %f\n", i, out[i][0], out[i][1]);
    // }

    fftw_destroy_plan(plan);
    fftw_free(garbage);
    fftw_free(out);
}

void hilbert_transform(double *input_signal, int input_length)
{
    fast_fourier_transform(input_signal, input_length);
}