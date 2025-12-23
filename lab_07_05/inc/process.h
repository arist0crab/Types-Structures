#ifndef PROCESS_H
#define PROCESS_H

#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include "data.h"
#include "input.h"
#include "output.h"
#include "bst.h"
#include "avl.h"
#include "visualization.h"
#include "hst.h"
#include "comparison.h"

result_t process_current_option(menu_option_t current_option, bst_node_t **bst_root, avl_node_t **avl_root, hst_chaining_t **chaining_hst, hst_open_t **open_hst);

#endif