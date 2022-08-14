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

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../libs/eight_algorithims.h"
#include "../libs/eight_files.h"

int main(int argc, char *argv[]) {
  int i = 0;
  int k = 0;
  int l = 0;
  int counter = 0;
  int real_lines = 0;
  int *magic = NULL;
  int files = 0;

  if (argc < 2) {
    printf("Error: No input file specified\n");
    exit(1);
  } else if (argc > (MAX_FILES + 1)) {
    printf("Error: Too many arguments specified. Expected %d\n", MAX_FILES);
    exit(1);
  }

  for (i = 1; i < argc && i < MAX_FILES; ++i) {
    if (access(argv[i], R_OK) == 0) {
      printf("This file is all good: %s\n", argv[i]);
      files++;
      counter = count_lines_in_file(argv[i], &real_lines);
      printf("Number of lines returned is: %d\n", counter);
      printf("Number of real lines returned is: %d\n", real_lines);
      magic = read_file_to_array(argv[i], real_lines, counter);

      for (k = 0; k < real_lines; ++k)
        printf("Magic array contains: %d\n", magic[k]);

      qsort(magic, real_lines, sizeof(int), qsort_compare);

      for (l = 0; l < real_lines; ++l)
        printf("Sorted array contains: %d\n", magic[l]);

      counter = 0;
      real_lines = 0;
      free(magic);
    } else
      printf("This file didn't exist wasn't writeable or the file name was too "
             "long: %s\n",
             argv[i]);
  }

  if (files == 0) {
    printf("Looks like no valid files");
    exit(1);
  }

  return 0;
}
