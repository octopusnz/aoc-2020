/* Copyright 2021 Jacob Doherty

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

#include <ctype.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// This is the maximum number of files we will simultaneously process
#define MAX_FILES 3
// This is the maximum number of lines in a file we will parse
#define MAX_LINES 1000
//This is the maximum number of characters in a file we will parse
#define MAX_CHARS 1000000

void brute_force (const char *file_path);
int count_lines (const char *file_path);
int *create_array (const char *file_path, int array_size);

int main (int argc, char *argv[])
{
    int i = 0;
    int j = 0;
    int counter = 0;
    int *magic = NULL;

    if (argc < 2)
    {
        printf("Error: No input file specified\n");
        exit(1);
    }
    else if (argc > (MAX_FILES + 1))
    {
        printf("Error: Too many arguments specified. Expected %d\n", MAX_FILES);
        exit(1);
    }

    for (i = 1; i < argc; ++i)
    {
        if (access(argv[i], R_OK) == 0)
        {
            printf("This file is all good: %s\n", argv[i]);
        }
        else
        {
            printf("This file didn't exist or wasn't writeable: %s\n", argv[i]);
        }
    }

    for (j = 1; j < argc; ++j)
    {
        counter = count_lines(argv[j]);
        printf("Number of lines is: %d\n", counter);
        magic = create_array(argv[j], counter);
        free(magic);
    }

    return 0;
}

int count_lines (const char *file_path)
{
    FILE *fp = NULL;
    int c = 0;
    int pc = 0;
    int count = 0;
    int char_count = 0;
    int real_chars = 0;
    int real_char_count = 0;
    int total_count = 0;

    fp = fopen(file_path, "r");

    if (fp == NULL)
    {
        perror("Error opening file: ");
        return 1;
    }

    while ((c = getc(fp)) != EOF && (count < MAX_LINES) && (char_count < MAX_CHARS))
    {
        if (!isspace(c))
        {
            real_chars++;
            real_char_count++;
        }

        else if ((c == '\n') && (real_chars > 0))
        {
            count++;
            total_count++;
            real_chars = 0;
        }

       else if (c == '\n')
       {
           total_count++;
       }

       char_count++;
       pc = c;
    }

    if ((pc != '\n') && (real_char_count > 0))
   {
       count++;
       total_count++;
   }

    if (count >= MAX_LINES)
    {
        printf("We stopped processing this file part-way\n");
        printf("We processed up to %d lines\n", total_count);
    }

    if  (char_count >= MAX_CHARS)
    {
        printf("Max chars reached");
    }


    if (real_char_count == 0)
    {
        printf("It looks like this was an empty file\n");
        count = 0;
    }

    printf("Total Line Count is: %d\n", total_count);
    printf("Real Line Count is: %d\n", count);
    printf("Total Char Count is: %d\n", char_count);
    printf("Real Char Count is: %d\n", real_char_count);
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
        perror("Error opening file: ");
        return 1;
    }

    fclose(fp);

    return data;
}

void brute_force (const char *file_path)
{
    printf("Input file: %s\n", file_path);
}
