#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include "data.h"

status_t print_menu(void);
status_t print_exit_code_result(status_t exit_code);
status_t print_find_result(const tree_node_t **root);

#endif