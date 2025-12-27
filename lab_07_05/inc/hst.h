#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auxiliary.h"

#define MAX_LOAD_FACTOR 0.7
#define INITIAL_TABLE_SIZE 101

typedef struct hash_node_t
{
    char *word;
    int count;
    struct hash_node_t *next;
    bool is_deleted;
} hash_node_t;

typedef struct
{
    hash_node_t **table;
    int size;
    int count;
    int collisions;
    int total_comparisons;
    int searches_count;
} hst_chaining_t;

typedef struct
{
    char **keys;
    int *counts;
    bool *occupied;
    bool *deleted;
    int size;
    int count;
    int collisions;
    int total_comparisons;
    int searches_count;
} hst_open_t;

hst_chaining_t *create_hash_table_chaining(int size);
void free_hash_table_chaining(hst_chaining_t *ht);
unsigned int hash_function(const char *word, int table_size);
void hash_table_insert_chaining(hst_chaining_t *ht, const char *word);
hash_node_t *hash_table_search_chaining(hst_chaining_t *ht, const char *word, int *comparisons);
void hash_table_delete_chaining(hst_chaining_t *ht, const char *word);
void rehash_chaining(hst_chaining_t *ht);

hst_open_t *create_hash_table_open(int size);
void free_hash_table_open(hst_open_t *ht);
void hash_table_insert_open(hst_open_t *ht, const char *word);
int hash_table_search_open(hst_open_t *ht, const char *word, int *comparisons);
void hash_table_delete_open(hst_open_t *ht, const char *word);
void rehash_open(hst_open_t *ht);

double calculate_load_factor_chaining(hst_chaining_t *ht);
double calculate_load_factor_open(hst_open_t *ht);
void build_hash_table_from_file_chaining(hst_chaining_t *ht, const char *filename);
void build_hash_table_from_file_open(hst_open_t *ht, const char *filename);
double get_average_comparisons_chaining(hst_chaining_t *ht);
double get_average_comparisons_open(hst_open_t *ht);

#endif