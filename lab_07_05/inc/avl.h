#ifndef AVL_H
#define AVL_H

#include "bst.h"
#include "color.h"
#include "data.h"
#include "auxiliary.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct avl_node_t
{
    char *word;
    int count;
    int height;
    struct avl_node_t *left;
    struct avl_node_t *right;
} avl_node_t;

void print_avl(avl_node_t *root);
result_t make_avl_from_file(avl_node_t **root, const char *filename);
void free_avl(avl_node_t *root);
size_t avl_height(avl_node_t *root);
result_t insert_avl_node(avl_node_t **root, const char *word);
result_t delete_avl_node(avl_node_t **root, const char *word, bool *to_del_found);
result_t find_word_in_avl(avl_node_t **root, const char *word, bool flag, int *comparisons);
size_t count_nodes_avl(avl_node_t *root);

#endif