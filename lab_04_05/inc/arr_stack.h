#ifndef __ARR_STACK_H__
#define __ARR_STACK_H__

#include "data.h"
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

#endif