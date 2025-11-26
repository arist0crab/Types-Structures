#ifndef __GET_RANDOM_WORDS__H
#define __GET_RANDOM_WORDS__H

#include "data.h"
#include "file_functional.h"
#include "tree_functional.h"
#include <stdio.h>

status_t get_random_word_from_file(const char *filename, char **out_word);
status_t get_random_word_from_tree(tree_node_t *root, char **out_word, size_t words_in_tree_quantity);
status_t count_nodes(const tree_node_t *root, size_t *cnt);


#endif