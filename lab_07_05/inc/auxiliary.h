#ifndef __AUXILIARY_H__
#define __AUXILIARY_H__

#include "data.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// TODO добавить doxygen
char *str_dynamic_copy(const char *s);
status_t safe_open_file(char *filename, FILE **filestream);
status_t free_words_arr(char ***words_arr, size_t arr_length);
status_t get_words_arr_from_file(const char *filename, char ***words_arr, size_t *arr_length);

#endif