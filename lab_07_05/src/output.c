#include "../inc/output.h"  // TODO

status_t print_branch(bst_node_t *node, char* prefix, int is_tail, char *color);


status_t print_menu(void)
{
    // TODO сделать таблицу более читаемой
    printf("%s", BLUE);
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║                   M E N U                    ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║ 0. Выход                                     ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║          РАБОТА С БИНАРНЫМ ДЕРЕВОМ           ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║ 1. Считать бинарное дерево из файла          ║\n");
    printf("║ 2. Добавить узел дерева                      ║\n");
    printf("║ 3. Удалить узел дерева                       ║\n");
    printf("║ 4. Очистить все дерево                       ║\n");
    printf("║ 5. Найти слово в дереве                      ║\n");
    printf("║ 6. Сбалансировать дерево                     ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║             РАБОТА С ХЭШ-ТАБЛИЦЕЙ            ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║ 7. Считать хэш-таблицу из файла              ║\n");
    printf("║ 8. Добавить элемент в хэш-таблицу            ║\n");
    printf("║ 9. Удалить элемент из хэш-таблицы            ║\n");
    printf("║ 10. Очистить хэш-таблицу                     ║\n");
    printf("║ 11. Изменить размер таблицы                  ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║       РАБОТА СО СБАЛАНСИРОВАННЫМ ДЕРЕВОМ     ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║ 12. Создать сбалансированное дерево          ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║           ПЕЧАТЬ ДЕЕРВЬЕВ И ТАБЛИЦ           ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║ 13. Вывести бинарное дерево                  ║\n");
    printf("║ 14. Вывести хэш-таблицу                      ║\n");
    printf("║ 15. Вывести сбалансированное дерево          ║\n");    
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║                  ЭФФЕКТИВНОСТЬ               ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║ 16. Сравнить эффективность                   ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    printf("%s", RESET);

    return SUCCESS_CODE;
}

status_t print_exit_code_result(status_t exit_code)
{
    status_t ec = SUCCESS_CODE;

    switch (exit_code)
    {
        case SUCCESS_CODE:
            printf("%sВсе прошло хорошо :)\n%s", GREEN_BOLD, RESET);
            break;

        case ERR_IO:
            printf("%sПроизошла ошибка ввода/вывода :(\n%s", RED_BOLD, RESET);
            break;

        case ERR_MEM:
            printf("%sПроизошла ошибка при работе с памятью :(\n%s", RED_BOLD, RESET);
            break;

        case ERR_ARGS:
            printf("%sПроизошла ошибка при работе с аргументами функии%s\n", RED_BOLD, RESET);
            break;

        case ERR_FILE:
            printf("%sПроизошла ошибка при работе с файлом\n%s", RED_BOLD, RESET);
            break;

        case ERR_RANGE:
            printf("%sПроизошла ошибка обработки массива или обработки количества чего-либо :(\n%s", RED_BOLD, RESET);
            break; 
            
        case ERR_NOT_FOUND:
            printf("%sПо вашему запросу ничего не найдено :(%s\n", RED_BOLD, RESET);
            break;

        case ERR_ALREADY_EXISTS:
            printf("%sСлово, которое вы пытаетесь добавить, уже существует%s\n", RED_BOLD, RESET);
            break;

        case ERR_INVALID_POINTER:
            printf("%sПроизошла ошибка при работе с указателем\n%s", RED_BOLD, RESET);
            break;

        case ERR_TABLE_DOESNT_EXIST:
            printf("%sТаблицы пока не существует, думаю, ее придется создать%s\n", RED_BOLD, RESET);
            break;

        default:
            ec = ERR_RANGE;
            break;
    }

    return ec;
}

status_t print_insert_menu(char *target_word)
{
    status_t ec = SUCCESS_CODE;

    if (!target_word)
        ec = ERR_ARGS;

    printf("%sХотели бы вы добавить слово '%s' в дерево и файл?%s\n", BLUE, target_word, RESET);
    printf("%s0 - да%s\n", BLUE, RESET);
    printf("%s1 - нет%s\n", BLUE, RESET);

    return ec;
}

status_t print_hash_table(const hash_table_t *table)
{
    status_t ec = SUCCESS_CODE;
    hash_node_t *current = NULL;
    bool first_in_chain = true;

    if (!table || table->max_size == 0)
        ec = ERR_ARGS;

    if (ec == SUCCESS_CODE)
    {
        printf("%s", BLUE);
        printf("╔══════════════════════════════════════════════════════╗\n");
        printf("║                    HASH TABLE                        ║\n");
        printf("╠════════════╦═══════════════════════╦═════════════════╣\n");
        printf("║ Size: %4lu ║ Unique words: %7lu ║ Load: %6.1f%%   ║\n", table->max_size, table->uniq_words_quantity, (double)table->uniq_words_quantity / table->max_size * 100);
        printf("╠════════════╬═══════════════════════╩═════════════════╝\n");

        for (size_t i = 0; ec == SUCCESS_CODE && i < table->max_size; i++)
        {
            if (table->data[i])
            {                
                current = table->data[i];
                first_in_chain = true;
                
                printf("║ %10zu ║ ", i);
                while (current)
                {
                    if (!first_in_chain)
                        printf(" -> ");
                    
                    printf("%s", current->word);
                    current = current->next;
                    first_in_chain = false;
                }
                printf("\n");
            }
        }
        printf("╚════════════╝\n%s", RESET);
    }

    return ec;
}

status_t print_pretty_bst(bst_node_t *root) 
{
    status_t ec = SUCCESS_CODE;

    if (!root)
        ec = ERR_NOT_FOUND;
    
    if (ec == SUCCESS_CODE)  // печатаем корень
        printf("* %s (%zu)\n", root->word, root->counted);
    
    if (ec == SUCCESS_CODE && root->right)  // печатаем ребенка
        print_branch(root->right, "", root->left == NULL, BLUE);

    if (ec == SUCCESS_CODE && root->left)  // печатаем ребенка
        print_branch(root->left, "", 1, GREEN);

    return ec;
}

status_t print_branch(bst_node_t *node, char* prefix, int is_tail, char *color) 
{
    char new_prefix[MAX_PREFIX_SIZE];

    if (node)
    {
        // печатаем что есть и формируем новый префикс
        printf("%s%s%s%s%s (%zu)\n", prefix, color, (is_tail ? "└── " : "├── "), RESET, node->word, node->counted);
        snprintf(new_prefix, sizeof(new_prefix), "%s%s", prefix, (is_tail ? "    " : "│   "));

        if (node->right)
            print_branch(node->right, new_prefix, node->left == NULL, BLUE);

        if (node->left)
            print_branch(node->left, new_prefix, 1, GREEN);
    }

    return SUCCESS_CODE;
}

// TODO
// status_t print_compare_table(double average_tree_time_ns, double average_file_time_ns)
// {
//     printf("%s╔═════════════════════════════════════════════╗\n%s", BLUE_BOLD, RESET);
//     printf("%s║       C O M P A R E      R E S U L T S      ║\n%s", BLUE_BOLD, RESET);
//     printf("%s╠══════════════════════╦══════════════════════╣\n%s", BLUE_BOLD, RESET);
//     printf("%s║    tree time (ns)    ║    file time (ns)    ║\n%s", BLUE_BOLD, RESET);
//     printf("%s╠══════════════════════╬══════════════════════╣\n%s", BLUE_BOLD, RESET);
//     printf("%s║  %18.2lf  ║  %18.2lf  ║\n%s", BLUE_BOLD, average_tree_time_ns, average_file_time_ns, RESET);
//     printf("%s╚══════════════════════╩══════════════════════╝\n%s", BLUE_BOLD, RESET);
    
//     return SUCCESS_CODE;
// }


// TODO
/* status_t print_compare_find_operation_table(double time_balanced, double time_degenerate, double time_random)
** {
**    printf("%s╔═════════════════════════════════════════════╗\n%s", BLUE_BOLD, RESET);
**    printf("%s║    C O M P A R E  F I N D  R E S U L T S    ║\n%s", BLUE_BOLD, RESET);
**    printf("%s╠══════════════════════╦══════════════════════╣\n%s", BLUE_BOLD, RESET);
**    printf("%s║     balanced tree    ║ %-20lf ║\n%s", BLUE_BOLD, time_balanced, RESET);
**    printf("%s╠══════════════════════╬══════════════════════╣\n%s", BLUE_BOLD, RESET);
**    printf("%s║    degenerate tree   ║ %-20lf ║\n%s", BLUE_BOLD, time_degenerate, RESET);
**    printf("%s╠══════════════════════╬══════════════════════╣\n%s", BLUE_BOLD, RESET);
**    printf("%s║      random tree     ║ %-20lf ║\n%s", BLUE_BOLD, time_random, RESET);
**    printf("%s╚══════════════════════╩══════════════════════╝\n%s", BLUE_BOLD, RESET);
**
**    return SUCCESS_CODE;    
**}
*/