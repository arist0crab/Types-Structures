#ifndef __ARR_H__
#define __ARR_H__

#include "data.h"
#include <stdio.h>

// TODO в случае переполнение очереди на массиве прекратить симмуляцию

typedef struct 
{
    request_t data[MAX_QUEUE_SIZE];
    size_t pin;  // индекс для добавления (хвост)
    size_t pout;  // индекс для извлечения (голова)
    size_t size;  // текущее количество элементов
    size_t total_enqueued;  // было добавлено элементов
    size_t total_dequeued;  // было извлечено
} arr_queue_t;


/** @brief Функция для добавления элемента request в конец очереди (статический массив)
 * @param arr_queue Очередь, в которую добавляется элемент
 * @param request Элемент, который будет добавлен в очередь
 * @return Статус выполнения функции
*/
status_t push_arr(arr_queue_t *arr_queue, request_t *request);

/** @brief Функция для удаления элемента из начала очереди (статический масссив).
 * Если указан popped_request, то удаленный элемент передается в popped_request.
 * @param arr_queue Очередь, из которой удаляется элемент
 * @param popped_request Указатель на элемент, куда будут передан удаляемый элемент,
 * если popped_request != NULL
 * @return Статус выполнения функции
*/
status_t pop_arr(arr_queue_t *arr_queue, request_t *popped_request);


/** @brief Функция для печати очереди (статический массив).
 * @param arr_queue Очередь, которая выводится на экран (в терминал)
 * @return Статус выполнения функции
*/
status_t print_arr(arr_queue_t *arr_queue);

#endif