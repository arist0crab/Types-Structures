#include "../inc/data.h"
#include "process.h"
#include "input.h"
#include "output.h"

int main(void)
{
    status_t exit_code = SUCCESS_CODE;
    status_t menu_opt_processing_status = SUCCESS_CODE;
    menu_option_t cur_menu_opt = 0;  // выбранная опция меню

    tree_node_t *root = NULL;

    print_menu();
    exit_code = input_cur_menu_opt((int *)&cur_menu_opt, MENU_OPTIONS_QUANTITY);
    while (cur_menu_opt != 0 && exit_code == SUCCESS_CODE)
    {
        menu_opt_processing_status = procces_menu_choice(cur_menu_opt, &root);
        print_exit_code_result(menu_opt_processing_status);
        print_menu();
        exit_code = input_cur_menu_opt((int *)&cur_menu_opt, MENU_OPTIONS_QUANTITY);
    }

    print_exit_code_result(exit_code);

    return exit_code;
}