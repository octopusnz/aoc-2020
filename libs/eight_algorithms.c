/* ==========================================================================
    OctoLibC - A small C library for common tasks
    https://github.com/octopusnz/octolibc
    Copyright (c) 2025 Jacob Doherty
    SPDX-License-Identifier: MIT
    File: eight_algorithms.c
    Description: Contains useful utilities for working with datasets and sorting
============================================================================= */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "../libs/eight_algorithms.h"
#include "../libs/eight_files.h"

static size_t eight_strnlen(const char *s, size_t maxlen)
{
    size_t n = 0;
    if (!s)
        return 0;
    while (n < maxlen && s[n] != '\0')
        n++;
    return n;
}

int is_position_valid(FileStore entry)
{
    int pos1 = entry.min - 1;
    int pos2 = entry.max - 1;
    int match1 = 0;
    int match2 = 0;
    size_t len = eight_strnlen(entry.value, MAX_LINE_LENGTH);

    if (pos1 >= 0 && (size_t)pos1 < len && entry.value[pos1] == entry.letter)
    {
        match1 = 1;
    }

    if (pos2 >= 0 && (size_t)pos2 < len && entry.value[pos2] == entry.letter)
    {
        match2 = 1;
    }

    return (match1 ^ match2); /* exclusive OR: valid if exactly one matches */
}

int qsort_compare_time_struct(const void *a, const void *b)
{
    const Times *timeA = (const Times *)a;
    const Times *timeB = (const Times *)b;

    if (timeA->time < timeB->time)
        return -1;
    else if (timeA->time > timeB->time)
        return 1;
    else
        return 0;
}

int is_letter_count_valid(FileStore entry)
{
    int count = 0;
    int i = 0;

    for (i = 0; entry.value[i] != '\0'; i++)
    {
        if (entry.value[i] == entry.letter)
        {
            count++;
        }
    }
    return (count >= entry.min && count <= entry.max);
}

void bubble_sort(int *array_name, int array_size)
{
    int swapped = 0;
    int i = 0;
    int j = 0;
    int temp = 0;

    if (!array_name || array_size <= 1)
        return;

    for (i = 0; i < array_size - 1; i++)
    {
        swapped = 0;
        for (j = 0; j < array_size - i - 1; j++)
        {
            if (array_name[j] > array_name[j + 1])
            {
                temp = array_name[j];
                array_name[j] = array_name[j + 1];
                array_name[j + 1] = temp;
                swapped = 1;
            }
        }
        if (swapped == 0)
            break;
    }
}

ManyMatches find_triple_sorted(int *array_name, int array_size, int target_num)
{
    int i = 0;
    int left = 0;
    int right = 0;
    long sum = 0;
    ManyMatches result = {0, 0, 0, 0};

    if (!array_name || array_size < 3)
        return result;

    for (i = 0; i < array_size - 2; i++)
    {
        left = i + 1;
        right = array_size - 1;

        while (left < right)
        {
            sum = (long)array_name[i] + (long)array_name[left] + (long)array_name[right];
            if (sum == target_num)
            {
                result.num1 = array_name[left];
                result.num2 = array_name[right];
                result.num3 = array_name[i];
                result.found = 1;
                return result;
            }
            else if (sum < target_num)
            {
                left++;
            }
            else
            {
                right--;
            }
        }
    }
    return result;
}

Matches find_pair_sorted(int *array_name, int array_size, int target_num)
{
    int left = 0;
    int right = array_size - 1;
    long sum = 0;
    Matches result = {0, 0, 0};

    if (!array_name || array_size < 2)
        return result;

    while (left < right)
    {
        sum = (long)array_name[left] + (long)array_name[right];

        if (sum == target_num)
        {
            result.num1 = array_name[left];
            result.num2 = array_name[right];
            result.found = 1;
            return result;
        }
        else if (sum < target_num)
        {
            left++;
        }
        else
        {
            right--;
        }
    }
    return result;
}

int find_max(int *array_name, int array_size)
{
    int i = 0;
    int max_value = 0;
    
    if (array_size <= 0)
        return 0;
    max_value = array_name[0];
    
    for (i = 1; i < array_size; i++)
    {
        if (array_name[i] > max_value)
        {
            max_value = array_name[i];
        }
    }
    return max_value;
}

int qsort_compare(const void *a, const void *b)
{
    int int_a = *((int *)a);
    int int_b = *((int *)b);

    if (int_a == int_b)
        return 0;
    else if (int_a < int_b)
        return -1;
    else
        return 1;
}

ManyMatches find_triple(int *array_name, int array_size, int large_int, int target_num)
{
    int *hashTable = NULL;
    size_t table_len = 0;
    int i = 0;
    int j = 0;
    long current_target = 0;
    long complement = 0;
    ManyMatches result = {0, 0, 0, 0};

    if (!array_name || array_size < 3 || large_int < 0)
        return result;

    table_len = (size_t)large_int + 1;
    if (table_len == 0 || table_len > (((size_t)-1) / sizeof(int)))
    {
        printf("Refusing to allocate hash table of %lu ints\n", (unsigned long)table_len);
        return result;
    }

    hashTable = (int *)calloc(table_len, sizeof(int));

    if (!hashTable)
    {
        printf("Memory allocation failed!\n");
        return result;
    }

    for (i = 0; i < array_size - 2; i++)
    {
        memset(hashTable, 0, table_len * sizeof(int));
        current_target = (long)target_num - (long)array_name[i];

        for (j = i + 1; j < array_size; j++)
        {
            long v = (long)array_name[j];
            complement = current_target - v;

            if (complement >= 0 && complement < (long)table_len && hashTable[(int)complement])
            {
                result.num1 = (int)complement;
                result.num2 = array_name[i];
                result.num3 = array_name[j];
                result.found = 1;
                free(hashTable);
                return result;
            }

            if (v >= 0 && v < (long)table_len)
            {
                hashTable[(int)v] = 1;
            }
        }
    }

    printf("No triplet found\n");
    free(hashTable);
    return result;
}

Matches find_pair(int *array_name, int array_size, int large_int, int target_num)
{
    int *hashTable = NULL;
    size_t table_len = 0;
    int i = 0;
    long complement = 0;
    Matches result = {0, 0, 0};

    if (!array_name || array_size < 2 || large_int < 0)
        return result;

    table_len = (size_t)large_int + 1;
    if (table_len == 0 || table_len > (((size_t)-1) / sizeof(int)))
    {
        printf("Refusing to allocate hash table of %lu ints\n", (unsigned long)table_len);
        return result;
    }

    hashTable = (int *)calloc(table_len, sizeof(int));

    if (!hashTable)
    {
        printf("Memory allocation failed!\n");
        return result;
    }

    for (i = 0; i < array_size; i++)
    {
        long v = (long)array_name[i];
        complement = (long)target_num - v;
        if (complement >= 0 && complement < (long)table_len && hashTable[(int)complement])
        {
            result.num1 = array_name[i];
            result.num2 = (int)complement;
            result.found = 1;
            free(hashTable);
            return result;
        }

        if (v >= 0 && v < (long)table_len)
        {
            hashTable[(int)v] = 1;
        }
    }
    printf("No pair found.\n");
    free(hashTable);
    return result;
}
