#include "matrix.h"

status_t free_all_matr(void);
status_t free_dense_matrix(dense_matrix_t *dense_matrix);
status_t free_csr_matr(void);
status_t free_csc_matr(void);
status_t allocate_dense_matrix(dense_matrix_t *dense_matr, size_t n, size_t m);

// инициализируем глобальные структуры
dense_matrix_t dense_matr_1 = {0}, dense_matr_2 = {0};
CSR_matrix_t CSR_matr = {0};
CSC_matrix_t CSC_matr = {0};
int matrices_initialized_quantity = 0;

status_t allocate_dense_matrix(dense_matrix_t *dense_matr, size_t n, size_t m)
{
    status_t ec = SUCCESS_CODE;
    int *data = NULL;  // указатель на всю матрицу значений одним блоком

    // освобождаем dense_matr, если он что-то содержит
    free_dense_matrix(dense_matr);
    dense_matr->data = NULL;

    if (n == 0 || m == 0 || n > MAX_MATRIX_SIZE || m > MAX_MATRIX_SIZE)
        ec = ERR_RANGE;

    if (ec == SUCCESS_CODE)
    {
        // выделяем память под массив указателей на строки
        dense_matr->data = calloc(n, sizeof(int*));
        if (dense_matr->data == NULL)
            ec = ERR_MEM;
    }

    if (ec == SUCCESS_CODE)
    {
        // выделяем память под всю матрицу одним блоком
        data = calloc(n, m * sizeof(int));
        if (data == NULL)
            ec = ERR_MEM;
    }

    if (ec == SUCCESS_CODE)
    {
        // настраиваем указатели на строки
        for (size_t i = 0; i < n; i++)
            dense_matr->data[i] = data + i * m;  // каждый указатель указывает на начало новой строки  
            
        dense_matr->rows = n;
        dense_matr->cols = m;
    }

    // если что-то пошло не так, освобождаем все
    if (ec != SUCCESS_CODE)
    {
        if (dense_matr->data) free(dense_matr->data);
        if (data) free(data);
        dense_matr->data = NULL;
        data = NULL;
    }

    return ec;
}

status_t allocate_csr_matrix(size_t non_zero_quantity, size_t rows_quantity)
{
    status_t ec = SUCCESS_CODE;

    if (non_zero_quantity == 0)
        ec = ERR_RANGE;

    CSR_matr.A = NULL;
    CSR_matr.JA = NULL;
    CSR_matr.IA = NULL;

    CSR_matr.A = calloc(non_zero_quantity, sizeof(int));
    CSR_matr.JA = calloc(non_zero_quantity, sizeof(int));
    CSR_matr.IA = calloc(rows_quantity + 1, sizeof(int));

    if (CSR_matr.A == NULL || CSR_matr.JA == NULL || CSR_matr.IA == NULL)
        ec = ERR_MEM;

    if (ec != SUCCESS_CODE)
    {
        CSR_matr.A = NULL;
        CSR_matr.JA = NULL;
        CSR_matr.IA = NULL;
    }

    return ec;
}

status_t allocate_csc_matrix(size_t non_zero_quantity, size_t cols_quantity)
{
    status_t ec = SUCCESS_CODE;

    if (non_zero_quantity == 0)
        ec = ERR_RANGE;

    CSC_matr.B = NULL;
    CSC_matr.JB = NULL;
    CSC_matr.IB = NULL;

    CSC_matr.B = calloc(non_zero_quantity, sizeof(int));
    CSC_matr.IB = calloc(non_zero_quantity, sizeof(int));
    CSC_matr.JB = calloc(cols_quantity + 1, sizeof(int));

    if (CSC_matr.B == NULL || CSC_matr.JB == NULL || CSC_matr.IB == NULL)
        ec = ERR_MEM;

    if (ec != SUCCESS_CODE)
    {
        CSC_matr.B = NULL;
        CSC_matr.JB = NULL;
        CSC_matr.IB = NULL;
    }

    return ec;
}

status_t free_dense_matrix(dense_matrix_t *dense_matrix)
{
    status_t ec = SUCCESS_CODE;
    
    if (dense_matrix == NULL)
        return ec;
    
    if (dense_matrix->data && dense_matrix->rows > 0) 
    {
        // находим минимальный указатель (начало общего блока данных)
        int *min_ptr = dense_matrix->data[0];
        for (size_t i = 1; i < dense_matrix->rows; i++)
            if (dense_matrix->data[i] != NULL && dense_matrix->data[i] < min_ptr)
                min_ptr = dense_matrix->data[i];
        
        free(min_ptr);
        free(dense_matrix->data);
    }
    
    // обнуляем структуру
    dense_matrix->data = NULL;
    dense_matrix->rows = 0;
    dense_matrix->cols = 0;

    // "сбросили" матрицу - подкрутили счетчик
    matrices_initialized_quantity -= (matrices_initialized_quantity > 0);
    
    return ec;
}

status_t free_csr_matr(void)
{
    if (CSR_matr.A) free(CSR_matr.A);
    if (CSR_matr.JA) free(CSR_matr.JA);
    if (CSR_matr.IA) free(CSR_matr.IA);
    CSR_matr.A = NULL, CSR_matr.JA = NULL, CSR_matr.IA = NULL;
    CSR_matr.rows = 0, CSR_matr.cols = 0, CSR_matr.non_zero = 0;

    // "сбросили" матрицу - подкрутили счетчик
    matrices_initialized_quantity -= (matrices_initialized_quantity > 0);

    return SUCCESS_CODE;
}

status_t free_csc_matr(void)
{
    if (CSC_matr.B) free(CSC_matr.B);
    if (CSC_matr.JB) free(CSC_matr.JB);
    if (CSC_matr.IB) free(CSC_matr.IB);
    CSC_matr.B = NULL, CSC_matr.JB = NULL, CSC_matr.IB = NULL;
    CSC_matr.rows = 0, CSC_matr.cols = 0, CSC_matr.non_zero = 0;

    // "сбросили" матрицу - подкрутили счетчик
    matrices_initialized_quantity -= (matrices_initialized_quantity > 0);

    return SUCCESS_CODE;
}

status_t free_all_matr(void)
{
    free_dense_matrix(&dense_matr_1);
    free_dense_matrix(&dense_matr_2);
    free_csr_matr();
    free_csc_matr();

    matrices_initialized_quantity = 0;

    return SUCCESS_CODE;
}