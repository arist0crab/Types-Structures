#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "data.h"
#include <stdio.h>
#include <stdlib.h>

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
extern CSR_matrix_t CSR_matr;
extern CSC_matrix_t CSC_matr;
extern int matrices_initialized_quantity;

#endif