#include "data.h"
#include "input.h"
#include "output.h"
#include <stdbool.h>

int main(void)
{
    status_t exit_code = SUCCESS_CODE;
    menu_option_t cur_menu_opt;

    do
    {
        print_menu();
        exit_code = input_cur_menu_opt(&cur_menu_opt);
        // TODO вставить основуню обработку
    } 
    while (cur_menu_opt != 0 && exit_code == SUCCESS_CODE);
    
    exit_code = print_result(exit_code);

    return exit_code;
}