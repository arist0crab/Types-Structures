#ifndef __PROCESS_H__
#define __PROCESS_H__

#include "data.h"
#include "input.h"
#include "output.h"
#include "tree_functional.h"
#include "file_functional.h"
#include <time.h>

status_t procces_menu_choice(menu_option_t menu_option, tree_node_t **root);
status_t print_compare_table(double average_tree_time_ns, double average_file_time_ns);

#endif