#define _POSIX_C_SOURCE 199309L
#include "compare_algorithms.h"
#include <time.h>

status_t fill_random_csr(CSR_matrix_t *mat, size_t num_random);
status_t fill_random_csc(CSC_matrix_t *mat, size_t num_random);
bool position_exists(size_t *positions, size_t count, size_t pos);

status_t compare_matrix_multiplication(void)
{
    status_t ec = SUCCESS_CODE;
    size_t common_rows, common_cols;
    size_t cur_non_zero_quantity;

    ec = input_matrix_dimensions(&common_rows, &common_cols);
    if (ec != SUCCESS_CODE)
        return ec;

    // сбрасываем все матрицы
    free_all_matr();

    // выделяем память под обычные матрицы
    // ec = allocate_dense_matrix(&dense_matr_1, common_rows, common_cols);
    // if (ec == SUCCESS_CODE)
    //     ec = allocate_dense_matrix(&dense_matr_2, common_rows, common_cols);
    
    for (size_t cur_fill_percent = 10; cur_fill_percent <= 100; cur_fill_percent += 10)
    {
        cur_non_zero_quantity = common_rows * common_cols / 100 * cur_fill_percent;
        cur_non_zero_quantity = (!cur_non_zero_quantity) ? 1 : cur_non_zero_quantity;

        // выделяем память под CSR и CSC
        ec = allocate_csr_matrix(cur_non_zero_quantity, common_rows);
        if (ec == SUCCESS_CODE)
            ec = allocate_csc_matrix(cur_non_zero_quantity, common_cols);

        if (ec == SUCCESS_CODE)
        {
            CSR_matr.non_zero = cur_non_zero_quantity;
            CSC_matr.non_zero = cur_non_zero_quantity;
            CSR_matr.rows = common_rows, CSC_matr.rows = common_rows;
            CSR_matr.cols = common_cols, CSC_matr.cols = common_cols;
        }

        fill_random_csr(&CSR_matr, cur_non_zero_quantity);
        print_csr_matrix(&CSR_matr);
        fill_random_csc(&CSC_matr, cur_non_zero_quantity);
        print_csc_matrix(&CSC_matr);
        // TODO написать функцию заполнения dense матрицы
        // TODO организовать цикл для замеров времени и памяти

        free_csr_matr();
        free_csc_matr();
    }

    free_all_matr();

    return ec;
}

status_t fill_random_csr(CSR_matrix_t *mat, size_t num_random)
{
    if (!mat || !mat->A || !mat->JA || !mat->IA)
        return ERR_MEM;
    if (mat->rows == 0 || mat->cols == 0)
        return ERR_RANGE;

    size_t max_elements = mat->rows * mat->cols;
    if (num_random > max_elements)
        num_random = max_elements;

    for (size_t i = 0; i <= mat->rows; i++)
        mat->IA[i] = 0;

    size_t *positions = malloc(num_random * sizeof(size_t));
    if (!positions) 
        return ERR_MEM;

    srand((unsigned)time(NULL));

    // случайные уникальные позиции
    size_t count = 0;
    while (count < num_random) 
    {
        size_t pos = (size_t)(rand() % max_elements);
        if (!position_exists(positions, count, pos))
            positions[count++] = pos;
    }

    // cортируем позиции для построения CSR
    for (size_t i = 0; i < num_random - 1; i++) 
        for (size_t j = i + 1; j < num_random; j++) 
            if (positions[i] > positions[j]) 
            {
                size_t tmp = positions[i];
                positions[i] = positions[j];
                positions[j] = tmp;
            }

    for (size_t k = 0; k < num_random; k++) 
    {
        size_t row = positions[k] / mat->cols;
        size_t col = positions[k] % mat->cols;

        mat->A[k] = rand() % 100 + 1;
        mat->JA[k] = (int)col;
        mat->IA[row + 1]++;
    }

    // префиксная сумма IA
    for (size_t i = 1; i <= mat->rows; i++)
        mat->IA[i] += mat->IA[i - 1];

    free(positions);

    return SUCCESS_CODE;
}

status_t fill_random_csc(CSC_matrix_t *mat, size_t num_random)
{
    if (!mat || !mat->B || !mat->IB || !mat->JB)
        return ERR_MEM;
    if (mat->rows == 0 || mat->cols == 0)
        return ERR_RANGE;

    size_t max_elements = mat->rows * mat->cols;
    if (num_random > max_elements)
        num_random = max_elements;

    for (size_t j = 0; j <= mat->cols; j++)
        mat->JB[j] = 0;

    size_t *positions = malloc(num_random * sizeof(size_t));
    if (!positions) 
        return ERR_MEM;

    srand((unsigned)time(NULL));

    size_t count = 0;
    while (count < num_random) 
    {
        size_t pos = (size_t)(rand() % max_elements);
        if (!position_exists(positions, count, pos))
            positions[count++] = pos;
    }

    // cортируем позиции для построения CSC
    for (size_t i = 0; i < num_random - 1; i++)
        for (size_t j = i + 1; j < num_random; j++) 
        {
            size_t col_i = positions[i] % mat->cols;
            size_t col_j = positions[j] % mat->cols;
            if (col_i > col_j || (col_i == col_j && positions[i] > positions[j])) 
            {
                size_t tmp = positions[i];
                positions[i] = positions[j];
                positions[j] = tmp;
            }
        }

    for (size_t k = 0; k < num_random; k++) 
    {
        size_t row = positions[k] / mat->cols;
        size_t col = positions[k] % mat->cols;

        mat->B[k] = rand() % 100 + 1;
        mat->IB[k] = (int)row;
        mat->JB[col + 1]++;
    }

    // префиксная сумма JB
    for (size_t j = 1; j <= mat->cols; j++)
        mat->JB[j] += mat->JB[j - 1];

    free(positions);

    return SUCCESS_CODE;
}


bool position_exists(size_t *positions, size_t count, size_t pos)
{
    for (size_t i = 0; i < count; i++)
        if (positions[i] == pos)
            return true;
    return false;
}
