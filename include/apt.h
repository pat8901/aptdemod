#ifndef Apt_H
#define Apt_H

int down_sample(SNDFILE *sndfile, SF_INFO *sfinfo);
void read_samples(SNDFILE *sndfile, SF_INFO *sfinfo);

double *get_4160_sample();
double *get_11025_sample();

void create_audio_single(double *buffer);
void create_audio();

#endif