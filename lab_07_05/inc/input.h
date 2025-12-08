#ifndef __INPUT_H__
#define __INPUT_H__

#include "../inc/data.h"  // TODO
#include "color.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STRING_LENGTH 256

status_t input_cur_menu_opt(int *cur_menu_opt, int const menu_points_quantity);
status_t input_string(char **word, char *message);

#endif