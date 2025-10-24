#include "process.h"

status_t procces_menu_choice(int option, arr_stack_t *arr_stack, list_stack_t *list_stack, free_blocks_array_t *free_blocks)
{
    status_t ec = SUCCESS_CODE;
    char expression[MAX_EXPRESSION_SIZE];
    int stack_elem = 0;

    if (arr_stack == NULL)
        return ERR_INVALID_POINTER;

    switch (option)
    {
        case EXIT_PROGRAM:
            break;

        case CALC_ARITHMETIC_EXPRESSION_BY_ARRAY:
            ec = get_expression(expression);
            if (ec == SUCCESS_CODE)
                ec = calc_arithmetic_expr_by_arr(expression, &stack_elem);
            if (ec == SUCCESS_CODE)
                print_calc_result(stack_elem);
            break;

        case ADD_STACK_ELEMENT_TO_ARRAY:
            ec = get_stack_elem(&stack_elem);
            if (ec == SUCCESS_CODE)
                ec = push_arr_stack(arr_stack, stack_elem);
            break;

        case DELETE_STACK_ELEMENT_FROM_ARRAY:
            ec = pop_arr_stack(arr_stack, &stack_elem);
            if (ec == SUCCESS_CODE)
                print_pop_result(stack_elem);
            break;

        case PRINT_STACK_FROM_ARRAY:
            ec = print_arr_stack(arr_stack);
            break;

        case CALC_ARITHMETIC_EXPRESSION_BY_LIST:
            ec = get_expression(expression);
            if (ec == SUCCESS_CODE)
                ec = calc_arithmetic_expr_by_list(expression, &stack_elem, free_blocks);
            if (ec == SUCCESS_CODE)
                print_calc_result(stack_elem);
            break;

        case ADD_STACK_ELEMENT_TO_LIST:
            ec = get_stack_elem(&stack_elem);
            if (ec == SUCCESS_CODE)
                ec = push_list_stack(list_stack, stack_elem);
            break;

        case DELETE_STACK_ELEMENT_FROM_LIST:
            ec = pop_list_stack(list_stack, &stack_elem, free_blocks);
            if (ec == SUCCESS_CODE)
                print_pop_result(stack_elem);
            break;

        case PRINT_STACK_FROM_LIST:
            ec = print_list_stack(list_stack);
            break;

        case PRINT_FREE_BLOCKS_ARRAY:
            ec = print_free_blocks(free_blocks);
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