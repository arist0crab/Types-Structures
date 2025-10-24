#ifndef __ARR_STACK_H__
#define __ARR_STACK_H__

#include "data.h"
#include "color.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ARR_STACK_SIZE 100

typedef struct
{
    int data[MAX_ARR_STACK_SIZE];
    size_t current_size;
} arr_stack_t;

status_t print_arr_stack(arr_stack_t *arr_stack);
status_t push_arr_stack(arr_stack_t *arr_stack, int value);
status_t pop_arr_stack(arr_stack_t *arr_stack, int *popped_value);
status_t calc_arithmetic_expr_by_arr(const char *expression, int *result);

#endif