#ifndef __FILE_FUNCTIONAL_H__
#define __FILE_FUNCTIONAL_H__

#include "data.h"
#include "tree_functional.h"
#include <stdio.h>
#include <unistd.h>

status_t read_tree_from_file(tree_node_t **root, char *filename);
status_t find_word_in_file(char *filename, char *word, ssize_t *word_num);

#endif