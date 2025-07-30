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
#include <time.h>
#include <unistd.h>

#include "../libs/eight_algorithims.h"
#include "../libs/eight_files.h"

int main(int argc, char* argv[])
{
    int i = 0;
    int j = 0;
    int counter = 0;
    int real_lines = 0;
    int* magic = NULL;
    int target_num = 2020;
    int files = 0;
    int large_int = 0;
    Matches result = { 0, 0, 0 };
    Times final_times[3] = { 0 };
    clock_t start = 0;
    clock_t end = 0;
    double qsort_time_taken = 0;
    double bubble_time_taken = 0;
    double pair_time_taken = 0;
    double hash_time_taken = 0;
    int* unsorted = NULL;
    int time_size = 0;

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

            printf("Finding numbers using a HashTable...\n");
            start = clock();
            result = find_pair(magic, real_lines, large_int, target_num);
            end = clock();
            hash_time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;

            if (result.found) {
                printf("Pair found: %d and %d\n", result.num1, result.num2);
                printf("Final Result: %d\n", result.num1 * result.num2);

            } else {
                printf("No pair found.\n");
            }

            printf("Finding numbers by sorting and then using two pointers...\n");
            unsorted = (int*)calloc(real_lines, sizeof(int));
            memcpy(unsorted, magic, real_lines * sizeof(int));
            start = clock();
            qsort(unsorted, real_lines, sizeof(int), qsort_compare);
            end = clock();
            qsort_time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
            free(unsorted);

            unsorted = (int*)calloc(real_lines, sizeof(int));
            memcpy(unsorted, magic, real_lines * sizeof(int));
            start = clock();
            bubble_sort(unsorted, real_lines);
            end = clock();
            bubble_time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;

            start = clock();
            result = find_pair_sorted(unsorted, real_lines, target_num);
            end = clock();
            pair_time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;

            if (result.found) {
                printf("Pair found: %d and %d\n", result.num1, result.num2);
                printf("Final Result: %d\n", result.num1 * result.num2);

            } else {
                printf("No pair found.\n");
            }

            final_times[0].time = hash_time_taken;
            final_times[1].time = qsort_time_taken + pair_time_taken;
            final_times[2].time = bubble_time_taken + pair_time_taken;
            strcpy(final_times[0].method, "Hashtable:");
            strcpy(final_times[1].method, "Qsort:");
            strcpy(final_times[2].method, "Bubblesort:");

            time_size = sizeof(final_times) / sizeof(final_times[0]);
            qsort(final_times, time_size, sizeof(Times),
              qsort_compare_time_struct);

            printf("============================================\n");
            printf("Time summary based on approach:\n");
            for (j = 0; j < time_size; j++) {
                printf("%d. %s %.4f milliseconds\n", j + 1, final_times[j].method,
                       final_times[j].time * 1000);
            }
            printf("============================================\n");
            counter = 0;
            real_lines = 0;
            free(magic);
            free(unsorted);

        } else
            printf("This file didn't exist or wasn't writeable  %s\n", argv[i]);
    }

    if (files == 0) {
        printf("Looks like no valid files");
        exit(1);
    }

    return 0;
}
