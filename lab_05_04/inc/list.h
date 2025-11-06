#ifndef __LIST_H__
#define __LIST_H__

#include "data.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    request_t request;
    struct node *next;
} node_t;

typedef struct 
{
    node_t *pin; 
    node_t *pout; 
    size_t curr_size;
    size_t max_size;
} list_queue_t;

// TODO написать документацию
status_t pop_list(list_queue_t *list_queue, request_t *popped_request);
// TODO написать документацию
status_t push_list(list_queue_t *list_queue, request_t *request);
// TODO написать документацию
status_t allocate_list_node(node_t **new_node, const request_t *request);
// TODO написать документацию
status_t destroy_list_queue(list_queue_t *list_queue);

#endif