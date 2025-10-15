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
            ec = multiply_csr_and_csc();
            if (result_CSR_matr.A && result_CSR_matr.IA && result_CSR_matr.JA)
                print_csr_matrix(&result_CSR_matr);
            break;

        case COMMON_MATR_MULT:
            ec = multiply_dense_matrices();
            if (dense_matr_result.data && dense_matr_result.rows > 0 && dense_matr_result.cols > 0)
                print_dense_matrix(&dense_matr_result);
            break;

        case COMPARE_ALGORITHMS:
            print_compare_algorithms_warning();
            ec = compare_matrix_multiplication();
            break;
    }

    return ec;
}