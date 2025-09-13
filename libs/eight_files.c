/* ==========================================================================
    OctoLibC - A small C library for common tasks
    Version: 0.0.1
    https://github.com/octopusnz/octolibc
    Copyright (c) 2025 Jacob Doherty
    SPDX-License-Identifier: MIT
    File: eight_files.c
    Description: Contains useful utilities for reading files and parsing data
============================================================================= */


#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../libs/eight_files.h"

int count_lines_in_file(const char *file_path, int *real_lines, LineMode mode)
{
    FILE *fp = NULL;
    int c = 0;
    int pc = 0;
    int count = 0;
    int char_count = 0;
    int real_chars = 0;
    int real_char_count = 0;
    int total_count = 0;
    int fake_chars = 0;

    fp = fopen(file_path, "r");

    if (fp == NULL)
    {
        perror("Error opening file");
        return -1;
    }

    while ((c = getc(fp)) != EOF && (count < MAX_LINES) && (char_count < MAX_CHARS))
    {
        int is_real = 0;
        switch (mode) {
            /* Format: 1-3 a: abcde */
            case LINE_MODE_CUSTOM1:
                is_real = (isalnum(c) || c == ':' || c == '-' || c == ' ');
                break;
            /* Format: the character is a digit. See 'man isdigit' */
            case LINE_MODE_DIGIT:
                is_real = isdigit(c);
                break;
            default:
                is_real = 0;
        }

        if (is_real)
        {
            real_chars++;
            real_char_count++;
        }
        else if (c != '\n')
        {
            fake_chars++;
        }
        else if ((c == '\n') && (real_chars > 0) && (fake_chars == 0))
        {
            count++;
            total_count++;
            real_chars = 0;
            fake_chars = 0;
        }
        else
        {
            total_count++;
            real_chars = 0;
            fake_chars = 0;
        }
        char_count++;
        pc = c;
    }

    if ((pc != '\n') && (real_chars > 0))
    {
        count++;
        total_count++;
    }

    if (count >= MAX_LINES && c != EOF)
    {
        printf("We may have stopped processing this file part-way\n");
        printf("We processed up to %d lines\n", total_count);
    }

    if (char_count >= MAX_CHARS && c != EOF)
    {
        printf("We may have stopped processing this file part-way\n");
        printf("We processed up to %d characters\n", char_count);
    }

    if (real_char_count == 0)
    {
        printf("It looks like this was an empty file\n");
        total_count = 0;
    }

    printf("Total Line Count is: %d\n", total_count);
    printf("Real Line Count is: %d\n", count);
    printf("Total Char Count is: %d\n", char_count);
    printf("Real Char Count is: %d\n", real_char_count);

    fclose(fp);

    *real_lines = count;

    return total_count;
}

int read_file_to_array(const char *file_path, int array_size, void *out_array, 
    int *out_count, ReadMode mode)
{
    FILE *fp = NULL;
    int count = 0;
    char line[MAX_LINE_LENGTH];

    fp = fopen(file_path, "r");
    if (fp == NULL)
    {
        perror("Error opening file:");
        return -1;
    }

    if (mode == READ_MODE_FILESTORE)
    {
        FileStore *entries = (FileStore *)out_array;
        while (fgets(line, sizeof(line), fp) && count < array_size)
        {
            int min, max;
            char letter;
            char value[MAX_LINE_LENGTH];
            /* TO-DO: Replace sscanf? Buffer width is hard-coded.
                Should be (MAX_LINE_LENGTH -1) */
            if (sscanf(line, "%d-%d %c: %99s", &min, &max, &letter, value) == 4)
            {
                entries[count].min = min;
                entries[count].max = max;
                entries[count].letter = letter;
                strncpy(entries[count].value, value, MAX_LINE_LENGTH);
                count++;
            }
            else
            {
                fprintf(stderr, "Malformed line: %s", line);
            }
        }
    }
    else if (mode == READ_MODE_INT)
    {
        int *data = (int *)out_array;
        char *endptr = NULL;
        int check_value = 0;
        while (fgets(line, sizeof(line), fp) && count < array_size)
        {
            line[strcspn(line, "\n")] = 0;
            errno = 0;
            check_value = (int)strtol(line, &endptr, 10);
            /* Treat no-conversion and out-of-range as invalid lines, not fatal errors */
            if (endptr == line)
            {
                printf("Not a valid integer\n");
            }
            else if (errno == ERANGE)
            {
                fprintf(stderr, "Integer out of range: %s\n", line);
            }
            else if (*endptr != '\0')
            {
                printf("Valid number, but followed by non-numeric data\n");
            }
            else
            {
                data[count] = check_value;
                count++;
            }
        }
    }
    else
    {
        fprintf(stderr, "Unknown read mode\n");
        fclose(fp);
        return -1;
    }

    fclose(fp);
    if (out_count)
        *out_count = count;
    return 0;
}

