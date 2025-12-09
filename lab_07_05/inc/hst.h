#ifndef __HST_H__
#define __HST_H__

#include "data.h"
#include "input.h"
#include "auxiliary.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct hash_node {
    char *word;
    size_t tail_length;  // сколько узлов "прицепилось" к данному
    struct hash_node *next;
} hash_node_t;

typedef struct hash_table {
    hash_node_t **data;     
    size_t max_size;
    size_t uniq_words_quantity;
} hash_table_t;

// TODO добавить doxygen
status_t delete_hst_node(hash_table_t **table, const char *word);
status_t user_init_table(hash_table_t **table);
status_t print_hash_table(const hash_table_t *table);
status_t build_hash_from_file(hash_table_t **table, const char *filename);
status_t insert_hst_node(hash_table_t *table, const char *word);
status_t clear_hst_table(hash_table_t **table);
status_t resize_hst_table(hash_table_t **table);

#endif