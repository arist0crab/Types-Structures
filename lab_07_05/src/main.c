// TODO Подсчитать среднее количество сравнений для поиска данных в указанных структурах.
// TODO Произвести реструктуризацию хеш-таблицы, если среднее количество сравнений больше указанного.

#include "../inc/data.h"  // TODO
#include "bst.h"
#include "avl.h"
#include "process.h"
#include "input.h"
#include "output.h"


int main(void)
{
    status_t menu_opt_processing_status = SUCCESS_CODE;
    menu_option_t cur_menu_opt = 0;  // выбранная опция меню

    bst_node_t *bst_root = NULL;

    print_menu();
    input_cur_menu_opt((int *)&cur_menu_opt, MENU_OPTIONS_QUANTITY);
    while (cur_menu_opt != 0)
    {
        menu_opt_processing_status = procces_menu_choice(cur_menu_opt, &bst_root);
        print_exit_code_result(menu_opt_processing_status);
        print_menu();
        input_cur_menu_opt((int *)&cur_menu_opt, MENU_OPTIONS_QUANTITY);
    }

    return SUCCESS_CODE;
}