/* =========================================================================
    Advent of Code 2020 Solutions
    https://github.com/octopusnz/aoc-2020
    Copyright (c) 2025 Jacob Doherty
    SPDX-License-Identifier: MIT
========================================================================= */

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

static const int rights[5] = {1, 3, 5, 7, 1};
static const int downs[5] = {1, 1, 1, 1, 2};

static unsigned long count_trees(const DotHashGrid *grid, int right, int down)
{
    size_t row;
    size_t col;
    unsigned long trees;

    if (!grid || !grid->data || grid->rows <= 0 || grid->cols <= 0)
        return 0;
    if (right <= 0 || down <= 0)
        return 0;

    trees = 0;
    col = (size_t)right;
    for (row = (size_t)down; row < (size_t)grid->rows; row += (size_t)down)
    {
        if (dothash_get(grid, (int)row, (int)col, 1) == '#')
            trees++;
        col += (size_t)right;
    }
    return trees;
}
int main(int argc, char *argv[])
{
    int i = 0;
    DotHashGrid grid = {0, 0, NULL};
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
            int s;
            unsigned long product;

            printf("Processing file: %s\n", filename);

            /* Ensure no leak if grid was left populated (defensive) */
            free_dot_hash_grid(&grid);
            if (read_dot_hash_grid(filename, &grid) != 0)
            {
                fprintf(stderr, "Error reading file into tight grid\n");
                continue;
            }
            printf("Grid loaded: %d rows x %d cols\n", grid.rows, grid.cols);

            if (grid.rows <= 0 || grid.cols <= 0 || !grid.data)
            {
                printf("Empty/invalid grid, skipping.\n");
                free_dot_hash_grid(&grid);
                continue;
            }

            product = 1;
            for (s = 0; s < 5; ++s)
            {
                unsigned long trees = count_trees(&grid, rights[s], downs[s]);
                printf("Slope right %d, down %d: %lu trees\n", rights[s], downs[s], trees);
                product *= trees;
            }
            printf("Product of trees: %lu\n", product);


            /* Free on successful path to avoid leaks */
            free_dot_hash_grid(&grid);

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


    return 0;
}
