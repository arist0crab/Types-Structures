#include "list_stack.h"

status_t create_node(node_t **new_node, int value);
status_t execute_list_stack_operation(list_stack_t *operand_stack, list_stack_t *operator_stack, free_blocks_array_t *free_blocks);
status_t handle_char_for_list_stack(list_stack_t *operand_stack, list_stack_t *operator_stack, char ch, int *reading_number, int *number, free_blocks_array_t *free_blocks);

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

    if (ec == SUCCESS_CODE)
    {
        new_node->next = list_stack->stack_pointer;  // указывает на прежнюю вершину (stack_pointer)
        list_stack->stack_pointer = new_node;  // вершина обновляется
        list_stack->curr_size++;
    }

    return ec;
}

/** @brief Функция для освобождения из памяти всех элементов списка.
 * Возвращает статус своего выполнения.
*/
status_t free_list_stack(list_stack_t *list_stack)
{
    status_t ec = (list_stack) ? SUCCESS_CODE : ERR_INVALID_POINTER;
    node_t *cur_node = list_stack->stack_pointer;
    node_t *next_node = NULL;

    while (ec == SUCCESS_CODE && cur_node)
    {
        next_node = cur_node->next;
        free(cur_node);
        cur_node = next_node;
    }

    list_stack->curr_size = 0;

    return ec;    
}

/** @brief Удаляет элемент из конца стека (список). Если указан 
 * popped_value, то значение, которое будет удалено из стека, присваивается
 * данному указателю. Если popped_value == NULL, тогда удаляемое значение
 * не присваивается. 
*/
status_t pop_list_stack(list_stack_t *list_stack, int *value, free_blocks_array_t *free_blocks)
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
    {
        add_to_free_blocks_array(free_blocks, (void *)tmp_node);
        free(tmp_node);
    }

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

/** @brief Главная функция вычисления выражения.
 */
status_t calc_arithmetic_expr_by_list(const char *expression, int *result, free_blocks_array_t *free_blocks)
{
    status_t ec = (expression && result) ? SUCCESS_CODE : ERR_INVALID_POINTER;
    list_stack_t operand_stack = { NULL, 0 };
    list_stack_t operator_stack = { NULL, 0 };
    int number = 0, reading_number = 0, final_res = 0;
    int len = strlen(expression);
    char ch;

    if (ec == SUCCESS_CODE && len == 0)
        ec = ERR_RANGE;

    for (int i = 0; ec == SUCCESS_CODE && i <= len; i++)
    {
        ch = expression[i];
        ec = handle_char_for_list_stack(&operand_stack, &operator_stack, ch, &reading_number, &number, free_blocks);
    }

    if (ec == SUCCESS_CODE)
    {
        ec = pop_list_stack(&operand_stack, &final_res, free_blocks);
        if (ec == SUCCESS_CODE)
            *result = final_res;
    }

    if (operand_stack.stack_pointer)
        free_list_stack(&operand_stack);
    if (operator_stack.stack_pointer)
        free_list_stack(&operator_stack);

    return ec;
}

/** @brief Выполняет одну операцию из стека.
 */
status_t execute_list_stack_operation(list_stack_t *operand_stack, list_stack_t *operator_stack, free_blocks_array_t *free_blocks)
{
    status_t ec = SUCCESS_CODE;
    int op1, op2, res, op_temp;
    char op;

    ec = pop_list_stack(operand_stack, &op2, free_blocks);
    
    if (ec == SUCCESS_CODE)
        ec = pop_list_stack(operand_stack, &op1, free_blocks);

    if (ec == SUCCESS_CODE)
        ec = pop_list_stack(operator_stack, &op_temp, free_blocks);

    if (ec == SUCCESS_CODE)
    {
        op = (char)op_temp;
        ec = do_operation(op1, op2, op, &res);
    }

    if (ec == SUCCESS_CODE)
        ec = push_list_stack(operand_stack, res);

    return ec;
}

/** @brief Обрабатывает один символ строки выражения.
 */
status_t handle_char_for_list_stack(list_stack_t *operand_stack, list_stack_t *operator_stack, char ch, int *reading_number, int *number, free_blocks_array_t *free_blocks)
{
    status_t ec = SUCCESS_CODE;

    if (isdigit(ch))
    {
        *number = (*number * 10) + (ch - '0');
        *reading_number = 1;
    }
    else if (is_operator(ch) || ch == '\0')
    {
        if (*reading_number)
        {
            ec = push_list_stack(operand_stack, *number);
            *number = 0;
            *reading_number = 0;
        }

        if (ec == SUCCESS_CODE && operator_stack->curr_size > 0)
            ec = execute_list_stack_operation(operand_stack, operator_stack, free_blocks);

        if (ec == SUCCESS_CODE && ch != '\0')
            ec = push_list_stack(operator_stack, ch);
    }
    else if (ch != ' ')
    {
        ec = ERR_RANGE;
    }

    return ec;
}