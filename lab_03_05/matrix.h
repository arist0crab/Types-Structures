#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *A;
    int *JA;
    int *IA;
    size_t rows;
    size_t cols;
    size_t non_zero;
} CSR_matrix_t;

typedef struct {
    int *B;
    int *IB;
    int *JB;
    size_t rows;
    size_t cols;       
    size_t non_zero;   
} CSC_matrix_t;

typedef struct {
    int **data;
    size_t rows;
    size_t cols;
} dense_matrix_t;

extern dense_matrix_t dense_matr_1, dense_matr_2;
extern CSR_matrix_t result_CSR_matr;
extern CSR_matrix_t CSR_matr;
extern CSC_matrix_t CSC_matr;
extern int matrices_initialized_quantity;

status_t free_all_matr(void);
status_t free_dense_matrix(dense_matrix_t *dense_matrix);
status_t free_csr_matr(void);
status_t free_csc_matr(void);
status_t allocate_dense_matrix(dense_matrix_t *dense_matr, size_t n, size_t m);
status_t allocate_csr_matrix(size_t non_zero_quantity, size_t rows_quantity);
status_t allocate_csc_matrix(size_t non_zero_quantity, size_t cols_quantity);
status_t multiply_csr_and_csc(void);

#endif