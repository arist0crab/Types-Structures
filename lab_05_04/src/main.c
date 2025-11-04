#include "data.h"
#include "input.h"
#include "output.h"
#include "proccess.h"
#include "arr.h"

int main(void)
{
    status_t exit_code = SUCCESS_CODE;
    status_t menu_opt_processing_status = SUCCESS_CODE;
    menu_option_t cur_menu_opt = RUN_ARRAY_SIMULATION;

    print_menu();
    exit_code = input_cur_menu_opt(&cur_menu_opt);
    while (cur_menu_opt != EXIT_PROGRAM && exit_code == SUCCESS_CODE)
    {
        menu_opt_processing_status = procces_menu_choice(cur_menu_opt);
        print_exit_code_result(menu_opt_processing_status);
        print_menu();
        exit_code = input_cur_menu_opt(&cur_menu_opt);
    }

    return exit_code;
}