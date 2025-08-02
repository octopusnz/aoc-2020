/*
Filename: eight_files.h
Child files: eight_files.c

Contains useful utilities for working with files and validating the contents */

#ifndef EIGHT_FILES_H
#define EIGHT_FILES_H

/* File limits to respect */

#define MAX_CHARS 1000000
#define MAX_FILES 3
#define MAX_FILE_NAME_CHARS 20
#define MAX_LINES 1000
#define MAX_LINE_LENGTH 100
#define MAX_INT_VALUE 10000

/* Structs */

typedef struct {
    int min;
    int max;
    char value[MAX_LINE_LENGTH];
    char letter;
} FileStore;

typedef enum {
    LINE_MODE_DIGIT,
    LINE_MODE_ALNUM
} LineMode;

/* Function definitions */


int *read_file_to_array(const char *file_path, int array_size,
                        int total_lines_to_read);
FileStore *read_file_to_array_alnum(const char *file_path, int array_size,
                        int *counter);       
int count_lines_in_file(const char *file_path, int *real_lines, LineMode mode);                             

#endif
