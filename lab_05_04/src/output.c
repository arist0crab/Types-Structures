#include "output.h"


status_t print_menu(void)
{
    printf("%s", BLUE);
    printf("╔═════════════════════════════════════════════╗\n");
    printf("║                   M E N U                   ║\n");
    printf("╠═════════════════════════════════════════════╣\n");
    printf("║ 0. Выход                                    ║\n");
    printf("║ 1. Запустить симуляцию массивом             ║\n");
    printf("║ 2. Запустить симуляцию списком              ║\n");
    printf("║ 3. Оценить эффективность                    ║\n");
    printf("║ 4. Посмотреть информацию о памяти (массив)  ║\n");
    printf("║ 5. Посмотреть информацию о памяти (список)  ║\n");
    printf("║ 6. Настроить параметры симуляции            ║\n");
    printf("╚═════════════════════════════════════════════╝\n");
    printf("%s", RESET);

    return SUCCESS_CODE;
}

status_t print_simulation_config_menu(void)
{
    printf("%s", BLUE);
    printf("╔═════════════════════════════════════════════╗\n");
    printf("║          C O N F I G    M E N U             ║\n");
    printf("╠═════════════════════════════════════════════╣\n");
    printf("║ 0. Назад                                    ║\n");
    printf("║ 1. Изменить время поступления заявок Т1     ║\n");
    printf("║ 2. Изменить время поступления заявок Т2     ║\n");
    printf("║ 3. Изменить время обработки заявок Т1       ║\n");
    printf("║ 4. Изменить время обработки заявок Т2       ║\n");
    printf("╚═════════════════════════════════════════════╝\n");
    printf("%s", RESET);

    return SUCCESS_CODE;
}

status_t print_simulation_configs(time_range_t *arr_t1, time_range_t *arr_t2, time_range_t *serve_t1, time_range_t *serve_t2)
{
    printf("%s", BLUE);
    printf("╔════════════════════════════════════════════════════╗\n");
    printf("║            C U R R E N T  C O N F I G S            ║\n");
    printf("╠════════════════════════════════════════════════════╣\n");
    printf("║ Интервал поступления T1:  { %-6.2lf, %-6.2lf } ед.в. ║\n", arr_t1->min_time, arr_t1->max_time);
    printf("║ Интервал поступления T2:  { %-6.2lf, %-6.2lf } ед.в. ║\n", arr_t2->min_time, arr_t2->max_time);
    printf("║ Интервал обслуживания T1: { %-6.2lf, %-6.2lf } ед.в. ║\n", serve_t1->min_time, serve_t1->max_time);
    printf("║ Интервал обслуживания T2: { %-6.2lf, %-6.2lf } ед.в. ║\n", serve_t2->min_time, serve_t2->max_time);
    printf("╚════════════════════════════════════════════════════╝\n");
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

        case ERR_OVERFLOW:
            printf("%sПроизошло переполнение чего-нибудь\n%s", RED_BOLD, RESET);
            break;

        case ERR_POP_EMPTY:
            printf("%sПроизошла попытка удаления элемента из пустоко стека%s\n", RED_BOLD, RESET);
            break;

        case ERR_INVALID_POINTER:
            printf("%sПроизошла ошибка при работе с указателем%s\n", RED_BOLD, RESET);
            break;
        
        default:
            ec = ERR_RANGE;
            break;
    }

    return ec;
}