#ifndef __TREE_FUNCTIONAL_H__
#define __TREE_FUNCTIONAL_H__

#include "data.h"
#include "color.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define PARENT(i) ((i-1) / 2)
#define MAX_PREFIX_SIZE 256

status_t insert_tree_node(tree_node_t **root, const char *word);
status_t delete_tree_node(tree_node_t **root, const char *word);
status_t clear_tree(tree_node_t **root);
status_t find_word_in_tree(tree_node_t *root, tree_node_t **target_root, const char *word);
status_t print_pretty_tree(tree_node_t *root);
char *str_dynamic_copy(const char *s);

#endif