#include "input.h"

status_t print_menu(void)
{
    printf("%s", BLUE);
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║                   M E N U                    ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║ 0. Выход                                     ║\n");
    printf("║ 1. Считать дерево из файла                   ║\n");
    printf("║ 2. Вывести дерево                            ║\n");
    printf("║ 3. Добавить узел дерева                      ║\n");
    printf("║ 4. Удалить узел дерева                       ║\n");
    printf("║ 5. Найти слово в файле                       ║\n");
    printf("║ 6. Найти слово в дереве                      ║\n");
    printf("║ 7. Сравнить время поиска слова дерево/файл   ║\n");
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

        case ERR_RANGE:
            printf("%sПроизошла ошибка обработки массива или обработки количества чего-либо :(\n%s", RED_BOLD, RESET);
            break;

        case ERR_MEM:
            printf("%sПроизошла ошибка при работе с памятью :(\n%s", RED_BOLD, RESET);
            break;
            
        case ERR_ARGS:
            printf("%sПроизошла ошибка при работе с аргументами функии%s\n", RED_BOLD, RESET);
            break;

        case ERR_NOT_FOUND:
            printf("%sПроизошла ошибка при поиске чего-нибудь: мы ничего не нашли :(%s\n", RED_BOLD, RESET);
            break;

        case ERR_FILE:
            printf("%sПроизошла ошибка при работе с файлом%s\n", RED_BOLD, RESET);
            break;

        case ERR_EMPTY_TREE:
            printf("%sПроизошла ошибка: невозможно выполнить для пустого дерева%s\n", RED_BOLD, RESET);
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

status_t print_compare_table(double average_tree_time_ns, double average_file_time_ns)
{
    printf("%s╔═════════════════════════════════════════════╗\n%s", BLUE_BOLD, RESET);
    printf("%s║       C O M P A R E      R E S U L T S      ║\n%s", BLUE_BOLD, RESET);
    printf("%s╠══════════════════════╦══════════════════════╣\n%s", BLUE_BOLD, RESET);
    printf("%s║    tree time (ns)    ║    file time (ns)    ║\n%s", BLUE_BOLD, RESET);
    printf("%s╠══════════════════════╬══════════════════════╣\n%s", BLUE_BOLD, RESET);
    printf("%s║  %18.2lf  ║  %18.2lf  ║\n%s", BLUE_BOLD, average_tree_time_ns, average_file_time_ns, RESET);
    printf("%s╚══════════════════════╩══════════════════════╝\n%s", BLUE_BOLD, RESET);
    
    return SUCCESS_CODE;
}