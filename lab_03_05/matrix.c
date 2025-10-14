#include "matrix.h"

status_t free_all_matr(void);
status_t free_dense_matrix(dense_matrix_t *dense_matrix);
status_t free_csr_matr(void);
status_t free_csc_matr(void);
status_t allocate_dense_matrix(dense_matrix_t *dense_matr, size_t n, size_t m);

// инициализируем глобальные структуры
dense_matrix_t dense_matr_1 = {0}, dense_matr_2 = {0};
dense_matrix_t dense_matr_result = {0};
CSR_matrix_t result_CSR_matr = {0};
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

status_t free_result_csr_matr(void)
{
    if (result_CSR_matr.A) free(result_CSR_matr.A);
    if (result_CSR_matr.JA) free(result_CSR_matr.JA);
    if (result_CSR_matr.IA) free(result_CSR_matr.IA);
    result_CSR_matr.A = NULL, result_CSR_matr.JA = NULL, result_CSR_matr.IA = NULL;
    result_CSR_matr.rows = 0, result_CSR_matr.cols = 0, result_CSR_matr.non_zero = 0;

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
    free_dense_matrix(&dense_matr_result);
    free_result_csr_matr();
    free_csr_matr();
    free_csc_matr();

    matrices_initialized_quantity = 0;

    return SUCCESS_CODE;
}


status_t multiply_csr_and_csc(void)
{
    status_t ec = SUCCESS_CODE;

    size_t i, j, k;
    size_t rowsA, colsA, rowsB, colsB;
    int *temp_row = NULL;
    size_t temp_size = 0;
    size_t nnz_count = 0;

    if (!CSR_matr.A || !CSR_matr.JA || !CSR_matr.IA || CSR_matr.rows == 0 || CSR_matr.cols == 0 ||  CSR_matr.non_zero == 0)
        return ERR_MEM;
    if (!CSC_matr.B || !CSC_matr.IB || !CSC_matr.JB || CSC_matr.rows == 0 || CSC_matr.cols == 0 || CSC_matr.non_zero == 0)
        return ERR_MEM;

    rowsA = CSR_matr.rows;
    colsA = CSR_matr.cols;
    rowsB = CSC_matr.rows;
    colsB = CSC_matr.cols;

    if (colsA != rowsB)
        return ERR_RANGE;

    // временный массив для накопления значений строки
    temp_row = (int*)calloc(colsB, sizeof(int));
    if (!temp_row)
        ec = ERR_MEM;

    // результирующая структура
    if (ec == SUCCESS_CODE) 
    {
        result_CSR_matr.rows = rowsA;
        result_CSR_matr.cols = colsB;
        result_CSR_matr.IA = (int*)calloc(rowsA + 1, sizeof(int));
        if (!result_CSR_matr.IA) ec = ERR_MEM;
    }

    // подсчёт количества ненулевых элементов и запись в IA
    if (ec == SUCCESS_CODE) 
    {
        for (i = 0; i < rowsA; i++) 
        {
            temp_size = 0;
            for (j = 0; j < colsB; j++) 
            {
                int sum = 0;
                for (k = CSR_matr.IA[i]; k < (size_t)CSR_matr.IA[i + 1]; k++) 
                {
                    int colA = CSR_matr.JA[k];
                    for (size_t p = CSC_matr.JB[j]; p < (size_t)CSC_matr.JB[j + 1]; p++) 
                    {
                        if (CSC_matr.IB[p] == colA) 
                        {
                            sum += CSR_matr.A[k] * CSC_matr.B[p];
                            break;
                        }
                    }
                }
                if (sum != 0) 
                {
                    temp_row[temp_size++] = j; // сохраняем индекс столбца
                    nnz_count++;
                }
            }
            result_CSR_matr.IA[i + 1] = nnz_count;
        }
    }

    // выделение памяти под A и JA
    if (ec == SUCCESS_CODE) 
    {
        result_CSR_matr.A = (int*)malloc(nnz_count * sizeof(int));
        result_CSR_matr.JA = (int*)malloc(nnz_count * sizeof(int));
        if (!result_CSR_matr.A || !result_CSR_matr.JA) ec = ERR_MEM;
    }

    // заполнение A и JA
    if (ec == SUCCESS_CODE) 
    {
        nnz_count = 0;
        for (i = 0; i < rowsA; i++) 
        {
            for (j = 0; j < colsB; j++) 
            {
                int sum = 0;
                for (k = CSR_matr.IA[i]; k < (size_t)CSR_matr.IA[i + 1]; k++) 
                {
                    int colA = CSR_matr.JA[k];
                    for (size_t p = CSC_matr.JB[j]; p < (size_t)CSC_matr.JB[j + 1]; p++) 
                    {
                        if (CSC_matr.IB[p] == colA) 
                        {
                            sum += CSR_matr.A[k] * CSC_matr.B[p];
                            break;
                        }
                    }
                }
                if (sum != 0) 
                {
                    result_CSR_matr.A[nnz_count] = sum;
                    result_CSR_matr.JA[nnz_count] = j;
                    nnz_count++;
                }
            }
        }
        result_CSR_matr.non_zero = nnz_count;
    }

    free(temp_row);

    if (ec != SUCCESS_CODE) 
        free_result_csr_matr();

    return ec;
}

status_t multiply_dense_matrices(void)
{
    status_t ec = SUCCESS_CODE;

    if (dense_matr_1.cols != dense_matr_2.rows)
        return ERR_MULT;

    // если матрица существует освобождаем ее, чтобы перезаписать
    if (dense_matr_result.data)
        ec = free_dense_matrix(&dense_matr_result);

    if (ec == SUCCESS_CODE)
        ec = allocate_dense_matrix(&dense_matr_result, dense_matr_1.rows, dense_matr_2.cols);

    if (ec == SUCCESS_CODE)
        for (size_t i = 0; i < dense_matr_result.rows; i++)
        {
            for (size_t j = 0; j < dense_matr_result.cols; j++)
            {
                int sum = 0;
                for (size_t k = 0; k < dense_matr_1.cols; k++)
                    sum += dense_matr_1.data[i][k] * dense_matr_2.data[k][j];
                dense_matr_result.data[i][j] = sum;
            }
        }

    if (ec != SUCCESS_CODE && dense_matr_result.data)
        free_dense_matrix(&dense_matr_result);

    return ec;
}
