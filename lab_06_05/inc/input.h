#ifndef __INPUT_H__
#define __INPUT_H__

#include "data.h"
#include "color.h"
#include <string.h>
#include <stdlib.h>

status_t input_cur_menu_opt(int *cur_menu_opt, int const menu_points_quantity);
status_t input_string(char **word, char *message);

#endif