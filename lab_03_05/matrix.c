#include "matrix.h"

status_t free_all_matr(void);
status_t free_dense_matrix(dense_matrix_t *dense_matrix);
status_t free_csr_matr(void);
status_t free_csc_matr(void);
status_t allocate_dense_matrix(dense_matrix_t *dense_matr, size_t n, size_t m);

// инициализируем глобальные структуры
dense_matrix_t dense_matr_1 = {0}, dense_matr_2 = {0};
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
    free_result_csr_matr();
    free_csr_matr();
    free_csc_matr();

    matrices_initialized_quantity = 0;

    return SUCCESS_CODE;
}


// FIXME не всегда правильно работает, например для слуая:
/*
   CSR                CSC
[0, 0, 5]          
[0, 0, 0]          [0, 0, 8]
[7, 0, 0]    *     [0, 0, 0]
[0, 0, 0]          [0, 9, 0]
*/
status_t multiply_csr_and_csc(void)
{
    status_t ec = SUCCESS_CODE;

    // Все указатели и переменные в начале
    size_t n_rows_A = 0, n_cols_B = 0, n_common = 0;
    size_t max_non_zero_quantity = 0, result_ind = 0;

    size_t *B_row_count = NULL;
    size_t *B_row_ptr = NULL;
    int *B_rows_cols = NULL;
    int *B_rows_vals = NULL;
    size_t *pos = NULL;
    int *row_result = NULL;

    // Проверка входных данных
    if (!CSR_matr.A || !CSR_matr.IA || !CSR_matr.JA ||
        !CSC_matr.B || !CSC_matr.IB || !CSC_matr.JB ||
        CSR_matr.cols == 0 || CSR_matr.rows == 0 || CSR_matr.non_zero == 0 ||
        CSC_matr.cols == 0 || CSC_matr.rows == 0 || CSC_matr.non_zero == 0)
    {
        return UNKNOWN_ERROR;
    }

    if (CSR_matr.cols != CSC_matr.rows)
        return ERR_MULT;

    n_rows_A = CSR_matr.rows;
    n_cols_B = CSC_matr.cols;
    n_common = CSR_matr.cols;

    // Подсчёт количества элементов в строках B (на основе CSC)
    B_row_count = (size_t*)calloc(n_common, sizeof(size_t));
    if (B_row_count == NULL)
    {
        ec = ERR_MEM;
    }

    if (ec == SUCCESS_CODE)
    {
        for (size_t col = 0; col < n_cols_B && ec == SUCCESS_CODE; ++col)
        {
            size_t start = CSC_matr.JB[col];
            size_t end = CSC_matr.JB[col + 1];

            for (size_t p = start; p < end; ++p)
            {
                size_t row = (size_t)CSC_matr.IB[p];
                if (row >= n_common)
                {
                    ec = UNKNOWN_ERROR;
                    break;
                }
                B_row_count[row]++;
            }
        }
    }

    // Индексные указатели строк B
    if (ec == SUCCESS_CODE)
    {
        B_row_ptr = (size_t*)calloc(n_common + 1, sizeof(size_t));
        if (B_row_ptr == NULL)
            ec = ERR_MEM;
    }

    if (ec == SUCCESS_CODE)
    {
        for (size_t i = 1; i <= n_common; ++i)
            B_row_ptr[i] = B_row_ptr[i - 1] + B_row_count[i - 1];

        B_rows_cols = (int*)calloc(CSC_matr.non_zero, sizeof(int));
        B_rows_vals = (int*)calloc(CSC_matr.non_zero, sizeof(int));
        pos = (size_t*)malloc(n_common * sizeof(size_t));

        if (!B_rows_cols || !B_rows_vals || !pos)
            ec = ERR_MEM;
    }

    if (ec == SUCCESS_CODE)
    {
        for (size_t i = 0; i < n_common; ++i)
            pos[i] = B_row_ptr[i];

        for (size_t col = 0; col < n_cols_B; ++col)
        {
            size_t start = CSC_matr.JB[col];
            size_t end = CSC_matr.JB[col + 1];
            for (size_t p = start; p < end; ++p)
            {
                size_t row = (size_t)CSC_matr.IB[p];
                size_t dst = pos[row]++;
                B_rows_cols[dst] = (int)col;
                B_rows_vals[dst] = CSC_matr.B[p];
            }
        }
    }

    if (ec == SUCCESS_CODE)
    {
        max_non_zero_quantity = n_rows_A * n_cols_B;
        free_result_csr_matr();

        result_CSR_matr.A = (int *)calloc(max_non_zero_quantity, sizeof(int));
        result_CSR_matr.JA = (int *)calloc(max_non_zero_quantity, sizeof(int));
        result_CSR_matr.IA = (int *)calloc((n_rows_A + 1), sizeof(int));

        if (!result_CSR_matr.A || !result_CSR_matr.JA || !result_CSR_matr.IA)
            ec = ERR_MEM;
    }

    if (ec == SUCCESS_CODE)
    {
        row_result = (int*)malloc(n_cols_B * sizeof(int));
        if (!row_result)
            ec = ERR_MEM;
    }

    if (ec == SUCCESS_CODE)
    {
        result_ind = 0;
        result_CSR_matr.IA[0] = 0;

        for (size_t i = 0; i < n_rows_A; ++i)
        {
            memset(row_result, 0, n_cols_B * sizeof(int));

            size_t row_start = CSR_matr.IA[i];
            size_t row_end = CSR_matr.IA[i + 1];

            for (size_t p = row_start; p < row_end; ++p)
            {
                size_t k = (size_t)CSR_matr.JA[p];
                int val_a = CSR_matr.A[p];
                size_t b_start = B_row_ptr[k];
                size_t b_end = B_row_ptr[k + 1];
                for (size_t q = b_start; q < b_end; ++q)
                {
                    int col_b = B_rows_cols[q];
                    int val_b = B_rows_vals[q];
                    row_result[col_b] += val_a * val_b;
                }
            }

            for (size_t j = 0; j < n_cols_B; ++j)
            {
                if (row_result[j] != 0)
                {
                    if (result_ind >= max_non_zero_quantity)
                    {
                        size_t new_cap = max_non_zero_quantity * 2 + 16;
                        int *newA = (int*)realloc(result_CSR_matr.A, new_cap * sizeof(int));
                        int *newJA = (int*)realloc(result_CSR_matr.JA, new_cap * sizeof(int));
                        if (!newA || !newJA)
                        {
                            ec = ERR_MEM;
                            break;
                        }
                        result_CSR_matr.A = newA;
                        result_CSR_matr.JA = newJA;
                        max_non_zero_quantity = new_cap;
                    }

                    if (ec == SUCCESS_CODE)
                    {
                        result_CSR_matr.A[result_ind] = row_result[j];
                        result_CSR_matr.JA[result_ind] = (int)j;
                        result_ind++;
                    }
                }
            }

            result_CSR_matr.IA[i + 1] = result_ind;

            if (ec != SUCCESS_CODE)
                break;
        }
    }

    // финализация
    if (ec == SUCCESS_CODE)
    {
        result_CSR_matr.rows = n_rows_A;
        result_CSR_matr.cols = n_cols_B;
        result_CSR_matr.non_zero = result_ind;
    }

    // Централизованное освобождение
    free(B_row_count);
    free(B_row_ptr);
    free(B_rows_cols);
    free(B_rows_vals);
    free(pos);
    free(row_result);

    return ec;
}