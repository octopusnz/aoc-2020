#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../libs/eight_files.h"

int count_lines_in_file(const char *file_path, int *real_lines) {
  FILE *fp = NULL;
  int c = 0;
  int pc = 0;
  int count = 0;
  int char_count = 0;
  int real_chars = 0;
  int real_char_count = 0;
  int total_count = 0;
  int fake_chars = 0;
  int fake_char_count = 0;

  fp = fopen(file_path, "r");

  if (fp == NULL) {
    perror("Error opening file");
    exit(1);
  }

  while ((c = getc(fp)) != EOF && (count < MAX_LINES) &&
         (char_count < MAX_CHARS)) {

    if (isdigit(c)) {
      real_chars++;
      real_char_count++;

    } else if (c != '\n') {
      fake_chars++;
      fake_char_count++;

    } else if ((c == '\n') && (real_chars > 0) && (fake_chars == 0)) {
      count++;
      total_count++;
      fake_char_count++;
      real_chars = 0;
      fake_chars = 0;

    } else {
      total_count++;
      real_chars = 0;
      fake_chars = 0;
      fake_char_count++;
    }
    char_count++;
    pc = c;
  }

  if ((pc != '\n') && (real_chars > 0)) {
    count++;
    total_count++;
  }

  if (count >= MAX_LINES && c != EOF) {
    printf("We may have stopped processing this file part-way\n");
    printf("We processed up to %d lines\n", total_count);
  }

  if (char_count >= MAX_CHARS && c != EOF) {
    printf("We may have stopped processing this file part-way\n");
    printf("We processed up to %d characters\n", char_count);
  }

  if (real_char_count == 0) {
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

int *read_file_to_array(const char *file_path, int array_size,
                        int total_lines_to_read) {
  FILE *fp = NULL;
  char *endptr = NULL;
  char parser[MAX_LINE_LENGTH] = {0};
  int i = 0;
  int *data = calloc(array_size, sizeof(int));
  int check_value = 0;
  int valid = 0;

  fp = fopen(file_path, "r");

  if (fp == NULL) {
    perror("Error opening file:");
    exit(1);
  }

  /* See 'man strtol'

     Since  strtol() can legitimately return 0, LONG_MAX, or LONG_MIN
     (LLONG_MAX or LLONG_MIN for strtoll()) on both the success and failure,
     the calling program should set errno to 0 before the call,
     and then determine  if  an error occurred by checking whether errno
     has a nonzero value after the call */

  for (i = 0; (i < total_lines_to_read) && (i < MAX_LINES); i++) {
    if (fgets(parser, sizeof(parser), fp) != NULL) {
      parser[strcspn(parser, "\n")] = 0;
      errno = 0;
      check_value = (int)strtol(parser, &endptr, 10);

      if (errno != 0) {
        perror("strtol");
        exit(EXIT_FAILURE);
      }
    }

    if (endptr == parser)
      printf("Not a valid integer\n");
    else if (*endptr != '\0')
      printf("Valid number, but followed by non-numeric data\n");
    else {
      data[valid] = check_value;
      valid++;
    }
  }

  fclose(fp);
  return data;
}
