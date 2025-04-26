#include <stdio.h>
#include <stdlib.h>
#include "../libs/eight_algorithims.h"

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
      printf("Checking: %d\n", array_name[i]);
    if (hashTable[complement]) {
        printf("Pair found: %d and %d\n", array_name[i], complement);
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
