#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include "data.h"
#include "color.h"
#include "matrix.h"
#include <stdio.h>

// печать инструкций, текста и тп
status_t print_menu(void);
status_t print_compare_algorithms_warning(void);
status_t print_file_input_warning(void);
status_t print_result(status_t exit_code);
status_t print_final_common_result(status_t exit_code);

// печать матриц
status_t print_all_matrices(void);
status_t print_csr_matrix(const CSR_matrix_t *csr_matr);
status_t print_csc_matrix(const CSC_matrix_t *csc_matr);
status_t print_dense_matrix(const dense_matrix_t *dense_matr);

#endif