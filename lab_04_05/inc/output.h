#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include "data.h"
#include "color.h"
#include <stdio.h>

status_t print_compare_table_header(void);
status_t print_menu(void);
status_t print_pop_result(int result);
status_t print_calc_result(int result);
status_t print_exit_code_result(status_t exit_code);
status_t print_push_pop_table(double arr_push_time, double arr_pop_time, double list_push_time, double list_pop_time);
status_t print_table_raw(char op_char, size_t stack_size, size_t memus_array, size_t memus_list, double time_array, double time_list);

#endif