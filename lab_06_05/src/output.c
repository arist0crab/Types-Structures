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
        
        default:
            ec = ERR_RANGE;
            break;
    }

    return ec;
}

status_t print_find_result(const tree_node_t **root)
{
    status_t ec = SUCCESS_CODE;

    if (!root)
        ec = ERR_ARGS;

    if (ec == SUCCESS_CODE && *root)
        printf("%sСлово было успешно найдено: %s (%lu)\n%s", BLUE, (*root)->word, (*root)->counted, RESET);
    else if (ec == SUCCESS_CODE && !(*root))
        printf("%sСлово не было найдено\n%s", BLUE, RESET);

    return ec;
}