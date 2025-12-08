#ifndef __BST_H__
#define __BST_H__

#include "data.h"
#include <stdio.h>

typedef struct tree_node
{
    char *word;
    size_t counted;
    struct tree_node *left;
    struct tree_node *right;
} tree_node_t;

#endif