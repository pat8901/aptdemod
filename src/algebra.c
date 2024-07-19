#include <sndfile.h>

// https://www.cuemath.com/linear-interpolation-formula/
float linear_interpolate(sf_count_t x_0, sf_count_t x_1, float x, float y_0, float y_1, float mu)
{
    // two different ways to achieve the same result.
    float y = y_0 + ((x - x_0) * ((y_1 - y_0) / (x_1 - x_0)));
    float result = (y_0 * (1 - mu)) + (y_1 * mu);
    // printf("y = %f | result = %f\n", y, result);
    return y;
}