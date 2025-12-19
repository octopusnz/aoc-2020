/* =========================================================================
    Advent of Code 2020 Solutions
    https://github.com/octopusnz/aoc-2020
    Copyright (c) 2025 Jacob Doherty
    SPDX-License-Identifier: MIT
========================================================================= */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

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
    int letter_total_valid = 0;           /* per file */
    int position_total_valid = 0;         /* per file */
    int letter_total_valid_all = 0;       /* aggregated across files */
    int position_total_valid_all = 0;     /* aggregated across files */
    int counter = 0;                      /* number of parsed lines placed into array */
    int real_lines = 0;                   /* number of valid lines expected */
    FileStore *magic = NULL;
    int files = 0;                        /* successfully processed files */

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
        const char *filename = argv[i];
        if (access(filename, R_OK) == 0)
        {
            printf("Processing file: %s\n", filename);
            real_lines = 0;
            letter_total_valid = 0;
            position_total_valid = 0;
            counter = count_lines_in_file(filename, &real_lines, LINE_MODE_CUSTOM1);
            if (counter == -1)
            {
                fprintf(stderr, "Error counting lines in file: %s\n", filename);
                continue;
            }

            if (real_lines <= 0)
            {
                printf("File %s is empty or has no valid lines, skipping.\n", filename);
                continue;
            }

            if ((size_t)real_lines > (((size_t)-1) / sizeof(FileStore)))
            {
                fprintf(stderr, "Refusing to allocate %d elements (size overflow risk) for %s\n", real_lines, filename);
                continue;
            }

            magic = calloc((size_t)real_lines, sizeof(FileStore));
            if (!magic)
            {
                fprintf(stderr, "Memory allocation failed for magic array (%d entries)\n", real_lines);
                continue;
            }
            counter = 0;
            if (read_file_to_array(filename, real_lines, magic, &counter, READ_MODE_FILESTORE) != 0)
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

            printf("Total valid strings (letter count): %d\n", letter_total_valid);
            printf("Total valid strings (position rule): %d\n", position_total_valid);

            /* Aggregate */
            letter_total_valid_all += letter_total_valid;
            position_total_valid_all += position_total_valid;

            /* Free on successful path to avoid leaks */
            free(magic);
            magic = NULL;

            /* Count only successfully processed files (optional) */
            files++;
        }
        else
        {
            fprintf(stderr, "Cannot read file: %s\n", filename);
        }
    }

    if (files == 0)
    {
        fprintf(stderr, "No valid files were processed.\n");
        return 1;
    }

    if (files > 1)
    {
        printf("Aggregated valid strings across %d files (letter count): %d\n", files, letter_total_valid_all);
        printf("Aggregated valid strings across %d files (position rule): %d\n", files, position_total_valid_all);
    }

    return 0;
}
