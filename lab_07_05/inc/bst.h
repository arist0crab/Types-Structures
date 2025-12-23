#ifndef BST_H
#define BST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "data.h"
#include "color.h"
#include "auxiliary.h"

typedef struct bst_node bst_node_t;

struct bst_node
{
    char *word;
    int count;
    bst_node_t *left;
    bst_node_t *right;
};

void print_bst(bst_node_t *root);
result_t make_bst_from_file(bst_node_t **root, const char *filename);
result_t insert_bst_node(bst_node_t **root, const char *word);
result_t delete_bst_node(bst_node_t **root, const char *word, bool *to_del_found);
result_t find_word_in_bst(bst_node_t **root, const char *word, bool flag, int *comparisons);
result_t define_nodes_quantity_on_each_level(bst_node_t **root);
result_t balance_tree(bst_node_t **root);
void infix(bst_node_t *root);
size_t tree_height(bst_node_t *root);
void free_bst(bst_node_t *root);
size_t count_nodes_bst(bst_node_t *root);

#endif