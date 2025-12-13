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
    printf("║                РАБОТА С ГРАФОМ               ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║ 1. Создать граф                              ║\n");
    printf("║ 2. Уничтожить граф                           ║\n");
    printf("║ 3. Настроить граф вручную                    ║\n");
    printf("║ 4. Считать граф из файла                     ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║                ВВОД ПАРАМЕТРОВ               ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║ 5. Задать столицу                            ║\n");
    printf("║ 6. Задать расстояние Т                       ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║                ОБРАБОТКА ГРАФОВ              ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║ 7. Найти кратчайшее между двумя городами     ║\n");
    printf("║ 8. Найти дальние города                      ║\n");
    printf("║ 9. Проверить связность графа                 ║\n");
    printf("║ 10. Найти минимальный остов                  ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║                ФУНКЦИИ ВЫВОДА                ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║ 11. Вывести граф                             ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    printf("%s", RESET);

    return SUCCESS_CODE;
}

status_t print_manual_input_menu(void)
{
    printf("%s", BLUE);
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║               MANUAL INPUT MENU              ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║ 0. Назад                                     ║\n");
    printf("║ 1. Добавить город                            ║\n");
    printf("║ 2. Добавить дорогу                           ║\n");
    printf("║ 3. Удалить город                             ║\n");
    printf("║ 4. Удалить дорогу                            ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    printf("%s", BLUE);

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
            printf("%sПроизошла ошибка при работе с аргументами функции%s\n", RED_BOLD, RESET);
            break;

        case ERR_FILE:
            printf("%sПроизошла ошибка при работе с файлом%s\n", RED_BOLD, RESET);
            break;

        case ERR_RANGE:
            printf("%sПроизошла ошибка обработки массива или обработки количества чего-либо :(\n%s", RED_BOLD, RESET);
            break; 

        case ERR_INVALID_POINTER:
            printf("%sПроизошла ошибка при работе с указателем\n%s", RED_BOLD, RESET);
            break;

        case ERR_NOT_FOUND:
            printf("%sПроизошла ошибка: начего не найдено\n%s", RED_BOLD, RESET);
            break;

        case ERR_ALREADY_EXISTS:
            printf("%sПроизошла ошибка: элемент уже существует\n%s", RED_BOLD, RESET);
            break;

        default:
            ec = ERR_RANGE;
            break;
    }

    return ec;
}
