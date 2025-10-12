#include "input.h"


status_t input_cur_menu_opt(menu_option_t *cur_menu_opt)
{
    int temp;
    printf("%sВыберите пункт меню: %s", BLUE, RESET);

    if (scanf("%d", &temp) != 1 || temp < 0 || temp > 4)
        return ERR_IO;
    
    *cur_menu_opt = (menu_option_t)temp;
    return SUCCESS_CODE;
}