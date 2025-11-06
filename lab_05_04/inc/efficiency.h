#ifndef __EFFICIENCU_H__
#define __EFFICIENCU_H__

#include "data.h"
#include "simulate.h"
#include <time.h>

#define EFFICIENCY_ITERATIONS_QUANTITY 1000
#define FILL_N_EMPTY_ITERATIONS_QUANTITY 100

// TODO написать документацию
status_t measure_efficiency(void);

// TODO написать документацию
status_t fill_and_empty_arr_queue(void);

// TODO написать документацию
status_t fill_and_empty_list_queue(void);

#endif