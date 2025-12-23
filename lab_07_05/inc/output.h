#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>
#include <string.h>
#include "data.h"
#include "hst.h"
#include "color.h"

void print_menu(void);
void print_result(const result_t exit_code);
result_t print_chaining_hash_table(const hst_chaining_t *table);
result_t print_open_hash_table(const hst_open_t *table);
void print_compare_table(double average_tree_time_ns, double average_file_time_ns);
void print_compare_trees_table(double average_balanced_time_ns, double average_degenerate_time_ns, double average_random_time_ns);
void print_compare_trees_table_2(double average_balanced_time_ns, double average_degenerate_time_ns, double average_random_time_ns);

#endif