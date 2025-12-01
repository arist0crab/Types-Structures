#ifndef __FILE_FUNCTIONAL_H__
#define __FILE_FUNCTIONAL_H__

#include "data.h"
#include "tree_functional.h"
#include <stdio.h>
#include <unistd.h>

status_t read_tree_from_file(tree_node_t **root, char *filename);
status_t find_word_in_file(char *filename, char *word, ssize_t *word_num);
status_t insert_word_to_file(const char *filename, char *target_word);
status_t safe_open_file(char *filename, FILE **filestream);
status_t get_words_arr_from_file(const char *filename, char ***words_arr, size_t *arr_length);
status_t free_words_arr(char ***words_arr, size_t arr_length);

#endif