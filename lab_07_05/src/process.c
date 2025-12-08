#include "../inc/process.h"  // TODO

status_t procces_menu_choice(menu_option_t menu_option)
{
    status_t ec = SUCCESS_CODE;

    switch (menu_option)
    {
        case EXIT_PROGRAM:
            break;

        case CREATE_BST_FROM_FILE:
            // TODO
            break;

        case ADD_BST_ELEM:
            // TODO
            break;

        case DELETE_BST_ELEM:
            // TODO
            break;

        case CLEAR_BST:
            // TODO
            break;

        case FIND_WORD_IN_BST:
            // TODO
            break;

        case BALANCE_BST:
            // TODO
            break;

        case CREATE_HST_FROM_FILE:
            // TODO
            break;

        case ADD_HST_ELEM:
            // TODO
            break;

        case DELETE_HST_ELEM:
            // TODO
            break;

        case CLEAR_HST:
            // TODO
            break;

        case CREATE_AVL_FROM_BST:
            // TODO
            break;

        case PRINT_BST:
            // TODO
            break;

        case PRINT_HST:
            // TODO
            break;

        case PRINT_AVL:
            // TODO
            break;

        case MEASURE_SEARCH_TIME:
            // TODO
            break;
        
        default:
            ec = ERR_RANGE;
            break;
    }

    return ec;
}
