/* ==========================================================================
    OctoLibC - A small C library for common tasks
    Version: 0.0.1
    https://github.com/octopusnz/octolibc
    Copyright (c) 2025 Jacob Doherty
    SPDX-License-Identifier: MIT
    File: eight_files.h
    Description: Contains useful utilities for reading files and parsing data
============================================================================= */

#ifndef EIGHT_FILES_H
#define EIGHT_FILES_H

/* File limits to respect */
#define MAX_CHARS 1000000
#define MAX_FILES 3
#define MAX_FILE_NAME_CHARS 20
#define MAX_LINES 10000
#define MAX_LINE_LENGTH 100
#define MAX_INT_VALUE 10000

/* Structs */

typedef struct {
    int min;
    int max;
    char letter;
    char value[MAX_LINE_LENGTH];
} FileStore;

/* Grid of '.' and '#' tightly sized to rows x cols */
typedef struct {
    int rows;
    int cols;
    /* Data stored in row-major order, length rows*cols */
    char *data;
} DotHashGrid;

/* Defines which parsing to use when checking for real lines in a file
   i.e DIGIT = ISDIGIT(). See 'man isdigit'. CUSTOMX will indicate a 
   specific custom parsing outside of the standard IS set of functions. */
typedef enum {
    LINE_MODE_DIGIT,
    LINE_MODE_CUSTOM1,
    LINE_MODE_CUSTOM2
} LineMode;

/* Defines which parsing to use when reading the file into an array */
typedef enum {
    READ_MODE_INT,
    READ_MODE_FILESTORE,
    READ_MODE_DOTHASH
} ReadMode;

/* Function definitions */

int count_lines_in_file(const char *file_path, int *real_lines, LineMode mode); 

int read_file_to_array(const char *file_path, int array_size, 
    void *out_array, int *out_count, ReadMode mode);                          

/* Read a '.'/'#' grid with tight allocation. Returns 0 on success. */
int read_dot_hash_grid(const char *file_path, DotHashGrid *out_grid);
void free_dot_hash_grid(DotHashGrid *grid);

/* Get a cell from the grid. If wrap_horiz is non-zero, column wraps around cols. */
char dothash_get(const DotHashGrid *grid, int row, int col, int wrap_horiz);
/* Set a cell in the grid to '.' or '#'. If wrap_horiz is non-zero, column wraps around cols. Returns 0 on success, -1 on error. */
int dothash_set(DotHashGrid *grid, int row, int col, char value, int wrap_horiz);

#endif
