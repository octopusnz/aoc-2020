/* Unity-based tests for algorithms, file parsing, and validators */
#include "../libs/unity/unity.h"
#include "../libs/eight_algorithms.h"
#include "../libs/eight_files.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

void setUp(void)
{
    /* No-op for now */
}

void tearDown(void)
{
    /* No-op for now */
}

/* --- Algorithms: positive paths --- */
static void test_find_max_returns_largest(void)
{
    int arr[5];
    int n;
    int m;
    arr[0] = 3; arr[1] = 1; arr[2] = 5; arr[3] = 2; arr[4] = 4;
    n = 5;
    m = find_max(arr, n);
    TEST_ASSERT_EQUAL_INT(5, m);
}

static void test_bubble_sort_sorts_ascending(void)
{
    int arr[6];
    int i;
    arr[0] = 5; arr[1] = 3; arr[2] = 6; arr[3] = 1; arr[4] = 2; arr[5] = 4;
    bubble_sort(arr, 6);
    for (i = 1; i < 6; ++i)
    {
        TEST_ASSERT_TRUE(arr[i - 1] <= arr[i]);
    }
}

static void test_qsort_sorts_ascending(void)
{
    int arr[5];
    int i;
    arr[0] = 10; arr[1] = -1; arr[2] = 7; arr[3] = 0; arr[4] = 2;
    qsort(arr, 5, sizeof(int), qsort_compare);
    for (i = 1; i < 5; ++i)
    {
        TEST_ASSERT_TRUE(arr[i - 1] <= arr[i]);
    }
}

static void test_find_pair_sorted_2020_example(void)
{
    /* Classic AoC 2020 Day 1 example */
    int arr[5];
    Matches res;
    int prod;
    arr[0] = 299; arr[1] = 366; arr[2] = 675; arr[3] = 979; arr[4] = 1721;
    qsort(arr, 5, sizeof(int), qsort_compare);
    res = find_pair_sorted(arr, 5, 2020);
    TEST_ASSERT_TRUE(res.found);
    prod = res.num1 * res.num2;
    TEST_ASSERT_EQUAL_INT(514579, prod);
}

static void test_find_pair_hash_2020_example(void)
{
    int arr[6];
    int n;
    int maxv;
    Matches res;
    int prod;
    arr[0] = 1721; arr[1] = 979; arr[2] = 366; arr[3] = 299; arr[4] = 675; arr[5] = 1456;
    n = 6;
    maxv = find_max(arr, n);
    res = find_pair(arr, n, maxv, 2020);
    TEST_ASSERT_TRUE(res.found);
    prod = res.num1 * res.num2;
    TEST_ASSERT_EQUAL_INT(514579, prod);
}

static void test_find_triple_hash_2020_example(void)
{
    int arr[6];
    int n;
    int maxv;
    ManyMatches m;
    long prod;
    arr[0] = 1721; arr[1] = 979; arr[2] = 366; arr[3] = 299; arr[4] = 675; arr[5] = 1456;
    n = 6;
    maxv = find_max(arr, n);
    m = find_triple(arr, n, maxv, 2020);
    TEST_ASSERT_TRUE(m.found);
    prod = (long)m.num1 * (long)m.num2 * (long)m.num3;
    TEST_ASSERT_EQUAL_INT(241861950, (int)prod);
}

static void test_find_triple_sorted_2020_example(void)
{
    int arr[6];
    int n;
    ManyMatches m;
    long prod;
    arr[0] = 1721; arr[1] = 979; arr[2] = 366; arr[3] = 299; arr[4] = 675; arr[5] = 1456;
    n = 6;
    qsort(arr, n, sizeof(int), qsort_compare);
    m = find_triple_sorted(arr, n, 2020);
    TEST_ASSERT_TRUE(m.found);
    prod = (long)m.num1 * (long)m.num2 * (long)m.num3;
    TEST_ASSERT_EQUAL_INT(241861950, (int)prod);
}

/* --- Algorithms: negative paths --- */
static void test_find_pair_sorted_no_solution(void)
{
    int arr[3];
    Matches res;
    arr[0] = 1; arr[1] = 2; arr[2] = 3;
    qsort(arr, 3, sizeof(int), qsort_compare);
    res = find_pair_sorted(arr, 3, 100);
    TEST_ASSERT_FALSE(res.found);
}

static void test_find_pair_hash_no_solution(void)
{
    int arr[4];
    int maxv;
    Matches res;
    arr[0] = 1; arr[1] = 2; arr[2] = 3; arr[3] = 4;
    maxv = find_max(arr, 4);
    res = find_pair(arr, 4, maxv, 9999);
    TEST_ASSERT_FALSE(res.found);
}

static void test_find_triple_no_solution_hash_and_sorted(void)
{
    int arr[5];
    int n;
    int maxv;
    ManyMatches m;
    arr[0] = 1; arr[1] = 2; arr[2] = 3; arr[3] = 4; arr[4] = 5;
    n = 5;
    maxv = find_max(arr, n);
    m = find_triple(arr, n, maxv, 2);
    TEST_ASSERT_FALSE(m.found);
    qsort(arr, n, sizeof(int), qsort_compare);
    m = find_triple_sorted(arr, n, 2);
    TEST_ASSERT_FALSE(m.found);
}

/* --- Files: counting and parsing --- */
static void test_count_lines_in_file_digit_mode(void)
{
    const char *fname = "tmp_count_digits.txt";
    FILE *fp;
    int real_lines;
    int total;
    fp = fopen(fname, "w");
    TEST_ASSERT_NOT_NULL(fp);
    /* Two valid digit lines, one non-digit line, one empty line */
    fputs("123\n", fp);
    fputs("abc\n", fp);
    fputs("456\n", fp);
    fputs("\n", fp);
    fclose(fp);
    real_lines = 0;
    total = count_lines_in_file(fname, &real_lines, LINE_MODE_DIGIT);
    TEST_ASSERT_EQUAL_INT(2, real_lines);
    TEST_ASSERT_TRUE(total >= real_lines);
    remove(fname);
}

static void test_count_lines_in_file_empty_file(void)
{
    const char *fname = "tmp_empty.txt";
    FILE *fp;
    int real_lines;
    int total;
    fp = fopen(fname, "w");
    TEST_ASSERT_NOT_NULL(fp);
    fclose(fp);
    real_lines = -1;
    total = count_lines_in_file(fname, &real_lines, LINE_MODE_DIGIT);
    TEST_ASSERT_EQUAL_INT(0, real_lines);
    TEST_ASSERT_EQUAL_INT(0, total);
    remove(fname);
}

static void test_count_lines_in_file_nonexistent_returns_minus1(void)
{
    int real_lines = 123;
    int total = count_lines_in_file("__no_such_file_digits.txt__", &real_lines, LINE_MODE_DIGIT);
    TEST_ASSERT_EQUAL_INT(-1, total);
}

static void test_read_file_to_array_ints(void)
{
    const char *fname = "tmp_ints.txt";
    FILE *fp;
    int arr[10];
    int out_count;
    int rc;
    int i;
    fp = fopen(fname, "w");
    TEST_ASSERT_NOT_NULL(fp);
    fputs("1\n2\n3\nX\n", fp);
    fclose(fp);
    out_count = 0;
    rc = read_file_to_array(fname, 10, arr, &out_count, READ_MODE_INT);
    TEST_ASSERT_EQUAL_INT(0, rc);
    TEST_ASSERT_EQUAL_INT(3, out_count);
    for (i = 0; i < out_count; ++i)
    {
        TEST_ASSERT_EQUAL_INT(i + 1, arr[i]);
    }
    remove(fname);
}

static void test_read_file_to_array_ints_respects_array_size_limit(void)
{
    const char *fname = "tmp_ints_limit.txt";
    FILE *fp;
    int arr[2];
    int out_count = -1;
    int rc;
    fp = fopen(fname, "w");
    TEST_ASSERT_NOT_NULL(fp);
    fputs("10\n20\n30\n", fp);
    fclose(fp);
    rc = read_file_to_array(fname, 2, arr, &out_count, READ_MODE_INT);
    TEST_ASSERT_EQUAL_INT(0, rc);
    TEST_ASSERT_EQUAL_INT(2, out_count);
    TEST_ASSERT_EQUAL_INT(10, arr[0]);
    TEST_ASSERT_EQUAL_INT(20, arr[1]);
    remove(fname);
}

static void test_read_file_to_array_ints_all_invalid_lines_yields_zero_count(void)
{
    const char *fname = "tmp_invalid_ints.txt";
    FILE *fp;
    int arr[4];
    int out_count = -1;
    int rc;
    fp = fopen(fname, "w");
    TEST_ASSERT_NOT_NULL(fp);
    fputs("a\nB\nC\n", fp);
    fclose(fp);
    rc = read_file_to_array(fname, 4, arr, &out_count, READ_MODE_INT);
    TEST_ASSERT_EQUAL_INT(0, rc);
    TEST_ASSERT_EQUAL_INT(0, out_count);
    remove(fname);
}

static void test_read_file_to_array_ints_out_of_range_is_ignored(void)
{
    const char *fname = "tmp_ints_oor.txt";
    FILE *fp;
    int arr[4];
    int out_count = -1;
    int rc;
    /* First a valid int, then a huge out-of-range number (should be ignored), then another valid int */
    fp = fopen(fname, "w");
    TEST_ASSERT_NOT_NULL(fp);
    fputs("5\n", fp);
    fputs("99999999999999999999999999999999999999999999\n", fp);
    fputs("6\n", fp);
    fclose(fp);
    rc = read_file_to_array(fname, 4, arr, &out_count, READ_MODE_INT);
    TEST_ASSERT_EQUAL_INT(0, rc);
    TEST_ASSERT_EQUAL_INT(2, out_count);
    TEST_ASSERT_EQUAL_INT(5, arr[0]);
    TEST_ASSERT_EQUAL_INT(6, arr[1]);
    remove(fname);
}

static void test_read_file_to_array_ints_out_of_int_range_is_ignored(void)
{
    const char *fname = "tmp_ints_int_range.txt";
    FILE *fp;
    int arr[4];
    int out_count = -1;
    int rc;
    char buf[64];
    long beyond_int_max;

    beyond_int_max = (long)INT_MAX + 1L;
    sprintf(buf, "%ld\n", beyond_int_max);

    fp = fopen(fname, "w");
    TEST_ASSERT_NOT_NULL(fp);
    fputs("5\n", fp);
    fputs(buf, fp);
    fputs("6\n", fp);
    fclose(fp);

    rc = read_file_to_array(fname, 4, arr, &out_count, READ_MODE_INT);
    TEST_ASSERT_EQUAL_INT(0, rc);
    TEST_ASSERT_EQUAL_INT(2, out_count);
    TEST_ASSERT_EQUAL_INT(5, arr[0]);
    TEST_ASSERT_EQUAL_INT(6, arr[1]);
    remove(fname);
}

static void test_read_file_to_array_ints_long_line_is_not_split_into_multiple_entries(void)
{
    const char *fname = "tmp_ints_long_line.txt";
    FILE *fp;
    int arr[10];
    int out_count = -1;
    int rc;
    int i;

    fp = fopen(fname, "w");
    TEST_ASSERT_NOT_NULL(fp);
    /* Write a very long numeric line (way longer than MAX_LINE_LENGTH) */
    for (i = 0; i < 300; ++i)
        fputc('9', fp);
    fputc('\n', fp);
    /* Then a valid small int */
    fputs("7\n", fp);
    fclose(fp);

    rc = read_file_to_array(fname, 10, arr, &out_count, READ_MODE_INT);
    TEST_ASSERT_EQUAL_INT(0, rc);
    /* Long line should be treated as a single (invalid/out-of-range) line, not many lines */
    TEST_ASSERT_EQUAL_INT(1, out_count);
    TEST_ASSERT_EQUAL_INT(7, arr[0]);
    remove(fname);
}

static void test_read_file_to_array_ints_whitespace_only_lines_are_skipped(void)
{
    const char *fname = "tmp_ints_blank_ws.txt";
    FILE *fp;
    int arr[4];
    int out_count = -1;
    int rc;

    fp = fopen(fname, "w");
    TEST_ASSERT_NOT_NULL(fp);
    fputs("   \n", fp);
    fputs("\t\t\n", fp);
    fputs("5\n", fp);
    fclose(fp);

    rc = read_file_to_array(fname, 4, arr, &out_count, READ_MODE_INT);
    TEST_ASSERT_EQUAL_INT(0, rc);
    TEST_ASSERT_EQUAL_INT(1, out_count);
    TEST_ASSERT_EQUAL_INT(5, arr[0]);
    remove(fname);
}

static void test_read_file_to_array_ints_rejects_plus_sign(void)
{
    const char *fname = "tmp_ints_plus.txt";
    FILE *fp;
    int arr[4];
    int out_count = -1;
    int rc;

    fp = fopen(fname, "w");
    TEST_ASSERT_NOT_NULL(fp);
    fputs("+123\n", fp);
    fputs("7\n", fp);
    fclose(fp);

    rc = read_file_to_array(fname, 4, arr, &out_count, READ_MODE_INT);
    TEST_ASSERT_EQUAL_INT(0, rc);
    TEST_ASSERT_EQUAL_INT(1, out_count);
    TEST_ASSERT_EQUAL_INT(7, arr[0]);
    remove(fname);
}

static void test_read_file_to_array_nonexistent_returns_minus1(void)
{
    int arr[1];
    int out_count = -123;
    int rc = read_file_to_array("__no_such_file__.txt", 1, arr, &out_count, READ_MODE_INT);
    TEST_ASSERT_EQUAL_INT(-1, rc);
    TEST_ASSERT_EQUAL_INT(-123, out_count);
}

static void test_read_file_to_array_filestore_and_validate(void)
{
    const char *fname = "tmp_fs.txt";
    FILE *fp;
    FileStore entries[4];
    int out_count;
    int rc;
    fp = fopen(fname, "w");
    TEST_ASSERT_NOT_NULL(fp);
    fputs("1-3 a: abcde\n", fp);
    fputs("1-3 b: cdefg\n", fp);
    fputs("2-9 c: ccccccccc\n", fp);
    fclose(fp);
    out_count = 0;
    rc = read_file_to_array(fname, 4, entries, &out_count, READ_MODE_FILESTORE);
    TEST_ASSERT_EQUAL_INT(0, rc);
    TEST_ASSERT_EQUAL_INT(3, out_count);
    TEST_ASSERT_TRUE(is_letter_count_valid(entries[0]));
    TEST_ASSERT_TRUE(is_position_valid(entries[0]));
    TEST_ASSERT_FALSE(is_letter_count_valid(entries[1]));
    TEST_ASSERT_FALSE(is_position_valid(entries[1]));
    TEST_ASSERT_TRUE(is_letter_count_valid(entries[2]));
    TEST_ASSERT_FALSE(is_position_valid(entries[2]));
    remove(fname);
}

static void test_read_file_to_array_filestore_respects_array_size_limit(void)
{
    const char *fname = "tmp_fs_limit.txt";
    FILE *fp;
    FileStore entries[1];
    int out_count = -1;
    int rc;
    fp = fopen(fname, "w");
    TEST_ASSERT_NOT_NULL(fp);
    fputs("1-3 a: abcde\n", fp);
    fputs("1-3 b: cdefg\n", fp);
    fclose(fp);
    rc = read_file_to_array(fname, 1, entries, &out_count, READ_MODE_FILESTORE);
    TEST_ASSERT_EQUAL_INT(0, rc);
    TEST_ASSERT_EQUAL_INT(1, out_count);
    remove(fname);
}

static void test_read_file_to_array_filestore_all_malformed_yields_zero_count(void)
{
    const char *fname = "tmp_fs_bad.txt";
    FILE *fp;
    FileStore entries[3];
    int out_count = -1;
    int rc;
    fp = fopen(fname, "w");
    TEST_ASSERT_NOT_NULL(fp);
    fputs("bad line\n", fp);
    fputs("still bad\n", fp);
    fclose(fp);
    rc = read_file_to_array(fname, 3, entries, &out_count, READ_MODE_FILESTORE);
    TEST_ASSERT_EQUAL_INT(0, rc);
    TEST_ASSERT_EQUAL_INT(0, out_count);
    remove(fname);
}

static void test_read_file_to_array_unknown_mode_returns_error(void)
{
    const char *fname = "tmp_unknown.txt";
    FILE *fp;
    int dummy[1];
    int out_count;
    int rc;
    fp = fopen(fname, "w");
    TEST_ASSERT_NOT_NULL(fp);
    fputs("42\n", fp);
    fclose(fp);
    out_count = 0;
    rc = read_file_to_array(fname, 1, dummy, &out_count, (ReadMode)99);
    TEST_ASSERT_EQUAL_INT(-1, rc);
    remove(fname);
}

static void test_read_file_to_array_invalid_args_returns_error(void)
{
    int out_count = 123;
    int rc;
    rc = read_file_to_array("tmp_doesnt_matter.txt", 0, NULL, &out_count, READ_MODE_INT);
    TEST_ASSERT_EQUAL_INT(-1, rc);
    TEST_ASSERT_EQUAL_INT(0, out_count);
}

/* --- Day 2: validators boundary conditions --- */
static void test_day2_position_out_of_range_is_false(void)
{
    FileStore fs;
    int ok;
    fs.min = 2; fs.max = 5; fs.letter = 'a';
    fs.value[0] = 'a'; fs.value[1] = '\0';
    ok = is_position_valid(fs);
    TEST_ASSERT_FALSE(ok);
}

/* --- DotHashGrid: load + get/set --- */
static void test_read_dot_hash_grid_and_get_wraps(void)
{
    const char *fname = "tmp_grid.txt";
    FILE *fp;
    DotHashGrid grid;
    int rc;

    fp = fopen(fname, "w");
    TEST_ASSERT_NOT_NULL(fp);
    fputs(".#.\n", fp);
    /* Whitespace-only line should be treated as empty */
    fputs("   \n", fp);
    fputs("#..\n", fp);
    fclose(fp);

    grid.rows = 0;
    grid.cols = 0;
    grid.data = NULL;
    rc = read_dot_hash_grid(fname, &grid);
    TEST_ASSERT_EQUAL_INT(0, rc);
    TEST_ASSERT_EQUAL_INT(2, grid.rows);
    TEST_ASSERT_EQUAL_INT(3, grid.cols);

    TEST_ASSERT_EQUAL_CHAR('.', dothash_get(&grid, 0, 0, 0));
    TEST_ASSERT_EQUAL_CHAR('#', dothash_get(&grid, 0, 1, 0));
    TEST_ASSERT_EQUAL_CHAR('.', dothash_get(&grid, 1, 2, 0));

    /* Wrapping: col 4 wraps to 1 (4 % 3) */
    TEST_ASSERT_EQUAL_CHAR('#', dothash_get(&grid, 0, 4, 1));

    free_dot_hash_grid(&grid);
    remove(fname);
}

static void test_dothash_set_and_get(void)
{
    DotHashGrid grid;
    int rc;

    grid.rows = 2;
    grid.cols = 2;
    grid.data = (char *)calloc(4, sizeof(char));
    TEST_ASSERT_NOT_NULL(grid.data);
    grid.data[0] = '.'; grid.data[1] = '.';
    grid.data[2] = '.'; grid.data[3] = '.';

    rc = dothash_set(&grid, 1, 1, '#', 0);
    TEST_ASSERT_EQUAL_INT(0, rc);
    TEST_ASSERT_EQUAL_CHAR('#', dothash_get(&grid, 1, 1, 0));

    /* Invalid value is rejected */
    rc = dothash_set(&grid, 0, 0, 'X', 0);
    TEST_ASSERT_EQUAL_INT(-1, rc);

    free_dot_hash_grid(&grid);
}

static void test_free_dot_hash_grid_is_idempotent(void)
{
    DotHashGrid grid;
    grid.rows = 1;
    grid.cols = 1;
    grid.data = (char *)malloc(1);
    TEST_ASSERT_NOT_NULL(grid.data);
    grid.data[0] = '#';
    free_dot_hash_grid(&grid);
    TEST_ASSERT_NULL(grid.data);
    TEST_ASSERT_EQUAL_INT(0, grid.rows);
    TEST_ASSERT_EQUAL_INT(0, grid.cols);
    /* Second call should be safe */
    free_dot_hash_grid(&grid);
    TEST_ASSERT_NULL(grid.data);
}

static void test_read_dot_hash_grid_pads_short_rows_with_dots(void)
{
    const char *fname = "tmp_grid_pad.txt";
    FILE *fp;
    DotHashGrid grid;
    int rc;

    fp = fopen(fname, "w");
    TEST_ASSERT_NOT_NULL(fp);
    fputs("#\n", fp);
    fputs("##\n", fp);
    fclose(fp);

    grid.rows = 0;
    grid.cols = 0;
    grid.data = NULL;
    rc = read_dot_hash_grid(fname, &grid);
    TEST_ASSERT_EQUAL_INT(0, rc);
    TEST_ASSERT_EQUAL_INT(2, grid.rows);
    TEST_ASSERT_EQUAL_INT(2, grid.cols);

    /* First row is "#" and should be padded to "#." */
    TEST_ASSERT_EQUAL_CHAR('#', dothash_get(&grid, 0, 0, 0));
    TEST_ASSERT_EQUAL_CHAR('.', dothash_get(&grid, 0, 1, 0));

    free_dot_hash_grid(&grid);
    remove(fname);
}

/* --- count_lines_in_file: custom modes --- */
static void test_count_lines_in_file_custom1_password_format(void)
{
    const char *fname = "tmp_custom1.txt";
    FILE *fp;
    int real_lines;
    int total;

    fp = fopen(fname, "w");
    TEST_ASSERT_NOT_NULL(fp);
    /* Valid */
    fputs("1-3 a: abcde\n", fp);
    /* Whitespace-only should be treated as empty */
    fputs("    \n", fp);
    /* Invalid due to '*' */
    fputs("1-3 b: ab*de\n", fp);
    /* Valid */
    fputs("2-9 c: ccccccccc\n", fp);
    fclose(fp);

    real_lines = 0;
    total = count_lines_in_file(fname, &real_lines, LINE_MODE_CUSTOM1);
    TEST_ASSERT_EQUAL_INT(2, real_lines);
    TEST_ASSERT_TRUE(total >= real_lines);
    remove(fname);
}

static void test_count_lines_in_file_custom2_dot_hash_only(void)
{
    const char *fname = "tmp_custom2.txt";
    FILE *fp;
    int real_lines;
    int total;

    fp = fopen(fname, "w");
    TEST_ASSERT_NOT_NULL(fp);
    fputs("..##..\n", fp);
    fputs("..x#..\n", fp); /* invalid due to x */
    fputs("#.\n", fp);
    fclose(fp);

    real_lines = 0;
    total = count_lines_in_file(fname, &real_lines, LINE_MODE_CUSTOM2);
    TEST_ASSERT_EQUAL_INT(2, real_lines);
    TEST_ASSERT_TRUE(total >= real_lines);
    remove(fname);
}

/* --- Files: READ_MODE_DOTHASH parsing into fixed rows --- */
static void test_read_file_to_array_dothash_parses_valid_rows_only(void)
{
    const char *fname = "tmp_rows_dothash.txt";
    FILE *fp;
    char rows[4][MAX_LINE_LENGTH];
    int out_count;
    int rc;

    fp = fopen(fname, "w");
    TEST_ASSERT_NOT_NULL(fp);
    fputs("..##..\n", fp);
    fputs("\n", fp);
    fputs("..x#..\n", fp); /* invalid */
    fputs("#.\n", fp);
    fclose(fp);

    memset(rows, 0, sizeof(rows));
    out_count = 0;
    rc = read_file_to_array(fname, 4, rows, &out_count, READ_MODE_DOTHASH);
    TEST_ASSERT_EQUAL_INT(0, rc);
    TEST_ASSERT_EQUAL_INT(2, out_count);
    TEST_ASSERT_EQUAL_STRING("..##..", rows[0]);
    TEST_ASSERT_EQUAL_STRING("#.", rows[1]);
    remove(fname);
}

static void test_read_dot_hash_grid_all_invalid_is_empty(void)
{
    const char *fname = "tmp_grid_invalid.txt";
    FILE *fp;
    DotHashGrid grid;
    int rc;

    fp = fopen(fname, "w");
    TEST_ASSERT_NOT_NULL(fp);
    fputs("abc\n", fp);
    fputs("123\n", fp);
    fputs("..x..\n", fp);
    fclose(fp);

    grid.rows = 123;
    grid.cols = 456;
    grid.data = (char *)0x1;
    rc = read_dot_hash_grid(fname, &grid);
    TEST_ASSERT_EQUAL_INT(0, rc);
    TEST_ASSERT_EQUAL_INT(0, grid.rows);
    TEST_ASSERT_EQUAL_INT(0, grid.cols);
    TEST_ASSERT_NULL(grid.data);
    remove(fname);
}

static void test_dothash_get_out_of_bounds_returns_nul(void)
{
    DotHashGrid grid;

    grid.rows = 2;
    grid.cols = 2;
    grid.data = (char *)calloc(4, sizeof(char));
    TEST_ASSERT_NOT_NULL(grid.data);
    grid.data[0] = '.'; grid.data[1] = '#';
    grid.data[2] = '#'; grid.data[3] = '.';

    TEST_ASSERT_EQUAL_CHAR('\0', dothash_get(&grid, -1, 0, 0));
    TEST_ASSERT_EQUAL_CHAR('\0', dothash_get(&grid, 0, -1, 0));
    TEST_ASSERT_EQUAL_CHAR('\0', dothash_get(&grid, 2, 0, 0));
    TEST_ASSERT_EQUAL_CHAR('\0', dothash_get(&grid, 0, 2, 0));

    /* With wrapping enabled, negative cols should wrap */
    TEST_ASSERT_EQUAL_CHAR('#', dothash_get(&grid, 0, -1, 1));

    free_dot_hash_grid(&grid);
}

static void test_dothash_set_out_of_bounds_returns_error(void)
{
    DotHashGrid grid;

    grid.rows = 2;
    grid.cols = 2;
    grid.data = (char *)calloc(4, sizeof(char));
    TEST_ASSERT_NOT_NULL(grid.data);
    grid.data[0] = '.'; grid.data[1] = '.';
    grid.data[2] = '.'; grid.data[3] = '.';

    TEST_ASSERT_EQUAL_INT(-1, dothash_set(&grid, -1, 0, '#', 0));
    TEST_ASSERT_EQUAL_INT(-1, dothash_set(&grid, 0, -1, '#', 0));
    TEST_ASSERT_EQUAL_INT(-1, dothash_set(&grid, 2, 0, '#', 0));
    TEST_ASSERT_EQUAL_INT(-1, dothash_set(&grid, 0, 2, '#', 0));

    /* Wrapping enabled should allow large/negative col */
    TEST_ASSERT_EQUAL_INT(0, dothash_set(&grid, 0, -1, '#', 1));
    TEST_ASSERT_EQUAL_CHAR('#', dothash_get(&grid, 0, 1, 0));

    free_dot_hash_grid(&grid);
}

static void test_algorithm_guards_null_and_small_sizes(void)
{
    Matches p;
    ManyMatches t;

    bubble_sort(NULL, 10);
    p = find_pair_sorted(NULL, 2, 10);
    TEST_ASSERT_FALSE(p.found);
    p = find_pair_sorted((int *)0, 1, 10);
    TEST_ASSERT_FALSE(p.found);

    t = find_triple_sorted(NULL, 3, 10);
    TEST_ASSERT_FALSE(t.found);
    t = find_triple_sorted((int *)0, 2, 10);
    TEST_ASSERT_FALSE(t.found);
}

/* --- Hash algorithms: negative/out-of-range values should not abort search --- */
static void test_find_pair_hash_ignores_negative_values(void)
{
    int arr[3];
    Matches res;
    int maxv;

    arr[0] = 1721;
    arr[1] = -1;
    arr[2] = 299;
    maxv = find_max(arr, 3);
    res = find_pair(arr, 3, maxv, 2020);
    TEST_ASSERT_TRUE(res.found);
    TEST_ASSERT_EQUAL_INT(2020, res.num1 + res.num2);
}

static void test_find_triple_hash_ignores_negative_values(void)
{
    int arr[7];
    ManyMatches res;
    int maxv;

    /* Standard example plus an extra negative */
    arr[0] = 1721; arr[1] = 979; arr[2] = 366; arr[3] = 299; arr[4] = 675; arr[5] = 1456; arr[6] = -50;
    maxv = find_max(arr, 7);
    res = find_triple(arr, 7, maxv, 2020);
    TEST_ASSERT_TRUE(res.found);
    TEST_ASSERT_EQUAL_INT(2020, res.num1 + res.num2 + res.num3);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_find_max_returns_largest);
    RUN_TEST(test_bubble_sort_sorts_ascending);
    RUN_TEST(test_qsort_sorts_ascending);
    RUN_TEST(test_find_pair_sorted_2020_example);
    RUN_TEST(test_find_pair_hash_2020_example);
    RUN_TEST(test_find_triple_hash_2020_example);
    RUN_TEST(test_find_triple_sorted_2020_example);
    RUN_TEST(test_find_pair_sorted_no_solution);
    RUN_TEST(test_find_pair_hash_no_solution);
    RUN_TEST(test_find_triple_no_solution_hash_and_sorted);
    RUN_TEST(test_count_lines_in_file_digit_mode);
    RUN_TEST(test_count_lines_in_file_empty_file);
    RUN_TEST(test_count_lines_in_file_nonexistent_returns_minus1);
    RUN_TEST(test_read_file_to_array_ints);
    RUN_TEST(test_read_file_to_array_ints_respects_array_size_limit);
    RUN_TEST(test_read_file_to_array_ints_all_invalid_lines_yields_zero_count);
    RUN_TEST(test_read_file_to_array_ints_out_of_range_is_ignored);
    RUN_TEST(test_read_file_to_array_ints_out_of_int_range_is_ignored);
    RUN_TEST(test_read_file_to_array_ints_long_line_is_not_split_into_multiple_entries);
    RUN_TEST(test_read_file_to_array_ints_whitespace_only_lines_are_skipped);
    RUN_TEST(test_read_file_to_array_ints_rejects_plus_sign);
    RUN_TEST(test_read_file_to_array_nonexistent_returns_minus1);
    RUN_TEST(test_read_file_to_array_filestore_and_validate);
    RUN_TEST(test_read_file_to_array_filestore_respects_array_size_limit);
    RUN_TEST(test_read_file_to_array_filestore_all_malformed_yields_zero_count);
    RUN_TEST(test_read_file_to_array_unknown_mode_returns_error);
    RUN_TEST(test_read_file_to_array_invalid_args_returns_error);
    RUN_TEST(test_day2_position_out_of_range_is_false);
    RUN_TEST(test_read_dot_hash_grid_and_get_wraps);
    RUN_TEST(test_dothash_set_and_get);
    RUN_TEST(test_free_dot_hash_grid_is_idempotent);
    RUN_TEST(test_read_dot_hash_grid_pads_short_rows_with_dots);
    RUN_TEST(test_count_lines_in_file_custom1_password_format);
    RUN_TEST(test_count_lines_in_file_custom2_dot_hash_only);
    RUN_TEST(test_read_file_to_array_dothash_parses_valid_rows_only);
    RUN_TEST(test_read_dot_hash_grid_all_invalid_is_empty);
    RUN_TEST(test_dothash_get_out_of_bounds_returns_nul);
    RUN_TEST(test_dothash_set_out_of_bounds_returns_error);
    RUN_TEST(test_algorithm_guards_null_and_small_sizes);
    RUN_TEST(test_find_pair_hash_ignores_negative_values);
    RUN_TEST(test_find_triple_hash_ignores_negative_values);
    return UNITY_END();
}

