#include "../inc/data.h"  // TODO
#include "output.h"
#include "../inc/input.h"
#include "../inc/process.h"


int main(void)
{
    status_t menu_opt_processing_status = SUCCESS_CODE;
    menu_option_t cur_menu_opt = 0;  // выбранная опция меню

    print_menu();
    while (input_cur_menu_opt((int *)&cur_menu_opt, MENU_OPTIONS_QUANTITY) != SUCCESS_CODE);
    while (cur_menu_opt != 0)
    {
        menu_opt_processing_status = procces_menu_choice(cur_menu_opt);
        print_exit_code_result(menu_opt_processing_status);
        print_menu();
        while (input_cur_menu_opt((int *)&cur_menu_opt, MENU_OPTIONS_QUANTITY) != SUCCESS_CODE);
    }

    return SUCCESS_CODE;
}