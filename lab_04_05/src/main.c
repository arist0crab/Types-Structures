#include "../inc/data.h"
#include "arr_stack.h"
#include "input.h"
#include "output.h"
#include "process.h"


int main(void)
{
    status_t exit_code = SUCCESS_CODE;
    status_t menu_opt_processing_status = SUCCESS_CODE;
    menu_option_t cur_menu_opt = 0;  // выбранная опция меню

    arr_stack_t main_arr_stack = { {0}, 0 };
    list_stack_t main_list_stack = { 0 };

    print_menu();
    exit_code = input_cur_menu_opt(&cur_menu_opt);
    while (cur_menu_opt != 0 && exit_code == SUCCESS_CODE)
    {
        menu_opt_processing_status = procces_menu_choice(cur_menu_opt, &main_arr_stack, &main_list_stack);
        print_exit_code_result(menu_opt_processing_status);
        print_menu();
        exit_code = input_cur_menu_opt(&cur_menu_opt);
    }

    print_exit_code_result(exit_code);

    return exit_code;
}