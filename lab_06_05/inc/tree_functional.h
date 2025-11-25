#ifndef __TREE_FUNCTIONAL_H__
#define __TREE_FUNCTIONAL_H__

#include "data.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// TODO добавить doxygen

status_t insert_tree_node(tree_node_t **root, const char *word);
status_t print_tree(tree_node_t *root, size_t nesting_level);
status_t delete_tree_node(tree_node_t **root, const char *word);
status_t find_word_in_tree(tree_node_t *root, tree_node_t **target_root, const char *word);

#endif