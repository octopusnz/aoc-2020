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
#include <unistd.h>

#include "../libs/eight_algorithims.h"
#include "../libs/eight_files.h"

int main(int argc, char *argv[]) {

  int i = 0;
  int counter = 0;
  int real_lines = 0;
  int *magic = NULL;
  int *matches = NULL;
  int target_num = 2020;
  int files = 0;
  int large_int = 0;
  Matches result = {0, 0, 0};

  if (argc < 2) {
    printf("Error: No input file specified\n");
    exit(1);
  } else if (argc > (MAX_FILES + 1)) {
    printf("Error: Too many arguments specified. Expected %d\n", MAX_FILES);
    exit(1);
  }

  for (i = 1; i < argc && i < MAX_FILES; ++i) {
    if (access(argv[i], R_OK) == 0) {
      printf("Processing file: %s\n", argv[i]);
      files++;
      counter = count_lines_in_file(argv[i], &real_lines);
      magic = read_file_to_array(argv[i], real_lines, counter);
      large_int = find_max(magic, real_lines);
      result = find_pair(magic, real_lines, large_int, target_num);
      if (result.found) {
        printf("Pair found: %d and %d\n", result.num1, result.num2);
      } else {
          printf("No pair found.\n");
      }

      /* qsort(magic, real_lines, sizeof(int), qsort_compare); */

      /* for (k = 0; k < real_lines; ++k) {
        printf("Array contains: %d\n", magic[k]);
        } */
      counter = 0;
      real_lines = 0;
      free(magic);
      free(matches);
    } else
      printf("This file didn't exist or wasn't writeable  %s\n", argv[i]);
  }

  if (files == 0) {
    printf("Looks like no valid files");
    exit(1);
  }

  return 0;
}
