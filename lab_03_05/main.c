#include "data.h"
#include "input.h"
#include "output.h"
#include "process.h"
#include <stdbool.h>

int main(void)
{
    status_t exit_code = SUCCESS_CODE;  // основной код возврата из программы
    status_t menu_opt_processing_status = SUCCESS_CODE;  // код возврата для обработки ошибок внутри цикла
    menu_option_t cur_menu_opt;  // выбранная опция меню

    print_menu();
    exit_code = input_cur_menu_opt(&cur_menu_opt);
    while (cur_menu_opt != 0 && exit_code == SUCCESS_CODE)
    {
        menu_opt_processing_status = proccess_menu_option(cur_menu_opt);
        print_result(menu_opt_processing_status);
        print_menu();
        exit_code = input_cur_menu_opt(&cur_menu_opt);
    }
    
    print_final_common_result(exit_code);

    return exit_code;
}