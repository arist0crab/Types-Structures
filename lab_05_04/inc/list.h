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
} list_queue_t;

/** @brief Функция для добавления элемента request в конец очереди (список)
 * @param list_queue Очередь, в которую добавляется элемент
 * @param request Элемент, который будет добавлен в очередь
 * @return Статус выполнения функции
*/
status_t push_list(list_queue_t *list_queue, request_t *request);

/** @brief Функция для удаления элемента из начала очереди (список).
 * Если указан popped_request, то удаленный элемент передается в popped_request.
 * @param list_queue Очередь, из которой удаляется элемент
 * @param popped_request Указатель на элемент, куда будут передан удаляемый элемент,
 * если popped_request != NULL
 * @return Статус выполнения функции
*/
status_t pop_list(list_queue_t *list_queue, request_t *popped_request);

/** @brief Динамически выделяет память для узла списка, выделенный адрес присваивается 
 * new_node. Значение нового узла - request.
 * @param new_node Указатель на новый узел списка.
 * @param request Значение (запись), присваиваемое новому узлу.
 * @return Статус выполнения функции
*/
status_t allocate_list_node(node_t **new_node, const request_t *request);

/** @brief Освобождает выделенные узелы списка list_queue.
 * @param list_queue Список, чьи узлы будут освобождены.
 * @return Статус выполнения функции
*/
status_t destroy_list_queue(list_queue_t *list_queue);

#endif