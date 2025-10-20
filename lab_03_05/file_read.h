#ifndef __FILE_READ_H__
#define __FILE_READ_H__

#define MAX_FILENAME_LENGTH 128

#include "data.h"
#include "matrix.h"
#include "color.h"
#include <stdbool.h>

status_t read_csc_from_file(CSC_matrix_t *csc_matrix);
status_t read_csr_from_file(CSR_matrix_t *csr_matrix);
status_t read_dense_from_file(dense_matrix_t *dense_matr);

#endif