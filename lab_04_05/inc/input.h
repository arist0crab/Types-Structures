#ifndef __INPUT_H__
#define __INPUT_H__

#include "data.h"
#include "color.h"
#include <stdio.h>
#include <string.h>

status_t input_cur_menu_opt(menu_option_t *cur_menu_opt);
status_t get_expression(char *expression);
status_t get_stack_elem(int *elem);

#endif