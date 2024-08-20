#include <stdio.h>
#include <string.h>
#include <linux/limits.h>

void get_path()
{
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("Current working dir: %s\n", cwd);
    }
    else
    {
        perror("getcwd() error");
    }
}

void print_buffer_double(double *buffer, int length)
{
    for (int i = 0; i < length; i++)
    {
        printf("index %d: %f\n", i, buffer[i]);
    }
}

void memory_copy_practice()
{
    int *old_ptr = (int *)malloc(sizeof(int) * 10);
    long int *new_ptr = (long int *)malloc(sizeof(long int) * 10);
    for (int i = 0; i < 10; i++)
    {
        old_ptr[i] = (2 * i) + 1;
    }

    for (int i = 0; i < 10; i++)
    {
        printf("%d: %d size: %ld\n", i, old_ptr[i], sizeof(old_ptr[i]));
    }
    printf("=========\n");
    for (int i = 0; i < 10; i++)
    {
        printf("%d: %d\n", i, new_ptr[i]);
    }
    printf("======\n");

    // memcpy(new_ptr, old_ptr, sizeof(int) * 10);
    for (int i = 0; i < 10; i++)
    {
        new_ptr[i] = (long int)old_ptr[i];
    }

    for (int i = 0; i < 10; i++)
    {
        printf("%d: %d size: %ld\n", i, new_ptr[i], sizeof(new_ptr[i]));
    }
}