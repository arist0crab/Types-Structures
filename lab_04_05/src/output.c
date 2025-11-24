#include "output.h"


status_t print_menu(void)
{
    printf("+--------------------------------------------------+\n");
    printf("|                        Меню                      |\n");
    printf("+--------------------------------------------------+\n");
    printf("| 0  - Выйти из программы                          |\n");
    printf("+--------------------------------------------------+\n");
    printf("|                    М А С С И В                   |\n");
    printf("+--------------------------------------------------+\n");
    printf("| 1 - вычислить арифиметическое выражение          |\n");
    printf("| 2 - добавить элемент в стек                      |\n");
    printf("| 3 - удалить элемент из стека                     |\n");
    printf("| 4 - вывести стек                                 |\n");
    printf("+--------------------------------------------------+\n");
    printf("|                    С П И С О К                   |\n");
    printf("+--------------------------------------------------+\n");
    printf("| 5 - вычислить арифиметическое выражение          |\n");
    printf("| 6 - добавить элемент в стек                      |\n");
    printf("| 7 - удалить элемент из стека                     |\n");
    printf("| 8 - вывести стек                                 |\n");
    printf("+--------------------------------------------------+\n");
    printf("| 9 - сравнить производительность                  |\n");
    printf("| 10 - сравнить pop и push                         |\n");
    printf("| 11 - вывести массив свободных областей памяти    |\n");
    printf("+--------------------------------------------------+\n");

    return SUCCESS_CODE;
}

status_t print_push_pop_table(double arr_push_time, double arr_pop_time, double list_push_time, double list_pop_time)
{
    printf("+-------+---------------+----------------+\n");
    printf("|       | push operation | pop operation |\n");
    printf("+-------+---------------+----------------+\n");
    printf("| array | %-14.2lf | %-13.2lf |\n", arr_push_time, arr_pop_time);
    printf("| list  | %-14.2lf | %-13.2lf |\n", list_push_time, list_pop_time);
    printf("+-------+---------------+----------------+\n");

    return SUCCESS_CODE;
}

status_t print_compare_table_header(void)
{
    printf("+-----------+------------+----------------+---------------+--------------+-------------+\n");
    printf("| operation | stack size | memory (array) | memory (list) | time (array) | time (list) |\n");
    printf("+-----------+------------+----------------+---------------+--------------+-------------+\n");

    return SUCCESS_CODE;
}

status_t print_table_raw(char op_char, size_t stack_size, size_t memus_array, size_t memus_list, double time_array, double time_list)
{
    printf("|     %c     | %-10lu | %-14lu | %-13lu | %-12.2lf | %-11.2lf |\n", op_char, stack_size, memus_array, memus_list, time_array, time_list);
    printf("+-----------+------------+----------------+---------------+--------------+-------------+\n");
       
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

        case ERR_OVERFLOW:
            printf("%sПроизошло переполнение чего-нибудь\n%s", RED_BOLD, RESET);
            break;

        case ERR_POP_EMPTY:
            printf("%sПроизошла попытка удаления элемента из пустоко стека%s\n", RED_BOLD, RESET);
            break;

        case ERR_INVALID_POINTER:
            printf("%sПроизошла ошибка при работе с указателем%s\n", RED_BOLD, RESET);
            break;

        case ERR_ZERO_DIVISION:
            printf("%sПроизошла попытка деления на 0%s\n", RED_BOLD, RESET);
            break;
        
        default:
            ec = ERR_RANGE;
            break;
    }

    return ec;
}

status_t print_calc_result(int result)
{
    printf("%sРезультат вычислений: %d\n%s", BLUE_BOLD, result, RESET);
    return SUCCESS_CODE;
}

status_t print_pop_result(int result)
{
    printf("%sУдаленный элемент стека: %d\n%s", BLUE, result, RESET);
    return SUCCESS_CODE;
}