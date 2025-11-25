#include "../inc/process.h"

status_t procces_menu_choice(menu_option_t menu_option, tree_node_t **root)
{
    status_t ec = SUCCESS_CODE;
    char *target_word = NULL;
    tree_node_t *target_root = NULL;

    switch (menu_option)
    {
        case EXIT_PROGRAM:
            break;

        case READ_TREE_FROM_FILE:
            ec = input_string(&target_word, "Введите имя файла: ");
            if (ec == SUCCESS_CODE)
                ec = read_tree_from_file(root, target_word);
            break;

        case PRINT_TREE:
            ec = print_tree(*root, 0);
            break;

        case ADD_TREE_NODE:
            ec = input_string(&target_word, "Введите слово: ");
            if (ec == SUCCESS_CODE)
                ec = insert_tree_node(root, target_word);
            break;

        case DELETE_TREE_NODE:
            ec = input_string(&target_word, "Введите слово: ");
            if (ec == SUCCESS_CODE)
                ec = delete_tree_node(root, target_word);
            break;

        case FIND_WORD_IN_FILE:
            break;

        case FIND_WORD_IN_TREE:
            ec = input_string(&target_word, "Введите слово для поиска: ");
            if (ec == SUCCESS_CODE)
            {
                ec = find_word_in_tree(*root, &target_root, target_word);
                if (ec == SUCCESS_CODE)
                    print_find_result((const tree_node_t **)&target_root);
            }

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