/*
Filename: eight_files.h
Child files: eight_files.c

Contains useful utilities for working with files and validating the contents */

#ifndef EIGHT_FILES_H
#define EIGHT_FILES_H


/* File limits to respect */

#define MAX_CHARS 1000000
#define MAX_FILES 3
#define MAX_LINES 1000
#define MAX_LINE_LENGTH 100

/* Function definitions */

int count_lines_in_file (const char *filepath, int *real_lines);
int *read_file_to_array (const char *file_path, int array_size, int total_lines_to_read);

#endif
