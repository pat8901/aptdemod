# NOAA APT Signal Decoder

This C program demodulates Automatic Picture Transmission (APT) radio signals from NOAA satellites, converting the received signals into weather images. By processing the APT signals, the program extracts and reconstructs the image data, providing weather visualizations from satellite transmissions. Ideal for amateur radio enthusiasts and weather monitoring applications, this decoder leverages signal processing techniques to deliver accurate and detailed images.

# Introduction
This program was developed for Linux.

### What is APT
The Automatic Picture Transmission (APT) system is an analog image transmission system developed for use on weather satellites. It was introduced in the 1960s. A user station anywhere in the world can receive local data at least twice a day from each satellite as it passes nearly overhead.

For more details, please see the [additional information](https://en.wikipedia.org/wiki/Automatic_picture_transmission).

# Building / Installing

<h2 style="border-bottom: none;">Linux</h2>
Noaa-apt has only been tested on Debian based distros.
In the future this will change, but in the mean time we recommend using Debian based distros. 

### Dependencies

<details>

<summary>Install Dependencies - Debian, Ubuntu, and other Debian-based distros</summary>

```bash
sudo apt install git cmake build-essential libfftw3-dev libsndfile1-dev
```

</details>
<details>

<summary>Install Dependencies - Red-Hat-based systems</summary>

```bash
# TBA
```

</details>
<details>

<summary>Install Dependencies - Arch-based systems</summary>

```bash
# TBA
```

</details>
<details>

<summary>Install Dependencies - Gentoo</summary>

```bash
# TBA
```

</details>

### Build noaa-apt
```bash
# Clone noaa-apt repo
git clone https://github.com/pat8901/noaa-apt.git
cd noaa-apt

# Build noaa-apt
make

# Run
./noaa-apt
```

# Quick Start

### Usage
```bash
noaa-apt [options] [arguments]
```

### Available Options/Arguments
```
-h --help: Displays help menu, providing usage and miscellaneous information.
-a --about: Displays details about the program, including license information.
-v --verbose: Provide additional program information as it runs.
-f [file] --file [file]: This option is followed by a file path argument which tells the program what audio file to process.
```

### Sample command
```bash
noaa-apt -v -f /path/to/audio/file/audio.wav
```