/*
Filename: eight_algorithims.h
Child files: eight_algorithims.c

Contains useful utilities for working with datasets and sorting */

#ifndef EIGHT_ALGORITHIMS_H
#define EIGHT_ALGORITHIMS_H

/* Structs */

typedef struct {
    int num1;
    int num2;
    int found;

} Matches;

/* Function definitions */

/* Quicksort

   1. https://en.wikipedia.org/wiki/Quicksort
   2. man qsort */

int qsort_compare(const void *a, const void *b);

/* Find Pair - Read integers in an Array to Hash Table. This compares two numbers and see
   if they sum to a third number. */

Matches find_pair(int *array_name, int array_size, int large_int,  int target_num);

/* Find Max - Find the largest integer in an array to dynamically set the size of a HashTable. */

int find_max(int *array_name, int array_size);

#endif
