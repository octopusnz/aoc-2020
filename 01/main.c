#include <time.h>
#include <stdio.h>
#include <string.h>

/****************************************************
 Function definitions
*****************************************************/

void brute_force (const char *file_path);

/****************************************************
 Main starts here
*****************************************************/

int main (int argc, char *argv[]) {

  for (int i = 1; i < argc; ++i) {
        brute_force(argv[i]);
  }

  return 0;
}

void brute_force (const char *file_path) {

   printf("Input file: %s\n", file_path);
}
