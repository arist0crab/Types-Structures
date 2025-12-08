#ifndef __BST_H__
#define __BST_H__

#include "data.h"
#include "color.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct tree_node
{
    char *word;
    size_t counted;
    struct tree_node *left;
    struct tree_node *right;
} bst_node_t;

// TODO добавить doxygen
status_t read_tree_from_file(bst_node_t **root, char *filename);
status_t print_pretty_bst(bst_node_t *root);

#endif