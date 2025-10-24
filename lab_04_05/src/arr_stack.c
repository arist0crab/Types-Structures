#include "arr_stack.h"

status_t push_arr_stack(arr_stack_t *arr_stack, int value);
status_t pop_arr_stack(arr_stack_t *arr_stack, int *popped_value);
status_t do_operation(int op1, int op2, char operator, int *result);

/** @brief Выводит на экран (в терминал) стек (массив).
*/
status_t print_arr_stack(arr_stack_t *arr_stack)
{
    status_t ec = (arr_stack) ? SUCCESS_CODE : ERR_INVALID_POINTER;

    if (ec == SUCCESS_CODE && arr_stack->current_size == 0)
        printf("%s============ ТЕКУЩИЙ СТЕК ПУСТ ============%s\n", BLUE_BOLD, RESET);
    else if (ec == SUCCESS_CODE)
    {
        printf("%s============ ТЕКУЩИЙ СТЕК ============%s\n", BLUE_BOLD, RESET);
        printf("Размер стека: %lu\n", arr_stack->current_size);
        for (size_t i = 0; i < arr_stack->current_size; i++)
            printf("%d ", arr_stack->data[i]);
        printf("\n");
    }

    return ec;
}       

/**
 * @brief Добавляет элемент в конец стека (массив). Возвращает код завершения
 * функции. Если добавить элемент невозможно ввиду переполнения стека, возвращается
 * соответствующая ошибка.
*/
status_t push_arr_stack(arr_stack_t *arr_stack, int value)
{
    status_t ec = SUCCESS_CODE;

    if (arr_stack == NULL)
        ec = ERR_INVALID_POINTER;
    else if (arr_stack->current_size == MAX_ARR_STACK_SIZE)
        ec = ERR_OVERFLOW;
    else 
        arr_stack->data[arr_stack->current_size++] = value;

    return ec;
}


/** @brief Удаляет элемент из конца стека (массива). Если указан 
 * popped_value, то значение, которое будет удалено из стека, присваивается
 * данному указателю. Если popped_value == NULL, тогда удаляемое значение
 * не присваивается. 
*/
status_t pop_arr_stack(arr_stack_t *arr_stack, int *popped_value)
{
    status_t ec = SUCCESS_CODE;

    if (arr_stack == NULL)
        ec = ERR_INVALID_POINTER;
    else if (arr_stack->current_size == 0)
        ec = ERR_POP_EMPTY;
    else 
        arr_stack->current_size--;

    if (popped_value)
        *popped_value = arr_stack->data[arr_stack->current_size];;

    return ec;
}


/** @brief Вычисляет операцию между двумя операндами. Результат записывает 
 * в result. Возвращает статус своего завершения.
*/
status_t do_operation(int op1, int op2, char operator, int *result)
{
    status_t ec = (result) ? SUCCESS_CODE : ERR_INVALID_POINTER;

    switch (operator)
    {
        case '+':
            *result = op1 + op2;
            break;
        case '-':
            *result = op1 - op2;
            break;
        case '*':
            *result = op1 * op2;
            break;
        case '/':
            if (op2 == 0)
                ec = ERR_ZERO_DIVISION;
            else 
                *result = op1 / op2;
            break;
        
        default:
            ec = ERR_RANGE;
            break;
    }

    return ec;
}