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

/* Defines which parsing to use when checking for real lines in a file
   i.e DIGIT = ISDIGIT(). See 'man isdigit'. CUSTOMX will indicate a 
   specific custom parsing outside of the standard IS set of functions. */
typedef enum {
    LINE_MODE_DIGIT,
    LINE_MODE_CUSTOM1
} LineMode;

/* Defines which parsing to use when reading the file into an array */
typedef enum {
    READ_MODE_INT,
    READ_MODE_FILESTORE
} ReadMode;

/* Function definitions */

int count_lines_in_file(const char *file_path, int *real_lines, LineMode mode); 
int read_file_to_array(const char *file_path, int array_size, 
    void *out_array, int *out_count, ReadMode mode);                          

#endif
