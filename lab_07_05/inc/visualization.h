#ifndef GRAPHVIZ_VISUALIZATION_H
#define GRAPHVIZ_VISUALIZATION_H

#include "hst.h"
#include "bst.h"
#include "avl.h"
#include "data.h"
#include <math.h>

void visualize_bst_tree(bst_node_t *root, const char *output_filename);
void visualize_avl_tree(avl_node_t *root, const char *output_filename);
void visualize_hash_table_chaining(hst_chaining_t *ht, const char *output_filename);
void visualize_hash_table_open(hst_open_t *ht, const char *output_filename);

void display_bst_tree_png(bst_node_t *root);
void display_avl_tree_png(avl_node_t *root);
void display_hash_table_chaining_png(hst_chaining_t *ht);
void display_hash_table_open_png(hst_open_t *ht);

void generate_dot_for_bst(bst_node_t *root, FILE *dot_file, int *node_id);
void generate_dot_for_avl(avl_node_t *root, FILE *dot_file, int *node_id);

#endif