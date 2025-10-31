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
    int dot_count = 0;
    int hash_count = 0;

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
            /* Format: the character is either a '#' or a '.' */
            case LINE_MODE_CUSTOM2:
                is_real = (c == '#' || c == '.');
                break;
            default:
                is_real = 0;
        }

        if (is_real)
        {
            if (mode == LINE_MODE_CUSTOM2)
            {
                if (c == '#')
                    hash_count++;
                else if (c == '.')
                    dot_count++;
            }
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
    if (mode == LINE_MODE_CUSTOM2)
    {
        printf("Dot Count '.' is: %d\n", dot_count);
        printf("Hash Count '#' is: %d\n", hash_count);
    }

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
    else if (mode == READ_MODE_DOTHASH)
    {
        /* Interpret out_array as a 2D char array: char rows[array_size][MAX_LINE_LENGTH] */
        char (*rows)[MAX_LINE_LENGTH] = (char (*)[MAX_LINE_LENGTH])out_array;
        int expected_cols = -1;
        while (fgets(line, sizeof(line), fp) && count < array_size)
        {
            size_t len;
            size_t k;
            int valid;

            /* Trim newline */
            line[strcspn(line, "\n")] = 0;

            /* Skip empty lines silently */
            if (line[0] == '\0')
                continue;

            /* Validate characters and measure length */
            len = strlen(line);
            valid = 1;
            for (k = 0; k < len; ++k)
            {
                if (!(line[k] == '.' || line[k] == '#'))
                {
                    valid = 0;
                    break;
                }
            }
            if (!valid)
            {
                fprintf(stderr, "Malformed map line (non ./# char): %s\n", line);
                continue;
            }

            if (len >= (size_t)MAX_LINE_LENGTH)
            {
                fprintf(stderr, "Line too long (%lu >= %d), truncating: %s\n", (unsigned long)len, MAX_LINE_LENGTH, line);
                len = MAX_LINE_LENGTH - 1;
                line[len] = '\0';
            }

            if (expected_cols < 0)
                expected_cols = (int)len;
            else if ((int)len != expected_cols)
            {
                fprintf(stderr, "Warning: inconsistent row length (expected %d, got %lu)\n", expected_cols, (unsigned long)len);
            }

            /* Copy into the 2D array row */
            strncpy(rows[count], line, MAX_LINE_LENGTH);
            rows[count][MAX_LINE_LENGTH - 1] = '\0';
            count++;
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

int read_dot_hash_grid(const char *file_path, DotHashGrid *out_grid)
{
    FILE *fp = NULL;
    char line[MAX_LINE_LENGTH];
    int rows = 0;
    int cols = -1;
    int r;

    if (!out_grid)
        return -1;
    out_grid->rows = 0;
    out_grid->cols = 0;
    out_grid->data = NULL;

    /* First pass: determine rows and columns */
    fp = fopen(file_path, "r");
    if (!fp)
    {
        perror("Error opening file");
        return -1;
    }
    while (fgets(line, sizeof(line), fp))
    {
        size_t len;
        size_t k;
        int valid = 1;
        line[strcspn(line, "\n")] = 0;
        if (line[0] == '\0')
            continue;
        len = strlen(line);
        for (k = 0; k < len; ++k)
        {
            if (!(line[k] == '.' || line[k] == '#'))
            {
                valid = 0;
                break;
            }
        }
        if (!valid)
            continue;
        if (len >= (size_t)MAX_LINE_LENGTH)
            len = MAX_LINE_LENGTH - 1;
        if (cols < 0)
            cols = (int)len;
        if ((int)len > cols)
            cols = (int)len;
        rows++;
        if (rows >= MAX_LINES)
            break;
    }
    fclose(fp);

    if (rows == 0 || cols <= 0)
        return 0; /* empty grid */

    /* Allocate tight buffer */
    out_grid->rows = rows;
    out_grid->cols = cols;
    out_grid->data = (char *)calloc((size_t)rows * (size_t)cols, sizeof(char));
    if (!out_grid->data)
    {
        fprintf(stderr, "Memory allocation failed for DotHashGrid (%d x %d)\n", rows, cols);
        out_grid->rows = out_grid->cols = 0;
        return -1;
    }

    /* Second pass: fill buffer, right-pad shorter lines with '\0' (treat as '.') */
    fp = fopen(file_path, "r");
    if (!fp)
    {
        perror("Error opening file");
        free(out_grid->data);
        out_grid->data = NULL;
        out_grid->rows = out_grid->cols = 0;
        return -1;
    }
    r = 0;
    while (fgets(line, sizeof(line), fp) && r < rows)
    {
        size_t len;
        size_t c;
        line[strcspn(line, "\n")] = 0;
        if (line[0] == '\0')
            continue;
        len = strlen(line);
        if (len >= (size_t)cols)
            len = (size_t)cols;
        for (c = 0; c < len; ++c)
        {
            char ch = line[c];
            out_grid->data[r * cols + (int)c] = (ch == '.' || ch == '#') ? ch : '.';
        }
        for (; c < (size_t)cols; ++c)
        {
            out_grid->data[r * cols + (int)c] = '.';
        }
        r++;
    }
    fclose(fp);

    return 0;
}

void free_dot_hash_grid(DotHashGrid *grid)
{
    if (!grid) return;
    if (grid->data)
        free(grid->data);
    grid->data = NULL;
    grid->rows = 0;
    grid->cols = 0;
}

char dothash_get(const DotHashGrid *grid, int row, int col, int wrap_horiz)
{
    int c;
    if (!grid || !grid->data)
        return '\0';
    if (row < 0 || row >= grid->rows)
        return '\0';
    if (grid->cols <= 0)
        return '\0';
    if (wrap_horiz)
    {
        int m = grid->cols;
        /* Safe modulo for negative values */
        c = col % m;
        if (c < 0) c += m;
    }
    else
    {
        if (col < 0 || col >= grid->cols)
            return '\0';
        c = col;
    }
    return grid->data[row * grid->cols + c];
}

int dothash_set(DotHashGrid *grid, int row, int col, char value, int wrap_horiz)
{
    int c;
    if (!grid || !grid->data)
        return -1;
    if (value != '.' && value != '#')
        return -1;
    if (row < 0 || row >= grid->rows)
        return -1;
    if (grid->cols <= 0)
        return -1;
    if (wrap_horiz)
    {
        int m = grid->cols;
        c = col % m;
        if (c < 0) c += m;
    }
    else
    {
        if (col < 0 || col >= grid->cols)
            return -1;
        c = col;
    }
    grid->data[row * grid->cols + c] = value;
    return 0;
}

