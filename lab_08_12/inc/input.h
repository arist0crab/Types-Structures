#ifndef __INPUT_H__
#define __INPUT_H__

#include "../inc/data.h"  // TODO
#include "color.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// TODO добавить doxygen
status_t input_cur_menu_opt(int *cur_menu_opt, int const menu_points_quantity);
status_t input_string(char **word, const char *message);
status_t input_size(size_t *size, const char *message);
status_t get_two_cities_and_distances(char **city_name_1, char **city_name_2, size_t *distance_1_to_2, size_t *distance_2_to_1);

#endif