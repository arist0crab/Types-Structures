#include "../inc/data.h"
#include "input.h"
#include "output.h"


int main(void)
{
    status_t exit_code = SUCCESS_CODE;
    menu_option_t cur_menu_opt = 0;  // выбранная опция меню

    print_menu();
    exit_code = input_cur_menu_opt(&cur_menu_opt);
    while (cur_menu_opt != 0 && exit_code == SUCCESS_CODE)
    {
        print_menu();
        exit_code = input_cur_menu_opt(&cur_menu_opt);
    }

    return exit_code;
}