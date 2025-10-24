#include "list_stack.h"

status_t create_node(node_t **new_node, int value);

/** @brief Выводит на экран (в терминал) стек (список).
*/
status_t print_list_stack(list_stack_t *list_stack)
{
    status_t ec = (list_stack) ? SUCCESS_CODE : ERR_INVALID_POINTER;
    node_t *cur_node = list_stack->stack_pointer;
    unsigned long occupied_memory = 0;
    size_t i = 0;

    if (ec == SUCCESS_CODE && cur_node == NULL)
        printf("%s============ ТЕКУЩИЙ СТЕК ПУСТ ============%s\n", BLUE_BOLD, RESET);
    else if (ec == SUCCESS_CODE)
    {
        printf("%s============ ТЕКУЩИЙ СТЕК ============%s\n", BLUE_BOLD, RESET);
        while (cur_node)
        {
            printf("%sЭлемент списка №%lu: %d (%p)\n%s", BLUE, i, cur_node->value, (void *)cur_node, RESET);
            cur_node = cur_node->next;
            i++;
        }
        occupied_memory = i * sizeof(node_t) + sizeof(list_stack_t);
        printf("%sДлина данного стека (списка): %lu%s\n", BLUE, i, RESET);
        printf("%sЗанимаемая память стека (списка): %lu байт(-ов)%s\n", BLUE, occupied_memory, RESET);
        // TODO освобожденные ячейки памяти
    }

    return ec;
}

/**
 * @brief Добавляет элемент в конец стека (список). Возвращает код завершения
 * функции. Если добавить элемент невозможно ввиду переполнения стека, возвращается
 * соответствующая ошибка.
*/
status_t push_list_stack(list_stack_t *list_stack, int value)
{
    status_t ec = (list_stack) ? SUCCESS_CODE : ERR_INVALID_POINTER;
    node_t *new_node = { 0 };

    if (ec == SUCCESS_CODE)
        ec = create_node(&new_node, value);

    if (ec == SUCCESS_CODE && list_stack->curr_size == list_stack->max_size)
        list_stack->curr_size *= 2;

    if (ec == SUCCESS_CODE)
    {
        new_node->next = list_stack->stack_pointer;  // указывает на прежнюю вершину (stack_pointer)
        list_stack->stack_pointer = new_node;  // вершина обновляется
        list_stack->curr_size++;
    }

    return ec;
}

/** @brief Удаляет элемент из конца стека (список). Если указан 
 * popped_value, то значение, которое будет удалено из стека, присваивается
 * данному указателю. Если popped_value == NULL, тогда удаляемое значение
 * не присваивается. 
*/
status_t pop_list_stack(list_stack_t *list_stack, int *value)
{
    status_t ec = (list_stack) ? SUCCESS_CODE : ERR_INVALID_POINTER;
    node_t *tmp_node = NULL;

    if (list_stack->stack_pointer == NULL)
        ec = ERR_POP_EMPTY;

    if (ec == SUCCESS_CODE)
    {
        tmp_node = list_stack->stack_pointer;
        list_stack->stack_pointer = list_stack->stack_pointer->next;
        list_stack->curr_size--;
    }

    if (ec == SUCCESS_CODE && value)
        *value = tmp_node->value;

    if (ec == SUCCESS_CODE && tmp_node)
        free(tmp_node);

    return ec;
}

/** @brief Создает новый узел списка. 
 * 
 * Динамически выделяет новый узел списка, указатель на него возвращает с помощью 
 * new_node. Новому узлу присваивается значение value. Функция возвращает статус
 * своего выполнения.
*/
status_t create_node(node_t **new_node, int value)
{
    status_t ec = (new_node) ? SUCCESS_CODE : ERR_INVALID_POINTER;

    if (ec == SUCCESS_CODE)
    {
        *new_node = malloc(sizeof(node_t));
        if (*new_node == NULL)
            ec = ERR_MEM;
    }

    if (ec == SUCCESS_CODE)
    {
        (*new_node)->value = value;
        (*new_node)->next = NULL;
    }

    return ec;
}