#ifndef __INPUT_H__
#define __INPUT_H__

#include "data.h"
#include "color.h"
#include <stdio.h>

status_t input_time_gap(double *min_time, double *max_time);
status_t input_cur_menu_opt(int *cur_menu_opt, int const menu_points_quantity);

#endif