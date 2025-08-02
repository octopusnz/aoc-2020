#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libs/eight_algorithms.h"
#include "../libs/eight_files.h"

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
    int sum = 0;
    ManyMatches result = {0, 0, 0, 0};

    for (i = 0; i < array_size - 2; i++)
    {
        left = i + 1;
        right = array_size - 1;

        while (left < right)
        {
            sum = array_name[i] + array_name[left] + array_name[right];
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
    Matches result = {0, 0, 0};

    while (left < right)
    {
        int sum = array_name[left] + array_name[right];

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
    if (array_size <= 0) return 0;
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
    int *hashTable = (int *)calloc(large_int + 1, sizeof(int));
    int i = 0;
    int j = 0;
    int current_target = 0;
    int complement = 0;
    ManyMatches result = {0, 0, 0, 0};

    if (!hashTable)
    {
        printf("Memory allocation failed!\n");
        return result;
    }

    for (i = 0; i < array_size - 2; i++)
    {
        memset(hashTable, 0, (large_int + 1) * sizeof(int));
        current_target = target_num - array_name[i];

        for (j = i + 1; j < array_size; j++)
        {
            complement = current_target - array_name[j];

            if (complement >= 0 && complement < large_int + 1 && hashTable[complement])
            {
                result.num1 = complement;
                result.num2 = array_name[i];
                result.num3 = array_name[j];
                result.found = 1;
                free(hashTable);
                return result;
            }

            if (array_name[j] >= 0 && array_name[j] < large_int + 1)
            {
                hashTable[array_name[j]] = 1;
            }
            else
            {
                printf("Out-of-bounds value %d at index %d\n", array_name[j], j);
                free(hashTable);
                return result;
            }
        }
    }

    printf("No triplet found");
    free(hashTable);
    return result;
}

Matches find_pair(int *array_name, int array_size, int large_int, int target_num)
{
    int *hashTable = (int *)calloc(large_int + 1, sizeof(int));
    int i = 0;
    int complement = 0;
    Matches result = {0, 0, 0};

    if (!hashTable)
    {
        printf("Memory allocation failed!\n");
        return result;
    }

    for (i = 0; i < array_size; i++)
    {
        complement = target_num - array_name[i];
        if (complement >= 0 && complement < large_int + 1 && hashTable[complement])
        {
            result.num1 = array_name[i];
            result.num2 = complement;
            result.found = 1;
            free(hashTable);
            return result;
        }

        if (array_name[i] >= 0 && array_name[i] < large_int + 1)
        {
            hashTable[array_name[i]] = 1;
        }
        else
        {
            printf("Out-of-bounds access detected at index %d\n", array_name[i]);
            free(hashTable);
            return result;
        }
    }
    printf("No pair found.\n");
    free(hashTable);
    return result;
}
