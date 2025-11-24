#include "../inc/process.h"

status_t procces_menu_choice(menu_option_t menu_option, tree_node_t **root)
{
    status_t ec = SUCCESS_CODE;
    char *target_word = NULL;

    switch (menu_option)
    {
        case EXIT_PROGRAM:
            break;

        case READ_FILE_AND_CREATE_TREE:
            break;

        case PRINT_TREE:
            ec = print_tree(*root, 0);
            break;

        case ADD_TREE_NODE:
            ec = input_word(&target_word);
            if (ec == SUCCESS_CODE)
                ec = insert_tree_node(root, target_word);
            break;

        case DELETE_TREE_NODE:
            ec = input_word(&target_word);
            if (ec == SUCCESS_CODE)
                ec = delete_tree_node(root, target_word);
            break;

        case FIND_WORD_IN_FILE:
            break;

        case FIND_WORD_IN_TREE:
            break;

        case COMPARE_FIND_TIME:
            break;;
        
        default:
            ec = ERR_RANGE;
            break;
    }

    if (target_word) free(target_word);

    return ec;
}