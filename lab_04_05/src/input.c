#include "input.h"

status_t input_cur_menu_opt(menu_option_t *cur_menu_opt)
{
    
    status_t ec = (cur_menu_opt) ? SUCCESS_CODE : ERR_INVALID_POINTER;
    char buffer[MAX_EXPRESSION_SIZE], extra;
    int temp;

    printf("%sВыберите пункт меню: %s", BLUE, RESET);

    if (ec == SUCCESS_CODE)
        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
            ec = ERR_IO;

    if (ec == SUCCESS_CODE)
    {
        if (sscanf(buffer, "%d %c", &temp, &extra) != 1)
            ec = ERR_IO;
        else if (temp < 0 || temp > 10)
            ec = ERR_RANGE;
        else
            *cur_menu_opt = (menu_option_t)temp;
    }

    return ec;
}


status_t get_expression(char *expression)
{
    status_t ec = SUCCESS_CODE;
 
    if (expression == NULL)
        ec = ERR_MEM;
    else
    {
        printf("%sВведите арифметическое выражение: %s", BLUE, RESET);
        if (fgets(expression, MAX_EXPRESSION_SIZE, stdin) != NULL)
            expression[strcspn(expression, "\n")] = '\0';
        else
            ec = ERR_IO;
    }

    if (ec == SUCCESS_CODE)
        printf("Введеное выражение: %s\n", expression);

    return ec;
}

status_t get_stack_elem(int *elem)
{
    status_t ec = (elem) ? SUCCESS_CODE : ERR_INVALID_POINTER;
    char buffer[MAX_EXPRESSION_SIZE], extra;
    int temp;

    if (ec == SUCCESS_CODE)
    {
        printf("%sВведите элемент стека: %s", BLUE, RESET);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
            ec = ERR_IO;
    }

    if (ec == SUCCESS_CODE)
        if (sscanf(buffer, "%d %c", &temp, &extra) != 1)
            ec = ERR_IO;

    if (ec == SUCCESS_CODE)
        *elem = temp;

    return ec;
}