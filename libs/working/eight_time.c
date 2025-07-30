#include <time.h>
#include "../libs/eight_time.h"

double clock_time(FuncType func, void *args) {
    clock_t start, end;
    start = clock();

    func(args);

    end = clock();
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

double mono_time(FuncType func, void *args)
{

    double elapsed
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    func(args);

    clock_gettime(CLOCK_MONOTONIC, &end);

elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
return elapsed;
}
