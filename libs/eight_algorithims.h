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

typedef struct {
    char method[20];
    double time;
} Times;


/* Function definitions */

/* Custom Qsort Compare for the Time Struct */

int qsort_compare_time_struct(const void *a, const void *b);

/* Bubblesort

   1. https://en.wikipedia.org/wiki/Bubble_sort */

void bubble_sort(int *array_name, int array_size);

/* Quicksort

   1. https://en.wikipedia.org/wiki/Quicksort
   2. man qsort */

int qsort_compare(const void *a, const void *b);

/* Find Pair - Read integers in an Array to Hash Table. This compares two numbers and see
   if they sum to a third number. */

Matches find_pair(int *array_name, int array_size, int large_int,  int target_num);

/* Find Max - Find the largest integer in an array to dynamically set the size of a HashTable. */

int find_max(int *array_name, int array_size);

/* Find two numbers that sum to a third number, in a sorted array using two pointers */

Matches find_pair_sorted(int *array_name, int array_size, int target_num);

#endif
