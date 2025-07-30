wrap_find_pair(magic, real_lines, large_int, target_num);
wrap_qsort(unsorted, real_lines, sizeof(int), qsort_compare);
wrap_bubble_sort(unsorted, real_lines);
wrap_find_pair_sorted(unsorted, real_lines, target_num);

void wrap_find_pair_sorted(void *args) {
    FunctionArgs *data = (FunctionArgs *)args;
    find_pair_sorted(data->unsorted, data->real_lines, data->target_num);
}
