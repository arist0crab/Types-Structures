#include "input.h"

status_t input_cur_menu_opt(int *cur_menu_opt, int const menu_points_quantity)
{
    
    status_t ec = (cur_menu_opt) ? SUCCESS_CODE : ERR_INVALID_POINTER;
    char buffer[MAX_STRING_LENGTH], extra;
    int temp;

    printf("%sВыберите пункт меню: %s", BLUE, RESET);

    if (ec == SUCCESS_CODE)
        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
            ec = ERR_IO;

    if (ec == SUCCESS_CODE)
    {
        if (sscanf(buffer, "%d %c", &temp, &extra) != 1)
            ec = ERR_IO;
        else if (temp < 0 || temp >= menu_points_quantity)
            ec = ERR_RANGE;
        else
            *cur_menu_opt = temp;
    }

    return ec;
}