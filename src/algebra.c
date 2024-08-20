#include <stdlib.h>
#include <stdio.h>
#include <sndfile.h>
#include <fftw3.h>
#include <math.h>
#include <string.h>
#include "algebra.h"

/*
Most up to date function to properly demodulate full APT signals.
TODO: get rid of unnecessary file check-in creation
*/
double *am_demodulate(double *input_signal, int input_length)
{
    fftw_complex *buffer = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * 11025);
    fftw_complex *out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * 11025);

    /* Placing input signal contents into buffer. Can not use memcopy due to
    differing value types. Must do it manually */
    for (int i = 0; i < 11025; i++)
    {
        buffer[i][0] = (long double)input_signal[i];
        buffer[i][1] = (long double)0;
    }

    /* Preform FFT operations*/
    fftw_plan p = fftw_plan_dft_1d(11025, buffer, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p);
    fftw_destroy_plan(p);

    // Shifting the 2400 signal
    for (int i = 0; i < 2400; i++)
    {
        double real_temp = out[0][0];
        double imaginary_temp = out[0][1];
        for (int j = 1; j < 11025; j++)
        {
            out[j - 1][0] = out[j][0];
            out[j - 1][1] = out[j][1];
        }
        out[11024][0] = real_temp;
        out[11024][1] = imaginary_temp;
    }

    FILE *fp2 = fopen("./output/text/11025_am_demod.txt", "w");
    fprintf(fp2, "Real,Imaginary\n");
    for (int i = 0; i < 11025; i++)
    {
        fprintf(fp2, "%f, %f\n", out[i][0], out[i][1]);
    }
    fclose(fp2);

    // Apply passband filter
    for (int i = 0; i < 11025; i++)
    {
        if (i > 2080 && i < 8944)
        {
            out[i][0] = 0;
            out[i][1] = 0;
        }
    }
    FILE *fp3 = fopen("./output/text/11025_filter_am_demod.txt", "w");
    fprintf(fp3, "Real,Imaginary\n");
    for (int i = 0; i < 11025; i++)
    {
        fprintf(fp3, "%f, %f\n", out[i][0], out[i][1]);
    }
    fclose(fp3);

    // Normalize
    for (int i = 0; i < 11025; i++)
    {
        out[i][0] = out[i][0] / 5512.5;
        out[i][1] = out[i][1] / 5512.5;
    }

    // Preform the inverse fft
    fftw_complex *new_real_signal = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * 11025);
    fftw_complex *new_buffer = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * 11025);
    for (int i = 0; i < 11025; i++)
    {
        new_buffer[i][0] = out[i][0];
        new_buffer[i][1] = out[i][1];
    }
    fftw_plan inverse_plan = fftw_plan_dft_1d(11025, new_buffer, new_real_signal, FFTW_BACKWARD, FFTW_ESTIMATE);
    FILE *fp4 = fopen("./output/text/11025_real_am_demod.txt", "w");
    fftw_execute(inverse_plan);

    fprintf(fp4, "Real,Imaginary\n");
    for (int i = 0; i < 11025; i++)
    {
        fprintf(fp4, "%f, %f\n", new_real_signal[i][0], new_real_signal[i][1]);
    }
    fclose(fp4);
    fftw_destroy_plan(inverse_plan);

    // Magnitude calculation
    double *result = (double *)malloc(sizeof(double) * 11025);
    for (int i = 0; i < 11025; i++)
    {
        double a = new_real_signal[i][0];
        double b = new_real_signal[i][1];
        double c = (a * a) + (b * b);
        double magnitude = sqrt(c);
        result[i] = magnitude;
    }

    FILE *fp5 = fopen("./output/text/11025_mag_am_demod.txt", "w");
    fprintf(fp5, "Real\n");
    for (int i = 0; i < 11025; i++)
    {
        fprintf(fp5, "%f\n", result[i]);
    }
    fclose(fp5);

    // Clean up
    free(buffer);
    free(out);
    free(new_buffer);
    free(new_real_signal);

    return result;
}

/*
Deprecated. This function passband filters a second of APT data,
resulting in isolating 2400 Hz signal.
*/
fftw_complex *am_demod_single_11025(double *input_signal, int input_length)
{
    fftw_complex *buffer = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * 11025);
    fftw_complex *out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * 11025);

    /* Placing input signal contents into buffer. Can not use memcopy due to
    differing value types. Must do it manually */
    for (int i = 0; i < 11025; i++)
    {
        buffer[i][0] = (long double)input_signal[i];
        buffer[i][1] = (long double)0;
    }

    /* Preform FFT operations*/
    fftw_plan p = fftw_plan_dft_1d(11025, buffer, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p);
    fftw_destroy_plan(p);

    // Setting negative frequencies to 0.
    for (int i = 5513; i < 11025; i++)
    {
        out[i][0] = (long double)0;
        out[i][1] = (long double)0;
    }

    // Removing the 0 frequency
    out[0][0] = 0;
    out[0][1] = 0;

    // Apply passband filter
    passband_filter(out, 2300, 2500);

    FILE *fp2 = fopen("./output/text/11025_frequency_filter.txt", "w");
    fprintf(fp2, "Real,Imaginary\n");
    for (int i = 0; i < 11025; i++)
    {
        fprintf(fp2, "%f, %f\n", out[i][0], out[i][1]);
    }
    fclose(fp2);

    // preform the inverse fft
    fftw_complex *new_real_signal = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * 11025);
    fftw_complex *new_buffer = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * 11025);
    for (int i = 0; i < 11025; i++)
    {
        new_buffer[i][0] = out[i][0];
        new_buffer[i][1] = out[i][1];
    }
    fftw_plan inverse_plan = fftw_plan_dft_1d(11025, new_buffer, new_real_signal, FFTW_BACKWARD, FFTW_ESTIMATE);
    FILE *fp3 = fopen("./output/text/11025_real_signal.txt", "w");
    fftw_execute(inverse_plan);

    fprintf(fp3, "Real,Imaginary\n");
    for (int i = 0; i < 11025; i++)
    {
        fprintf(fp3, "%f, %f\n", new_real_signal[i][0], new_real_signal[i][1]);
    }
    fclose(fp3);
    fftw_destroy_plan(inverse_plan);

    return new_real_signal;
}

// Takes in a complex signal and applies a passband fillter to get 2400Hz
void passband_filter(fftw_complex *signal, int from, int to)
{
    for (int i = 0; i < 5513; i++)
    {
        if (i < from || i > to)
        {
            signal[i][0] = 0;
            signal[i][1] = 0;
        }
    }
}

// https://www.cuemath.com/linear-interpolation-formula/
float linear_interpolate(sf_count_t x_0, sf_count_t x_1, float x, float y_0, float y_1)
{
    float y = y_0 + ((x - x_0) * ((y_1 - y_0) / (x_1 - x_0)));
    return y;
}

void finite_impulse_response()
{
}

/* Deprecated, may use this function in demodulation process to containerize work
https://www.fftw.org/fftw3_doc/Complex-One_002dDimensional-DFTs.html
https://en.wikipedia.org/wiki/Cooley%E2%80%93Tukey_FFT_algorithm
*/
void fast_fourier_transform(double *input_signal, int input_length)
{
}
