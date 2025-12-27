#ifndef COMPARISON_H
#define COMPARISON_H

#include "avl.h"
#include "bst.h"
#include "input.h"
#include "hst.h"
#include <time.h>

typedef struct
{
    double bst_search_time;
    double avl_search_time;
    double hash_chaining_search_time;
    double hash_open_search_time;

    int bst_comparisons;
    int avl_comparisons;
    int hash_chaining_comparisons;
    int hash_open_comparisons;

    size_t bst_memory;
    size_t avl_memory;
    size_t hash_chaining_memory;
    size_t hash_open_memory;

    int bst_height;
    int avl_height;
    double hash_chaining_load_factor;
    double hash_open_load_factor;
} comparison_results_t;

comparison_results_t compare_structures(bst_node_t *bst_root, avl_node_t *avl_root, hst_chaining_t *hash_chaining, hst_open_t *hash_open);
void print_comparison_results(comparison_results_t *results);
void benchmark_structures(const char *filename, int hash_table_size);

#endif