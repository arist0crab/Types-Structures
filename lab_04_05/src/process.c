#include "process.h"

status_t procces_menu_choice(int option)
{
    status_t ec = SUCCESS_CODE;

    switch (option)
    {
        case EXIT_PROGRAM:
            break;

        case CALC_ARITHMETIC_EXPRESSION_BY_ARRAY:
            break;

        case ADD_STACK_ELEMENT_TO_ARRAY:
            break;

        case DELETE_STACK_ELEMENT_FROM_ARRAY:
            break;

        case PRINT_STACK_FROM_ARRAY:
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