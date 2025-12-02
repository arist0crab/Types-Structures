#define _POSIX_C_SOURCE 199309L
#include "compare_efficiency.h"

// TODO возможно, пеписать balanced_tree.txt

status_t measure_one_file_for_find(const char *filename, tree_node_t **root, char ***words_arr, size_t *arr_length, double *result_average_time);

status_t compare_find_operation(void)
{
    status_t ec = SUCCESS_CODE;
    tree_node_t *root = NULL;
    char **words_arr = NULL;
    size_t arr_length = 0;

    const char *tree_balanced_file = "tree_balanced.txt";
    const char *tree_degenerate_file = "tree_degenerate.txt";
    const char *tree_random_file = "tree_random.txt";

    double tree_balanced_time = 0;
    double tree_degenerate_time = 0;
    double tree_random_time = 0;

    ec = measure_one_file_for_find(tree_balanced_file, &root, &words_arr, &arr_length, &tree_balanced_time);

    if (ec == SUCCESS_CODE)
        ec = measure_one_file_for_find(tree_degenerate_file, &root, &words_arr, &arr_length, &tree_degenerate_time);

    if (ec == SUCCESS_CODE)
        ec = measure_one_file_for_find(tree_random_file, &root, &words_arr, &arr_length, &tree_random_time);

    print_compare_find_operation_table(tree_balanced_time, tree_degenerate_time, tree_random_time);

    return ec;
}

status_t measure_one_file_for_find(const char *filename, tree_node_t **root, char ***words_arr, size_t *arr_length, double *result_average_time)
{
    status_t ec = SUCCESS_CODE;
    double total_tree_time = 0;
    tree_node_t *target_root = NULL;
    struct timespec start_time, end_time;

    if (!filename || !root || !words_arr || !arr_length || !result_average_time)
        ec = ERR_ARGS;

    if (ec == SUCCESS_CODE)
        ec = get_words_arr_from_file(filename, words_arr, arr_length);

    if (ec == SUCCESS_CODE)
        ec = read_tree_from_file(root, (char *)filename);

    for (size_t i = 0; ec == SUCCESS_CODE && i < *arr_length; i++)
    {
        clock_gettime(CLOCK_MONOTONIC, &start_time);
        ec = find_word_in_tree(*root, &target_root, (const char *)(*words_arr)[i]);
        clock_gettime(CLOCK_MONOTONIC, &end_time);
        total_tree_time += (end_time.tv_sec - start_time.tv_sec) * 10e9 + (end_time.tv_nsec - start_time.tv_nsec);
    }

    if (*words_arr)
        free_words_arr(words_arr, *arr_length);

    if (ec == SUCCESS_CODE)
        *result_average_time = total_tree_time / *arr_length;

    return ec;
}