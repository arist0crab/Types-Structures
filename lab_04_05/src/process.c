#include "process.h"

status_t procces_menu_choice(int option, arr_stack_t *arr_stack)
{
    status_t ec = SUCCESS_CODE;
    int stack_elem;

    if (arr_stack == NULL)
        return ERR_INVALID_POINTER;

    switch (option)
    {
        case EXIT_PROGRAM:
            break;

        case CALC_ARITHMETIC_EXPRESSION_BY_ARRAY:
            break;

        case ADD_STACK_ELEMENT_TO_ARRAY:
            ec = get_stack_elem(&stack_elem);
            if (ec == SUCCESS_CODE)
                ec = push_arr_stack(arr_stack, stack_elem);
            break;

        case DELETE_STACK_ELEMENT_FROM_ARRAY:
            break;

        case PRINT_STACK_FROM_ARRAY:
            ec = print_arr_stack(arr_stack);
            break;

        case CALC_ARITHMETIC_EXPRESSION_BY_LIST:
            break;

        case ADD_STACK_ELEMENT_TO_LIST:
            break;

        case DELETE_STACK_ELEMENT_FROM_LIST:
            break;

        case PRINT_STACK_FROM_LIST:
            break;

        case COMPARE_PERFORMANCE:
            break;

        case PRINT_ARRAY_OF_FREE_MEMORY_AREAS:
            break;
        
        default:
            ec = ERR_RANGE;
            break;
    }

    return ec;
}