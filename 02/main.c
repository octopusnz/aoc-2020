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
SOFTWARE. */

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <io.h>
#define access _access
#ifndef R_OK
#define R_OK 4
#endif
#else
#include <unistd.h>
#endif

#include "../libs/eight_algorithms.h"
#include "../libs/eight_files.h"

int main(int argc, char *argv[])
{
    int i = 0;
    int j = 0;
    int letter_total_valid = 0;
    int position_total_valid = 0;
    int counter = 0;
    int real_lines = 0;
    FileStore *magic = NULL;
    int files = 0;

    if (argc < 2)
    {
        fprintf(stderr, "Error: No input file specified\n");
        exit(1);
    }
    else if (argc > (MAX_FILES + 1))
    {
        fprintf(stderr, "Error: Too many arguments specified. Expected %d\n", MAX_FILES);
        exit(1);
    }

    for (i = 1; i < argc && files < MAX_FILES; ++i)
    {
        if (access(argv[i], R_OK) == 0)
        {
            printf("Processing file: %s\n", argv[i]);
            real_lines = 0;
            letter_total_valid = 0;
            position_total_valid = 0;
            counter = count_lines_in_file(argv[i], &real_lines, LINE_MODE_CUSTOM1);
            if (counter == -1)
            {
                fprintf(stderr, "Error counting lines in file: %s\n", argv[i]);
                continue;
            }

            if (real_lines <= 0)
            {
                printf("File %s is empty or has no valid lines, skipping.\n", argv[i]);
                continue;
            }

            magic = calloc(real_lines, sizeof(FileStore));
            if (!magic)
            {
                fprintf(stderr, "Memory allocation failed for magic array\n");
                exit(1);
            }
            counter = 0;
            if (read_file_to_array(argv[i], real_lines, magic, &counter, READ_MODE_FILESTORE) != 0)
            {
                fprintf(stderr, "Error reading file into FileStore array\n");
                free(magic);
                magic = NULL;
                /* Skip this file but continue processing others */
                continue;
            }

            for (j = 0; j < counter; j++)
            {
                if (is_letter_count_valid(magic[j]))
                {
                    letter_total_valid++;
                }
                if (is_position_valid(magic[j]))
                {
                    position_total_valid++;
                }
            }

            printf("Total valid passwords (letter count): %d\n", letter_total_valid);
            printf("Total valid passwords (position rule): %d\n", position_total_valid);

            /* Free on successful path to avoid leaks */
            free(magic);
            magic = NULL;

            /* Count only successfully processed files (optional) */
            files++;
        }
        else
        {
            fprintf(stderr, "Cannot read file: %s\n", argv[i]);
        }
    }

    if (files == 0)
    {
        fprintf(stderr, "No valid files were processed.\n");
        return 1;
    }

    return 0;
}
