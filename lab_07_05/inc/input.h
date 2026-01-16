#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "color.h"
#include "auxiliary.h"

status_t input_menu_option(menu_option_t *current_option);
status_t input_filename(char *filename);
status_t input_string(char **word, char *message);
status_t input_size(size_t *number);
status_t get_words_from_file(const char *filename, char ***arr, size_t *arr_length);

#endif