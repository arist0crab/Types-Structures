#ifndef __INPUT_H__
#define __INPUT_H__

#include "data.h"
#include "matrix.h"
#include "color.h"
#include <stdio.h>

status_t input_cur_menu_opt(menu_option_t *cur_menu_opt);
status_t input_any_matrix(void);
status_t input_matrix_dimensions(size_t *rows, size_t *cols);

#endif