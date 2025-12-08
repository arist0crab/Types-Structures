#include "../inc/process.h"  // TODO

status_t procces_menu_choice(menu_option_t menu_option, bst_node_t **bst_root)
{
    status_t ec = SUCCESS_CODE;
    char *filename = NULL;

    switch (menu_option)
    {
        case EXIT_PROGRAM:
            break;

        case CREATE_BST_FROM_FILE:
            ec = input_string(&filename, "Введите имя файла: ");
            if (ec == SUCCESS_CODE)
                ec = read_tree_from_file(bst_root, filename);
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
            ec = print_pretty_bst(*bst_root);
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
