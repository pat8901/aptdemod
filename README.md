# NOAA APT Signal Decoder

This C program decodes Automatic Picture Transmission (APT) radio signals from NOAA satellites, converting the received signals into weather images. By processing the APT signals, the program extracts and reconstructs the image data, providing weather visualizations from satellite transmissions. Ideal for amateur radio enthusiasts and weather monitoring applications, this decoder leverages signal processing techniques to deliver accurate and detailed images.

## Disclaimer
This program was developed for Linux. Testing has only been done for Linux based systems. In the future useability of this program on additional platforms will be preformed.

## How to install
1. Clone the repo
2. Install fftw3 (Fourier Transform library)
3. Install libsndfile (audio library)


## How to use
 
## APT

## TODO
    Short term:
    Add function to down sample 1 second of APT data from 11025Hz to 4160HZ.
    Add support for sync finding to line up images more accurately.
    Improve/refactor python plotting.
    Improve makefile.

    Long term:
    Add cli support.
    Add real time demodulation.
