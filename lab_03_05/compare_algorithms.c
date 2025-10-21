#define _POSIX_C_SOURCE 199309L
#include "compare_algorithms.h"
#include <time.h>

status_t calc_csr_matr_memory(CSR_matrix_t *csr_matr, size_t *memory_result);
status_t calc_csc_matr_memory(CSC_matrix_t *csc_matr, size_t *memory_result);
status_t calc_dense_matr_memory(dense_matrix_t *dense_matr, size_t *memory_result);
bool position_exists(size_t *positions, size_t count, size_t pos);

status_t compare_matrix_multiplication(void)
{
    status_t ec = SUCCESS_CODE;
    size_t common_rows, common_cols;
    size_t cur_non_zero_quantity;
    size_t dense_1_memory, dense_2_memory;
    size_t csr_memory, csc_memory;
    struct timespec start_mesuare, end_measure;
    long long global_dense_matr_time, global_sparse_matr_time;
    long long global_dense_memory, global_sparse_memory;

    ec = input_matrix_dimensions(&common_rows, &common_cols);
    if (ec == SUCCESS_CODE && common_rows != common_cols)
        ec = ERR_MULT;

    if (ec != SUCCESS_CODE)
        return ec;

    // сбрасываем все матрицы
    free_all_matr();

    // выделяем память под обычные матрицы
    ec = allocate_dense_matrix(&dense_matr_1, common_rows, common_cols);
    if (ec == SUCCESS_CODE)
        ec = allocate_dense_matrix(&dense_matr_2, common_rows, common_cols);

    if (ec == SUCCESS_CODE)
    {
        printf("+---------------------------------------------------------------------+\n");
        printf("| filling %% | dense time | sparse time | dense memory | sparse memory |\n");
        printf("+---------------------------------------------------------------------+\n");
    }
    
    for (size_t cur_fill_percent = 10; ec == SUCCESS_CODE && cur_fill_percent <= 100; cur_fill_percent += 10)
    {
        cur_non_zero_quantity = (size_t)(common_rows * common_cols / 100.00 * cur_fill_percent);
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

        global_dense_matr_time = 0;
        global_sparse_matr_time = 0;
        global_dense_memory = 0;
        global_sparse_memory = 0;

        // для каждого значения процента делаем замеры по 100 раз
        for (size_t i = 0; ec == SUCCESS_CODE && i < COMPARE_ITERATIONS_QUANITY; i++)
        {
            // заполняем матрицы
            fill_random_csr(&CSR_matr, cur_non_zero_quantity);
            fill_random_csc(&CSC_matr, cur_non_zero_quantity);
            fill_random_dense(&dense_matr_1, cur_non_zero_quantity);
            fill_random_dense(&dense_matr_2, cur_non_zero_quantity);

            // перемножаем и замеряем время для плотных матриц
            clock_gettime(CLOCK_MONOTONIC, &start_mesuare);
            multiply_dense_matrices();
            clock_gettime(CLOCK_MONOTONIC, &end_measure);
            global_dense_matr_time += (end_measure.tv_sec - start_mesuare.tv_sec) * 1000000000LL + (end_measure.tv_nsec - start_mesuare.tv_nsec);

            // перемножаем и замеряем время для плотных матриц
            clock_gettime(CLOCK_MONOTONIC, &start_mesuare);
            multiply_csr_and_csc();
            clock_gettime(CLOCK_MONOTONIC, &end_measure);
            global_sparse_matr_time += (end_measure.tv_sec - start_mesuare.tv_sec) * 1000000000LL + (end_measure.tv_nsec - start_mesuare.tv_nsec);           
        }

        global_dense_matr_time /= COMPARE_ITERATIONS_QUANITY;
        global_sparse_matr_time /= COMPARE_ITERATIONS_QUANITY;

        // считаем занимаемую память для разреженных матриц
        calc_csr_matr_memory(&CSR_matr, &csr_memory);
        calc_csc_matr_memory(&CSC_matr, &csc_memory);
        global_sparse_memory = csr_memory + csc_memory;

        // считаем занимаемую память для плотных матриц
        calc_dense_matr_memory(&dense_matr_1, &dense_1_memory);
        calc_dense_matr_memory(&dense_matr_2, &dense_2_memory);
        global_dense_memory = dense_1_memory + dense_2_memory;

        printf("| %9ld | %10lld | %11lld | %12lld | %13lld |\n", cur_fill_percent, global_dense_matr_time, global_sparse_matr_time, global_dense_memory, global_sparse_memory);
        printf("+---------------------------------------------------------------------+\n");

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

status_t fill_random_dense(dense_matrix_t *mat, size_t num_random)
{
    if (!mat || !mat->data)
        return ERR_MEM;
    if (mat->rows == 0 || mat->cols == 0)
        return ERR_RANGE;

    size_t max_elements = mat->rows * mat->cols;
    if (num_random > max_elements)
        num_random = max_elements;

    // обнуляем матрицу
    for (size_t i = 0; i < mat->rows; i++)
        for (size_t j = 0; j < mat->cols; j++)
            mat->data[i][j] = 0;

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

    // заполняем случайными числами от 1 до 100
    for (size_t k = 0; k < num_random; k++) {
        size_t row = positions[k] / mat->cols;
        size_t col = positions[k] % mat->cols;
        mat->data[row][col] = rand() % 100 + 1;
    }

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

status_t calc_csr_matr_memory(CSR_matrix_t *csr_matr, size_t *memory_result)
{
    if (!memory_result || !csr_matr)
        return ERR_MEM;

    *memory_result = sizeof(CSR_matrix_t) + csr_matr->non_zero * sizeof(int) * 2 + (csr_matr->rows + 1) * sizeof(int);
    return SUCCESS_CODE;
}

status_t calc_csc_matr_memory(CSC_matrix_t *csc_matr, size_t *memory_result)
{
    if (!memory_result || !csc_matr)
        return ERR_MEM;

    *memory_result = sizeof(CSC_matrix_t) + csc_matr->non_zero * sizeof(int) * 2 + (csc_matr->cols + 1) * sizeof(int);
    return SUCCESS_CODE;
}

status_t calc_dense_matr_memory(dense_matrix_t *dense_matr, size_t *memory_result)
{
    if (!dense_matr || !memory_result)
        return ERR_MEM;

    *memory_result = sizeof(dense_matrix_t) + dense_matr->rows * sizeof(int*) + dense_matr->rows * dense_matr->cols * sizeof(int);
    return SUCCESS_CODE;
}