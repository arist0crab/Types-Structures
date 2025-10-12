#include "output.h"


status_t print_menu(void)
{
    printf("+--------------------------------------------------+\n");
    printf("|                            Меню                  |\n");
    printf("+--------------------------------------------------+\n");
    printf("| 0  - Выйти из программы                          |\n");
    printf("| 1  - Умножить разреженную матрицу на вектор      |\n");
    printf("| 2  - Умножить полную матрицу на вектор           |\n");
    printf("| 3  - Вывести матрицу                             |\n");
    printf("| 4  - Сравнить алгоритмы матриц                   |\n");
    printf("+--------------------------------------------------+\n");

    return SUCCESS_CODE;
}

status_t print_result(status_t exit_code)
{
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

        case ERR_FILE:
            printf("%sПроизошла ошибка при работе с файлом :(\n%s", RED_BOLD, RESET);
            break;
        
        default:
            return UNKNOWN_ERROR;
            break;
    }

    return SUCCESS_CODE;
}