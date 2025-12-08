#include "../inc/output.h"  // TODO

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
    printf("║ 7. Создать хэш-таблицу                       ║\n");
    printf("║ 8. Добавить элемент в хэш-таблицу            ║\n");
    printf("║ 9. Удалить элемент из хэш-таблицы            ║\n");
    printf("║ 10. Очистить хэш-таблицу                     ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║       РАБОТА СО СБАЛАНСИРОВАННЫМ ДЕРЕВОМ     ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║ 11. Создать сбалансированное дерево          ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║           ПЕЧАТЬ ДЕЕРВЬЕВ И ТАБЛИЦ           ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║ 12. Вывести бинарное дерево                  ║\n");
    printf("║ 13. Вывести хэш-таблицу                      ║\n");
    printf("║ 14. Вывести сбалансированное дерево          ║\n");    
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║                  ЭФФЕКТИВНОСТЬ               ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║ 15. Сравнить эффективность                   ║\n");
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

        case ERR_INVALID_POINTER:
            printf("%sПроизошла ошибка при работе с указателем\n%s", RED_BOLD, RESET);
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