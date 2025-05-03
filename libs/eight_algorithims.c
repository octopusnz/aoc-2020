#include <stdio.h>
#include <stdlib.h>
#include "../libs/eight_algorithims.h"

int qsort_compare_time_struct(const void *a, const void *b) {

    const Times *timeA = (const Times*)a;
    const Times *timeB = (const Times*)b;

    if (timeA->time < timeB->time) return -1;
    else if (timeA->time > timeB->time) return 1;
    else return 0;
}

void bubble_sort(int *array_name, int array_size) {
  int swapped = 0;
  int i = 0;
  int j = 0;
  int temp = 0;

  for (i = 0; i < array_size - 1; i++) {
    swapped = 0;

    for (j = 0; j < array_size - i - 1; j++) {
      if (array_name[j] > array_name[j + 1]) {
        temp = array_name[j];
        array_name[j] = array_name[j + 1];
        array_name[j + 1] = temp;

        swapped = 1;
      }
    }
    if (swapped == 0) break;
  }
}

Matches find_pair_sorted(int *array_name, int array_size, int target_num) {

  int left = 0;
  int right = (array_size - 1);
  Matches result = {0, 0, 0};

  while (left < right) {
      int sum = array_name[left] + array_name[right];

      if (sum == target_num) {
          result.num1 = array_name[left];
          result.num2 = array_name[right];
          result.found = 1;
          return result;

      } else if (sum < target_num) {
          left++;
      } else {
          right--;
      }
  }
  return result;
}

int find_max(int *array_name, int array_size) {

  int i = 0;
  int max_value = array_name[0];
  for (i = 1; i < array_size; i++) {
    if (array_name[i] > max_value) {
      max_value = array_name[i];
    }
  }
  return max_value;
}

int qsort_compare(const void *a, const void *b) {
  int int_a = *((int *)a);
  int int_b = *((int *)b);

  if (int_a == int_b)
    return 0;
  else if (int_a < int_b)
    return -1;
  else
    return 1;
}

Matches find_pair (int *array_name, int array_size, int large_int, int target_num) {

  int *hashTable = (int *)calloc(large_int + 1, sizeof(int));
  int i = 0;
  Matches result = {0, 0, 0};

   if (!hashTable) {
        printf("Memory allocation failed!\n");
        return result;
    }

  for (i = 0; i < array_size; i++) {
      int complement = target_num - array_name[i];
    if (hashTable[complement]) {
        result.num1 = array_name[i];
        result.num2 = complement;
        result.found = 1;
        free(hashTable);
        return result;
    }

    if (array_name[i] >= 0 && array_name[i] < large_int + 1) {
    hashTable[array_name[i]] = 1;
    } else {
    printf("Out-of-bounds access detected at index %d\n", array_name[i]);
    free (hashTable);
    return result;
    }
  }
  printf("No pair found.\n");
  free (hashTable);
  return result;
}
