#include <stdio.h>
#include "../inc/data.h"
#include "../inc/input.h"
#include "../inc/output.h"
#include "../inc/process.h"
#include "../inc/hst.h"
#include "../inc/bst.h"

int main(void)
{
    result_t exit_code = OK_CODE;
    menu_option_t current_option = 1;

    bst_node_t *bst_root = NULL;
    avl_node_t *avl_root = NULL;
    hst_chaining_t *chaining_hst = NULL;
    hst_open_t *open_hst = NULL;

    chaining_hst = create_hash_table_chaining(10);
    open_hst = create_hash_table_open(10);

    print_menu();

    while (((exit_code = input_menu_option(&current_option)) == OK_CODE) && current_option)
    {
        exit_code = process_current_option(current_option, &bst_root, &avl_root, &chaining_hst, &open_hst);
        print_result(exit_code);
        print_menu();
    }

    print_result(exit_code);

    free_bst(bst_root);
    free_avl(avl_root);

    return exit_code;
}