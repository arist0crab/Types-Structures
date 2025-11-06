#include "list.h"

status_t push_list(list_queue_t *list_queue, request_t *request)
{
    node_t *cur_node;

    if (list_queue == NULL || request == NULL)
        return ERR_INVALID_POINTER;

    if (list_queue->curr_size == list_queue->max_size)
        return ERR_OVERFLOW;

    allocate_list_node(&cur_node, request);
    cur_node->next = NULL;
    if (list_queue->curr_size == 0)
        list_queue->pin = list_queue->pout = cur_node;
    else 
    {
        list_queue->pout->next = cur_node;
        list_queue->pout = cur_node;
    }
    list_queue->curr_size++;

    return SUCCESS_CODE;
}

status_t pop_list(list_queue_t *list_queue, request_t *popped_request)
{
    if (list_queue == NULL)
        return ERR_INVALID_POINTER;
    if (list_queue->curr_size == 0)
        return ERR_POP_EMPTY;

    node_t *old_head = list_queue->pin; 
    if (popped_request)
        *popped_request = old_head->request;

    list_queue->pin = old_head->next;
    if (list_queue->pin == NULL)
        list_queue->pout = NULL;

    free(old_head);
    list_queue->curr_size--;

    return SUCCESS_CODE;
}

status_t allocate_list_node(node_t **new_node, const request_t *request)
{
    if (new_node == NULL || request == NULL)
        return ERR_INVALID_POINTER;

    *new_node = malloc(sizeof(node_t));
    if (*new_node == NULL)
        return ERR_MEM;

    (*new_node)->request = *request;
    (*new_node)->next = NULL;

    return SUCCESS_CODE;
}

status_t destroy_list_queue(list_queue_t *list_queue)
{
    if (list_queue == NULL)
        return ERR_INVALID_POINTER;

    node_t *next_node;
    while (list_queue->pin)
    {
        next_node = list_queue->pin->next;
        free(list_queue->pin);
        list_queue->pin = next_node;
    }

    list_queue->pout = NULL;
    list_queue->curr_size = 0;

    return SUCCESS_CODE;
}

status_t print_list(list_queue_t *list_queue)
{
    if (list_queue == NULL)
        return ERR_INVALID_POINTER;

    node_t *cur = list_queue->pin;

    printf("Очередь (список):\n");
    for (size_t i = 0; cur != NULL; cur = cur->next, i++)
        printf("заявка №%lu / тип заявки %d / е.в. %lf / адрес %p\n", i + 1, cur->request.request_class + 1, cur->request.arrival_time, (void *)cur);

    return SUCCESS_CODE;
}
