#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include "data.h"
#include "color.h"
#include "matrix.h"
#include <stdio.h>

status_t print_menu(void);
status_t print_result(status_t exit_code);
status_t print_final_common_result(status_t exit_code);
status_t print_all_matrices(void);

#endif