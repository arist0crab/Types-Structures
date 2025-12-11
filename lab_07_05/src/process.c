#include "../inc/process.h"  // TODO

status_t procces_menu_choice(menu_option_t menu_option, bst_node_t **bst_root, hash_table_t **hst)
{
    status_t ec = SUCCESS_CODE;
    bst_node_t *temp_bst_node = NULL;
    int temp_menu_opt = 0;
    char *filename = NULL;
    char *word = NULL;

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
            ec = input_string(&word, "Введите слово: ");
            if (ec == SUCCESS_CODE)
                ec = insert_bst_node(bst_root, word);
            break;

        case DELETE_BST_ELEM:
            ec = input_string(&word, "Введите слово: ");
            if (ec == SUCCESS_CODE)
                ec = delete_bst_node(bst_root, word);
            break;

        case CLEAR_BST:
            ec = clear_bst(bst_root);
            break;

        case FIND_WORD_IN_BST:
            ec = input_string(&word, "Введите слово: ");
            if (ec == SUCCESS_CODE)
                ec = find_word_in_bst(*bst_root, &temp_bst_node, (const char *)word);
            break;

        case BALANCE_BST:
            ec = balance_tree(bst_root);
            break;

        case CREATE_HST_FROM_FILE:
            if (*hst == NULL) 
                ec = user_init_table(hst);
            if (ec == SUCCESS_CODE)
                ec = input_string(&filename, "Введите имя файла: ");
            if (ec == SUCCESS_CODE)
                ec = build_hash_from_file(hst, filename);
            break;

        case ADD_HST_ELEM:
            if (*hst == NULL) 
                ec = user_init_table(hst);
            if (ec == SUCCESS_CODE)
                ec = input_string(&word, "Введите слово: ");
            if (ec == SUCCESS_CODE)
                ec = insert_hst_node(*hst, (const char *)word);
            break;

        case DELETE_HST_ELEM:
            if (*hst == NULL)
                ec = ERR_TABLE_DOESNT_EXIST;
            if (ec == SUCCESS_CODE)
                ec = input_string(&word, "Введите слово для удаления: ");
            if (ec == SUCCESS_CODE)
                ec = delete_hst_node(hst, (const char *)word);
            break;

        case CLEAR_HST:
            ec = clear_hst_table(hst);
            break;

        case RESIZE_HST:
            print_hst_resize_warning();
            ec = input_cur_menu_opt(&temp_menu_opt, 2);
            if (ec == SUCCESS_CODE && temp_menu_opt == 0)
                ec = resize_hst_table(hst);
            break;

        case PRINT_BST:
            ec = print_pretty_bst(*bst_root);
            break;

        case PRINT_HST:
            ec = print_hash_table((const hash_table_t *)*hst);
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
