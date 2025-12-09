#include "arr.h"


status_t push_arr(arr_queue_t *arr_queue, request_t *request)
{
    if (arr_queue == NULL || request == NULL)
        return ERR_INVALID_POINTER;
    if (arr_queue->size >= MAX_QUEUE_SIZE)
        return ERR_OVERFLOW;

    arr_queue->data[arr_queue->pin] = *request;
    arr_queue->pin = (arr_queue->pin + 1) % MAX_QUEUE_SIZE;
    arr_queue->size++;
    arr_queue->total_enqueued++;

    return SUCCESS_CODE;
}

status_t pop_arr(arr_queue_t *arr_queue, request_t *popped_request)
{
    if (arr_queue == NULL)
        return ERR_INVALID_POINTER;
    if (arr_queue->size == 0)
        return ERR_POP_EMPTY;

    if (popped_request)
        *popped_request = arr_queue->data[arr_queue->pout];

    arr_queue->pout = (arr_queue->pout + 1) % MAX_QUEUE_SIZE;
    arr_queue->size--;
    arr_queue->total_dequeued++;

    return SUCCESS_CODE;
}

status_t print_arr(arr_queue_t *arr_queue)
{
    if (arr_queue == NULL)
        return ERR_INVALID_POINTER;

    printf("Очередь (статический массив):\n");
    for (size_t i = 0; i < arr_queue->size; i++)
        printf("заявка №%lu / тип заявки %d / е.в. %lf / адрес %p\n", i + 1, arr_queue->data[i].request_class + 1, arr_queue->data[i].arrival_time, (void *)(arr_queue->data + i));
    
    return SUCCESS_CODE;
}   