#include "process.h"


status_t proccess_menu_option(menu_option_t menu_option)
{
    status_t ec = SUCCESS_CODE;

    switch (menu_option)
    {
        case EXIT_PROGRAM:
            break;

        case PRINT_MATR:
            print_all_matrices();
            break;

        case INPUT_MATR:
            ec = input_any_matrix();
            break;

        case CSR_CSC_MATR_MULT:
            break;

        case COMMON_MATR_MULT:
            break;

        case COMPARE_ALGORITHMS:
            break;
    }

    return ec;
}