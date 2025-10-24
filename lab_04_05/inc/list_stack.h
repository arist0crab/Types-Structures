#ifndef __LIST_STACK_H__
#define __LIST_STACK_H__

#include "data.h"
#include "color.h"
#include "input.h"
#include <stdlib.h>

typedef struct node
{
    int value;  // значение узла
    struct node *next; // указатель на следующий узел
} node_t;


typedef struct 
{
    node_t *stack_pointer;  // указатель на вершину стека
    int curr_size;  // текущий размер стека
    int max_size;  // максимальный размер стека
} list_stack_t;

status_t print_list_stack(list_stack_t *list_stack);
status_t push_list_stack(list_stack_t *list_stack, int value);
status_t pop_list_stack(list_stack_t *list_stack, int *value);

#endif