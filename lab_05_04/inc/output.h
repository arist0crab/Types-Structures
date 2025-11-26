#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include "data.h"
#include "color.h"
#include <stdio.h>

status_t print_menu(void);
status_t print_simulation_config_menu(void);
status_t print_simulation_configs(time_range_t *arr_t1, time_range_t *arr_t2, time_range_t *serve_t1, time_range_t *serve_t2);
status_t print_exit_code_result(status_t exit_code);
status_t print_push_pop_table(double arr_push_time, double arr_pop_time, double list_push_time, double list_pop_time);

#endif