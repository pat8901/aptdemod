#include <fftw3.h>
#include "unity.h"
#include <stdlib.h>
#include "demod.h"

void setUp()
{
    fftw_complex *buffer = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * 11025);
    free(buffer);
}

void tearDown()
{
}

void test_is_equal(void)
{
    int a = 10;
    TEST_ASSERT_EQUAL(10, a);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_is_equal);

    UNITY_END();

    return 0;
}