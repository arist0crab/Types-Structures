#ifndef __MATRIX_H__
#define __MATRIX_H__

typedef struct {
    double *A;
    int *JA;
    int *IA;
    int rows;
    int cols;
    int non_zero;
} CSR_matrix_t;

typedef struct {
    double *B;
    int *IB;
    int *JB;
    int rows;
    int cols;       
    int non_zero;   
} CSC_matrix_t;

typedef struct {
    double **data;
    int rows;
    int cols;
} dense_matrix_t;

#endif