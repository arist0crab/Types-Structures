#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include "data.h"
#include "color.h"
#include <stdio.h>

// TODO добавить doxygen
status_t print_menu(void);
status_t print_exit_code_result(status_t exit_code);
status_t print_insert_menu(char *target_word);

/* TODO
status_t print_compare_find_operation_table(double time_balanced, double time_degenerate, double time_random);
status_t print_compare_sort_operation_table(double time_balanced, double time_degenerate, double time_random);
*/

#endif