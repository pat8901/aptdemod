#ifndef Apt_H
#define Apt_H
#define byte char

// each line is 0.50 seconds long
// text
struct Line
{

    int time;
    int data;

    unsigned byte channel_A_sync[39];
    unsigned byte channel_A_space[47];
    unsigned byte channel_A_image[909];
    unsigned byte channel_A_telemetry[45];

    unsigned byte channel_B_sync[39];
    unsigned byte channel_B_space[47];
    unsigned byte channel_B_image[909];
    unsigned byte channel_B_telemetry[45];
};

struct Line frame[128];

void pack_sync(struct Line *line_ptr, char channel);
void pack_space(struct Line *line_ptr, char channel);
void pack_image(struct Line *line_ptr, char channel);
void pack_telemetry(struct Line *line_ptr, char channel);
void fill_frame();
void print_frame();

void get_path();
float linear_interpolate(sf_count_t x_0, sf_count_t x_1, float x, float y_0, float y_1, float mu);
int seek(SNDFILE *sndfile, SF_INFO *sfinfo);
void read_samples(SNDFILE *sndfile, SF_INFO *sfinfo);
void print_buffer_4160(float *buffer);

#endif