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

#include <stdlib.h>
#include <stdio.h>
#include <sndfile.h>
#include <fftw3.h>
#include <math.h>
#include <string.h>
#include "demod.h"

/* Amplitude demodulates a finite length frame buffer holding a 11025hz analog signal. */
double *am_demodulate(double *input_signal, int input_length, int generate_stats)
{   

    if(input_length == 11025){

    }

    /* Initializing input and output buffers. */
    fftw_complex *buffer = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * 11025);
    fftw_complex *out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * 11025);

    /* Placing input signal samples into input buffer. */
    for (int i = 0; i < 11025; i++)
    {
        buffer[i][0] = (long double)input_signal[i];
        buffer[i][1] = (long double)0;
    }

    /* Preform FFT operations on input frame buffer and place
       results into output buffer. This transforms the audio signal
       from the time domain into the frequency domain.*/
    fftw_plan p = fftw_plan_dft_1d(11025, buffer, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p);
    fftw_destroy_plan(p);

    /* Shifting the 2400hz signal down to 0hz to center the data package.*/
    fftw_complex *shifted = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * 11025);
    int offset = 11025 - 2400;
    for (int i = 2400; i < 11025; i++)
    {
        shifted[i - 2400][0] = out[i][0];
        shifted[i - 2400][1] = out[i][1];
    }

    for (int i = 0; i < 2400; i++)
    {
        shifted[i + offset][0] = out[i][0];
        shifted[i + offset][1] = out[i][1];
    }

    for (int i = 0; i < 11025; i++)
    {
        out[i][0] = shifted[i][0];
        out[i][1] = shifted[i][1];
    }

    /* If activated, generate a report on the output
       data after being transformed into the frequency domain.*/
    if (generate_stats == ON)
    {
        FILE *fp2 = fopen("./output/text/11025_am_demod.txt", "w");
        fprintf(fp2, "Real,Imaginary\n");
        for (int i = 0; i < 11025; i++)
        {
            fprintf(fp2, "%f, %f\n", out[i][0], out[i][1]);
        }
        fclose(fp2);
    }

    /* Apply passband filter to isolate the data package
       centered on the 0hz frequency. */
    for (int i = 0; i < 11025; i++)
    {
        if (i > 2080 && i < 8944)
        {
            out[i][0] = 0;
            out[i][1] = 0;
        }
    }

    /* If activated, generate a report on the ouput data after
       the data package has been isolated.*/
    if (generate_stats == ON)
    {
        FILE *fp3 = fopen("./output/text/11025_filter_am_demod.txt", "w");
        fprintf(fp3, "Real,Imaginary\n");
        for (int i = 0; i < 11025; i++)
        {
            fprintf(fp3, "%f, %f\n", out[i][0], out[i][1]);
        }
        fclose(fp3);
    }

    /* Normalize output data. I do not remember why 5512.5*/
    for (int i = 0; i < 11025; i++)
    {
        out[i][0] = out[i][0] / 5512.5;
        out[i][1] = out[i][1] / 5512.5;
    }

    /* Preform the inverse fft on the ouput buffer to transform
       it back into the time domain. */
    fftw_complex *new_real_signal = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * 11025);
    fftw_complex *new_buffer = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * 11025);
    for (int i = 0; i < 11025; i++)
    {
        new_buffer[i][0] = out[i][0];
        new_buffer[i][1] = out[i][1];
    }
    fftw_plan inverse_plan = fftw_plan_dft_1d(11025, new_buffer, new_real_signal, FFTW_BACKWARD, FFTW_ESTIMATE);
    fftw_execute(inverse_plan);
    fftw_destroy_plan(inverse_plan);

    /* If activated, generate a report on the output data after it was
       transformed back into the time domain. */
    if (generate_stats == ON)
    {
        FILE *fp4 = fopen("./output/text/11025_real_am_demod.txt", "w");
        fprintf(fp4, "Real,Imaginary\n");
        for (int i = 0; i < 11025; i++)
        {
            fprintf(fp4, "%f, %f\n", new_real_signal[i][0], new_real_signal[i][1]);
        }
        fclose(fp4);
    }

    /* Calculate the magnitude between our two analog signals in the time domain.
       Getting the magnitude effectively amplitude demodulates out signal. */
    double *result = (double *)malloc(sizeof(double) * 11025);
    for (int i = 0; i < 11025; i++)
    {
        double a = new_real_signal[i][0];
        double b = new_real_signal[i][1];
        double c = (a * a) + (b * b);
        double magnitude = sqrt(c);
        result[i] = magnitude;
    }

    /* If activated, generate a report on the final results showing
       magnitude calculations from our result buffer.*/
    if (generate_stats == ON)
    {
        FILE *fp5 = fopen("./output/text/11025_mag_am_demod.txt", "w");
        fprintf(fp5, "Real\n");
        for (int i = 0; i < 11025; i++)
        {
            fprintf(fp5, "%f\n", result[i]);
        }
        fclose(fp5);
    }

    /* Clean up. */
    free(buffer);
    free(out);
    free(shifted);
    free(new_buffer);
    free(new_real_signal);

    return result;
}

/* Takes in a complex signal and applies a passband fillter to get 2400Hz */
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

/* DEPRECATED: Amplitude demodulates a single finite length frame
   buffer holding a 11025hz analog signal.*/
fftw_complex *am_demod_single_11025(double *input_signal, int input_length, int generate_stats)
{
    if(input_length == 11025){
        
    }

    /* Initializing input and output buffers. */
    fftw_complex *buffer = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * 11025);
    fftw_complex *out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * 11025);

    /* Placing input signal samples into input buffer. */
    for (int i = 0; i < 11025; i++)
    {
        buffer[i][0] = (long double)input_signal[i];
        buffer[i][1] = (long double)0;
    }

    /* Preform FFT operations on input frame buffer and place
       results into output buffer. This transforms the audio signal
       from the time domain into the frequency domain.*/
    fftw_plan p = fftw_plan_dft_1d(11025, buffer, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p);
    fftw_destroy_plan(p);

    /* Removing negative frequencies. */
    for (int i = 5513; i < 11025; i++)
    {
        out[i][0] = (long double)0;
        out[i][1] = (long double)0;
    }

    /* Removing the 0 frequency. */
    out[0][0] = 0;
    out[0][1] = 0;

    /* Apply passband filter to isolate the data package
       centered on the 0hz frequency. */
    passband_filter(out, 2300, 2500);

    if (generate_stats == ON)
    {
        FILE *fp2 = fopen("./output/text/11025_frequency_filter.txt", "w");
        fprintf(fp2, "Real,Imaginary\n");
        for (int i = 0; i < 11025; i++)
        {
            fprintf(fp2, "%f, %f\n", out[i][0], out[i][1]);
        }
        fclose(fp2);
    }

    /* Preform the inverse fft on the ouput buffer to transform
       it back into the time domain. */
    fftw_complex *new_real_signal = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * 11025);
    fftw_complex *new_buffer = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * 11025);
    for (int i = 0; i < 11025; i++)
    {
        new_buffer[i][0] = out[i][0];
        new_buffer[i][1] = out[i][1];
    }
    fftw_plan inverse_plan = fftw_plan_dft_1d(11025, new_buffer, new_real_signal, FFTW_BACKWARD, FFTW_ESTIMATE);
    fftw_execute(inverse_plan);
    fftw_destroy_plan(inverse_plan);

    if (generate_stats == ON)
    {
        FILE *fp3 = fopen("./output/text/11025_real_signal.txt", "w");
        fprintf(fp3, "Real,Imaginary\n");
        for (int i = 0; i < 11025; i++)
        {
            fprintf(fp3, "%f, %f\n", new_real_signal[i][0], new_real_signal[i][1]);
        }
        fclose(fp3);
    }

    return new_real_signal;
}

/* TBD: Amplitude demodulates an APT signal in real time. */
void finite_impulse_response()
{
}
