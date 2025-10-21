#ifndef __COMP_ALG_H__
#define __COMP_ALG_H__

#include "data.h"
#include "matrix.h"
#include "input.h"
#include "output.h"
#include <stdbool.h>

#define COMPARE_ITERATIONS_QUANITY 100

status_t compare_matrix_multiplication(void);
status_t fill_random_csr(CSR_matrix_t *mat, size_t num_random);
status_t fill_random_csc(CSC_matrix_t *mat, size_t num_random);
status_t fill_random_dense(dense_matrix_t *mat, size_t num_random);

#endif