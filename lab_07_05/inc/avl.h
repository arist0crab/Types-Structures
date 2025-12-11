#ifndef __AVL_H__
#define __AVL_H__

#include "data.h"
#include "bst.h"
#include <stdbool.h>

// TODO добавить doxygen
status_t balance_tree(bst_node_t **bst_root);

#endif