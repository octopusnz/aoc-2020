#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void brute_force (const char *file_path);
int count_lines (const char *file_path);
int *create_array (const char *file_path, int array_size);

int main (int argc, char *argv[])
{
    int i = 0;
    int counter = 0;
    int *magic = NULL;

    if (argc < 2)
    {
        printf("Error: No input file specified\n");
        exit(1);
    }

    for (i = 1; i < argc; ++i)
    {
        counter = count_lines(argv[i]);
        printf("Number of lines is: %d\n", counter);
        magic = create_array(argv[i], counter);
        free(magic);
    }

    return 0;
}

int count_lines (const char *file_path)
{
    FILE *fp = NULL;
    int i = 0;
    int count = 0;

    fp = fopen(file_path, "r");

    if (fp == NULL)
    {
        printf("Could not open file.");
        exit(1);
    }

    while ((i = getc(fp)) != EOF)
        if (i == '\n')
            count++;

    printf("Initial Count is: %d\n", count);
    fclose(fp);

    return count;
}

int *create_array (const char *file_path, int array_size)
{
    FILE *fp = NULL;
    int i = 0;
    int *data = malloc(sizeof(int) * array_size);

    fp = fopen(file_path, "r");

    if (fp == NULL)
    {
        printf("Could not open file.");
        exit(1);
    }

    fclose(fp);

    return data;
}

void brute_force (const char *file_path)
{
    printf("Input file: %s\n", file_path);
}
