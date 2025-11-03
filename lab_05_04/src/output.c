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
    printf("║ 4. Посмотреть информацию о памяти           ║\n");
    printf("║ 5. Настроить параметры симуляции            ║\n");
    printf("╚═════════════════════════════════════════════╝\n");
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

        // case ERR_MEM:
        //     printf("%sПроизошла ошибка при работе с памятью :(\n%s", RED_BOLD, RESET);
        //     break;

        // case ERR_OVERFLOW:
        //     printf("%sПроизошло переполнение чего-нибудь\n%s", RED_BOLD, RESET);
        //     break;

        // case ERR_POP_EMPTY:
        //     printf("%sПроизошла попытка удаления элемента из пустоко стека%s\n", RED_BOLD, RESET);
        //     break;

        case ERR_INVALID_POINTER:
            printf("%sПроизошла ошибка при работе с указателем%s\n", RED_BOLD, RESET);
            break;
        
        default:
            ec = ERR_RANGE;
            break;
    }

    return ec;
}