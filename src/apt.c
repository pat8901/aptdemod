// https://www.sigidwiki.com/wiki/Automatic_Picture_Transmission_(APT)
// https://en.wikipedia.org/wiki/Automatic_picture_transmission
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "apt.h"

int main(int argc, char *argv[])
{

    fill_frame();
    return 0;
}

void pack_sync(struct Line *line_ptr, char channel)
{
    srand(time(NULL));

    if (channel == 'A')
    {
        for (int i = 0; i < sizeof(line_ptr->channel_A_sync) / sizeof(line_ptr->channel_A_sync[0]); i++)
        {
            int number = rand() % 256;
            printf("%d\n", number);
            line_ptr->channel_A_sync[i] = number;
        }
    }
    else
    {
        for (int i = 0; i < sizeof(line_ptr->channel_B_sync) / sizeof(line_ptr->channel_B_sync[0]); i++)
        {
            int number = rand() % 256;
            line_ptr->channel_B_sync[i] = number;
        }
    }
}
void pack_space(struct Line *line_ptr, char channel)
{
    srand(time(NULL));
    if ((int)channel == 'A')
    {
        for (int i = 0; i < sizeof(line_ptr->channel_A_space) / sizeof(line_ptr->channel_A_space[0]); i++)
        {
            int number = rand() % 256;
            printf("%d\n", number);
            line_ptr->channel_A_space[i] = number;
        }
    }
    else
    {
        for (int i = 0; i < sizeof(line_ptr->channel_B_space) / sizeof(line_ptr->channel_B_space[0]); i++)
        {
            int number = rand() % 256;
            printf("%d\n", number);
            line_ptr->channel_B_space[i] = number;
        }
    }
}
void pack_image(struct Line *line_ptr, char channel)
{
    srand(time(NULL));
    if ((int)channel == 'A')
    {
        for (int i = 0; i < sizeof(line_ptr->channel_B_image) / sizeof(line_ptr->channel_A_image[0]); i++)
        {
            int number = rand() % 256;
            printf("%d\n", number);
            line_ptr->channel_A_image[i] = number;
        }
    }
    else
    {
        for (int i = 0; i < sizeof(line_ptr->channel_B_image) / sizeof(line_ptr->channel_B_image[0]); i++)
        {
            int number = rand() % 256;
            printf("%d\n", number);
            line_ptr->channel_B_image[i] = number;
        }
    }
}
void pack_telemetry(struct Line *line_ptr, char channel)
{
    srand(time(NULL));
    if ((int)channel == 'A')
    {
        for (int i = 0; i < sizeof(line_ptr->channel_A_telemetry) / sizeof(line_ptr->channel_A_telemetry[0]); i++)
        {
            int number = rand() % 256;
            printf("%d\n", number);
            line_ptr->channel_A_telemetry[i] = number;
        }
    }
    else
    {
        for (int i = 0; i < sizeof(line_ptr->channel_B_telemetry) / sizeof(line_ptr->channel_B_telemetry[0]); i++)
        {
            int number = rand() % 256;
            printf("%d\n", number);
            line_ptr->channel_B_telemetry[i] = number;
        }
    }
}

void fill_frame()
{

    for (int i = 0; i < 128; i++)
    {
        struct Line line;
        struct Line *line_ptr = &line;
        pack_sync(line_ptr, 'A');
        pack_space(line_ptr, 'A');
        pack_image(line_ptr, 'A');
        pack_telemetry(line_ptr, 'A');

        pack_sync(line_ptr, 'B');
        pack_space(line_ptr, 'B');
        pack_image(line_ptr, 'B');
        pack_telemetry(line_ptr, 'B');

        frame[i] = line;
    }
}

void print_frame()
{
    printf("line | time | data\n");
    for (int i = 0; i < 128; i++)
    {
        struct Line line = frame[i];
        printf("%d%8d%7d\n", i, line.time, line.data);
        printf("===================\n");
    }
}