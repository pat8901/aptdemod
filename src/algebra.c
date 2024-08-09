#include <stdlib.h>
#include <stdio.h>
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

    fftw_complex *garbage = (fftw_complex *)malloc(sizeof(fftw_complex) * 4800);
    fftw_complex *out = (fftw_complex *)malloc(sizeof(fftw_complex) * 4800);

    // TODO: Is there a way to use memcpy() instead?
    // memcpy(garbage, input_signal, sizeof(double) * input_length);
    for (int i = 0; i < 4160; i++)
    {
        garbage[i][0] = (long double)input_signal[i];
        garbage[i][1] = (long double)0;
    }
    for (int i = 4160; i < 4800; i++)
    {
        garbage[i][0] = (long double)0;
        garbage[i][1] = (long double)0;
    }

    // Print to verify values where properly copied over.
    // for (int i = 0; i < 4800; i++)
    // {
    //     printf("%d: %f, %f\n", i, garbage[i][0], garbage[i][1]);
    // }

    // TODO: Verify that this is getting the correct results.
    fftw_plan plan = fftw_plan_dft_1d(4800, garbage, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(plan);

    // Print to see the values of the first fft.
    // for (int i = 0; i < 2410; i++)
    // {
    //     printf("%d: %f, %f\n", i, out[i][0], out[i][1]);
    // }
    FILE *fp = fopen("./documentation/output/plots/after_fft.txt", "w");
    for (int i = 0; i < 4800; i++)
    {
        fprintf(fp, "%f, %f\n", out[i][0], out[i][1]);
    }
    fclose(fp);

    /* Set the negatives frequencies of fft to 0. */
    for (int i = 2401; i < 4800; i++)
    {
        out[i][0] = 0;
        out[i][1] = 0;
    }

    // Print to verify if frequencies were set to 0.
    for (int i = 0; i < 2405; i++)
    {
        printf("%d: %f, %f\n", i, out[i][0], out[i][1]);
    }

    fftw_destroy_plan(plan);
    fftw_free(garbage);
    fftw_free(out);
}

void fft_test()
{
    fftw_complex *in = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * 10);
    fftw_complex *out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * 10);

    for (int i = 0; i < 10; i++)
    {
        in[i][0] = (double)(2 * i);
        in[i][1] = (double)0;
    }

    printf("================\n");
    for (int i = 0; i < 10; i++)
    {
        printf("%d: %f, %f\n", i, in[i][0], in[i][1]);
    }

    fftw_plan p = fftw_plan_dft_1d(10, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p);
    printf("================\n");
    for (int i = 0; i < 10; i++)
    {
        printf("%d: %f, %f\n", i, out[i][0], out[i][1]);
    }
}

void fftw_test_11025(double *input_signal, int input_length)
{

    FILE *f_input = fopen("./documentation/output/plots/before_fft_11025_real_signal.txt", "w");
    for (int i = 0; i < 11025; i++)
    {
        fprintf(f_input, "%f, %f\n", input_signal[i]);
    }
    fclose(f_input);
    fftw_complex *buffer = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * 11025);
    fftw_complex *out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * 11025);

    for (int i = 0; i < 11025; i++)
    {
        buffer[i][0] = (long double)input_signal[i];
        buffer[i][1] = (long double)0;
    }

    fftw_plan p = fftw_plan_dft_1d(11025, buffer, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p);
    fftw_destroy_plan(p);

    FILE *fp = fopen("./documentation/output/plots/after_fft_11025.txt", "w");
    for (int i = 0; i < 11025; i++)
    {
        fprintf(fp, "%f, %f\n", out[i][0], out[i][1]);
    }
    fclose(fp);

    // Setting negative frequencies to 0.
    for (int i = 5513; i < 11025; i++)
    {
        out[i][0] = (long double)0;
        out[i][1] = (long double)0;
    }
    FILE *fp2 = fopen("./documentation/output/plots/after_fft_11025_negative.txt", "w");
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
    FILE *fp3 = fopen("./documentation/output/plots/after_fft_11025_real_signal.txt", "w");
    fftw_execute(inverse_plan);

    for (int i = 0; i < 11025; i++)
    {
        fprintf(fp3, "%f, %f\n", new_real_signal[i][0], new_real_signal[i][1]);
    }
    fclose(fp3);
    fftw_destroy_plan(inverse_plan);
}

void hilbert_transform(double *input_signal, int input_length)
{
    fast_fourier_transform(input_signal, input_length);
}