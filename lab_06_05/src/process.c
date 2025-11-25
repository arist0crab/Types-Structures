#include "../inc/process.h"

status_t find_in_file_general_process(ssize_t *file_word_index);
status_t find_in_tree_general_process(tree_node_t *root);

status_t procces_menu_choice(menu_option_t menu_option, tree_node_t **root)
{
    status_t ec = SUCCESS_CODE;
    char *target_word = NULL, *filename = NULL;
    ssize_t file_word_index = -1;

    switch (menu_option)
    {
        case EXIT_PROGRAM:
            break;

        case READ_TREE_FROM_FILE:
            ec = input_string(&filename, "Введите имя файла: ");
            if (ec == SUCCESS_CODE)
                ec = read_tree_from_file(root, filename);
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
            ec = find_in_file_general_process(&file_word_index);
            break;

        case FIND_WORD_IN_TREE:
            ec = find_in_tree_general_process(*root);
            break;

        case COMPARE_FIND_TIME:
            break;
        
        default:
            ec = ERR_RANGE;
            break;
    }

    if (target_word) free(target_word);

    return ec;
}

status_t find_in_file_general_process(ssize_t *file_word_index)
{
    status_t ec = SUCCESS_CODE;
    char *target_word = NULL, *filename = NULL;

    if (!file_word_index)
        ec = SUCCESS_CODE;

    if (ec == SUCCESS_CODE)
    {
        ec = input_string(&target_word, "Введите слово для поиска: ");
        if (ec == SUCCESS_CODE)
            ec = input_string(&filename, "Введите имя файла: ");
    }

    if (ec == SUCCESS_CODE)
        ec = find_word_in_file(filename, target_word, file_word_index);

    if (ec == SUCCESS_CODE && *file_word_index >= 0)
        printf("%sСлово было успешно найдено, индекс слова: %lu\n%s", BLUE, *file_word_index, RESET);
    else if (ec == SUCCESS_CODE)
    {
        printf("%sСлово не было найдено в файле\n%s", BLUE, RESET);
        // TODO реализовать добавление по желанию пользователя
    }
    
    return ec;
}

status_t find_in_tree_general_process(tree_node_t *root)
{
    status_t ec = SUCCESS_CODE;
    tree_node_t *target_root = NULL;
    char *target_word = NULL;

    if (!root || !target_root)
        ec = ERR_ARGS;

    ec = input_string(&target_word, "Введите слово для поиска: ");
    if (ec == SUCCESS_CODE)
    {
        target_root = NULL;
        ec = find_word_in_tree(root, &target_root, target_word);
    }

    if (ec == SUCCESS_CODE && target_root)
        printf("%sСлово было успешно найдено: %s (%lu)\n%s", BLUE, target_root->word, target_root->counted, RESET);
    else if (ec == SUCCESS_CODE && !target_root)
    {
        printf("%sСлово не было найдено\n%s", BLUE, RESET);
        // TODO реализовать добавление по желанию пользователя
    }

    return ec;
}