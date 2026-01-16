#define _POSIX_C_SOURCE 199309L
#include "../inc/process.h"

int check_avl(avl_node_t *root)
{
    if (!root) return 1;
    int bf = avl_height(root->left) - avl_height(root->right);
    if (bf < -1 || bf > 1)
        return 0;
    return check_avl(root->left) && check_avl(root->right);
}

status_t process_current_option(menu_option_t current_option, bst_node_t **bst_root, avl_node_t **avl_root, hst_chaining_t **chaining_hst, hst_open_t **open_hst)
{
    status_t exit_code = SUCCESS_CODE;
    char filename[MAX_FILENAME_LENGTH];
    char *buffer = NULL;
    int comp = 0;
    bool to_del_found = false;
    size_t chaining_size = 10, open_size = 10;
    if (*chaining_hst)
        chaining_size = (*chaining_hst)->size;
    if (*open_hst)
        open_size = (*open_hst)->size;

    switch (current_option)
    {
        case EXIT:
            break;

        case MAKE_TREE_FROM_FILE:
            exit_code = input_filename(filename);
            if (exit_code == SUCCESS_CODE)
                exit_code = make_bst_from_file(bst_root, filename);
            if (exit_code == SUCCESS_CODE)
                exit_code = make_avl_from_file(avl_root, filename);
            break;

        case PRINT_BST_TREE:
            print_bst(*bst_root);
            break;

        case PRINT_AVL_TREE:
            print_avl(*avl_root);
            printf("\n");
            break;

        case SHOW_BST_TREE:
            display_bst_tree_png(*bst_root);
            break;

        case SHOW_AVL_TREE:
            display_avl_tree_png(*avl_root);
            break;

        case ADD_WORD_TO_TREE:
            exit_code = input_string(&buffer, "Введите слово: ");
            if (exit_code == SUCCESS_CODE)
                exit_code = insert_bst_node(bst_root, buffer);
            if (exit_code == SUCCESS_CODE)
                exit_code = insert_avl_node(avl_root, buffer);
            break;

        case DELETE_WORD_FROM_TREE:
            exit_code = input_string(&buffer, "Введите слово: ");
            if (exit_code == SUCCESS_CODE)
                exit_code = delete_bst_node(bst_root, buffer, &to_del_found);
            if (exit_code == SUCCESS_CODE)
                exit_code = delete_avl_node(avl_root, buffer, &to_del_found);
            if ((exit_code == SUCCESS_CODE) && !to_del_found)
                printf("Такого элемента в дереве нет.\n");
            else if (exit_code == SUCCESS_CODE)
                printf("Элемент есть в дереве.\n");
            break;

        case FIND_WORD_IN_TREE:
            if (!*chaining_hst)
                printf("Таблица ещё не создана.\n");
            else
            {
                exit_code = input_string(&buffer, "Введите слово: ");
                if (exit_code == SUCCESS_CODE)
                    exit_code = find_word_in_bst(bst_root, buffer, 1, &comp);
                printf("Для поиска элемента в bst дереве было проведено %d сравнений.\n", comp);
                if (exit_code == SUCCESS_CODE)
                    exit_code = find_word_in_avl(avl_root, buffer, 1, &comp);
                printf("Для поиска элемента в avl дереве было проведено %d сравнений.\n", comp);
            }
            break;

        case MAKE_HASH_TABLE_FROM_FILE:
            exit_code = input_filename(filename);
            if (exit_code == SUCCESS_CODE)
                input_size(&chaining_size);
            if (exit_code == SUCCESS_CODE)
            {
                open_size = chaining_size;
                *chaining_hst = create_hash_table_chaining(chaining_size);
                *open_hst = create_hash_table_open(open_size);
                build_hash_table_from_file_chaining(*chaining_hst, filename);
                build_hash_table_from_file_open(*open_hst, filename);
            }
            break;

        case PRINT_CHAINING_HASH_TABLE:
            print_chaining_hash_table(*chaining_hst);
            break;

        case PRINT_OPEN_HASH_TABLE:
            print_open_hash_table(*open_hst);
            break;

        case SHOW_CHAINING_HASH_TABLE:
            display_hash_table_chaining_png(*chaining_hst);
            break;

        case SHOW_OPEN_HASH_TABLE:
            display_hash_table_open_png(*open_hst);
            break;

        case ADD_WORD_TO_HASH_TABLE:
            exit_code = input_string(&buffer, "Введите слово: ");
            if (exit_code == SUCCESS_CODE)
            {
                hash_table_insert_chaining(*chaining_hst, buffer);
                hash_table_insert_open(*open_hst, buffer);
            }
            break;

        case DELETE_WORD_FROM_HASH_TABLE:
            exit_code = input_string(&buffer, "Введите слово: ");
            if (exit_code == SUCCESS_CODE && (*chaining_hst)->count && (*open_hst)->count)
            {
                hash_table_delete_chaining(*chaining_hst, buffer);
                hash_table_delete_open(*open_hst, buffer);
            }
            else if (exit_code == SUCCESS_CODE)
                printf(RED_BOLD "Таблица пуста, нечего удалять.\n" RESET);
            break;

        case FIND_WORD_IN_HASH_TABLE:
            exit_code = input_string(&buffer, "Введите слово: ");
            if (exit_code == SUCCESS_CODE)
            {
                int comparisons = 0;
                hash_node_t *check_node = hash_table_search_chaining(*chaining_hst, buffer, &comparisons);
                if (!check_node)
                    printf("Такой элемент не найден.\n");
                printf("Для поиска элемента в хеш-табличке с цепочками было проведено %d сравнений.\n", comparisons);
                hash_table_search_open(*open_hst, buffer, &comparisons);
                printf("Для поиска элемента в хеш-табличке с открытой адресацией было проведено %d сравнений.\n", comparisons);
                
            }
            break;

        case COMPARE_EFFICIENCY:
            if (!bst_root || !avl_root || !chaining_hst || !open_hst)
            {
                printf("Не все структуры данных построены! Сначала загрузите данные.\n");
                break;
            }
            {
                comparison_results_t results =
                    compare_structures(*bst_root, *avl_root, *chaining_hst, *open_hst);
                print_comparison_results(&results);
            }
            break;

        default:
            break;
    }

    return exit_code;
}