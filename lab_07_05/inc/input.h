#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "color.h"
#include "auxiliary.h"

result_t input_menu_option(menu_option_t *current_option);
result_t input_filename(char *filename);
// result_t input_integer(int *number);
result_t input_string(char **word, char *message);
result_t input_size(size_t *number);
// result_t get_integers_from_file(const char *filename, int **arr, size_t *arr_length);
result_t get_words_from_file(const char *filename, char ***arr, size_t *arr_length);

#endif