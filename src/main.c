#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sndfile.h>
#include "apt.h"
#include "image.h"

int main(int argc, char *argv[])
{
    // Create demodulated audio file
    // create_audio();

    // Create weather satellite image from APT signal
    create_image(5512);
    return 0;
}