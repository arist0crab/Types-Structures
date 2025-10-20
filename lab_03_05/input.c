#include "input.h"

// ввод матриц
status_t input_dense_matr(dense_matrix_t *dense_matr);
status_t input_csr_matrix(void);
status_t input_csc_matrix(void);

// вспомогательные функции для ввода матриц
status_t input_non_zero_quantity(size_t *non_zero_quantity, size_t max_non_zero_quantity);
status_t input_non_zero_elements(int *non_zero_arr, size_t non_zero_quantity);
status_t input_indexes_array(int *indexes_array, size_t non_zero_quantity, int max_possible_index);
status_t input_max_non_zero_for_each_col_or_row(int *target_array, size_t range_ind, int non_zero_quantity);

status_t input_cur_menu_opt(menu_option_t *cur_menu_opt)
{
    int temp;  
    printf("%sВыберите пункт меню: %s", BLUE, RESET);

    if (scanf("%d", &temp) != 1 || temp < 0 || temp > 5)
        return ERR_IO;
    
    *cur_menu_opt = (menu_option_t)temp;
    return SUCCESS_CODE;
}

status_t input_any_matrix(void)
{
    status_t ec = SUCCESS_CODE;
    int matrix_type_option, input_type_option;

    printf("%s", BLUE);
    printf("Какую матрицу вы хотите ввести?\n");
    printf("0 - Матрицу в обычном представлении №1\n");
    printf("1 - Матрицу в обычном представлении №2\n");
    printf("2 - CSR-матрицу\n");
    printf("3 - CSC-матрицу\n");
    printf("%s", RESET);

    if (scanf("%d", &matrix_type_option) != 1 || matrix_type_option < 0 || matrix_type_option > 3)
        return ERR_IO;

    printf("%s", BLUE);
    printf("Каким способом вы хотите ввести матрицу?\n");
    printf("0 - ввести с клавиатуры\n");
    printf("1 - считать с файла\n");
    printf("%s", RESET);

    if (scanf("%d", &input_type_option) != 1 || input_type_option < 0 || input_type_option > 1)
        return ERR_IO;

    if (input_type_option == 0)
    {
        // ввод с клавиатуры
        if (matrix_type_option == 0)
            ec = input_dense_matr(&dense_matr_1);
        else if (matrix_type_option == 1)
            ec = input_dense_matr(&dense_matr_2);
        else if (matrix_type_option == 2)
            ec = input_csr_matrix();
        else if (matrix_type_option == 3)
            ec = input_csc_matrix();
        else
            ec = UNKNOWN_ERROR;
    }
    else if (input_type_option == 1)
    {
        // чтение из файла
        if (matrix_type_option == 0)
            ec = read_dense_from_file(&dense_matr_1);
        else if (matrix_type_option == 1)
            ec = read_dense_from_file(&dense_matr_2);
        else if (matrix_type_option == 2)
            ec = read_csr_from_file(&CSR_matr);
        else if (matrix_type_option == 3)
            ec = read_csc_from_file(&CSC_matr);
        else
            ec = UNKNOWN_ERROR;
    }
    else 
        ec = UNKNOWN_ERROR;

    return ec;
}

status_t input_csr_matrix(void)
{
    status_t ec = SUCCESS_CODE;

    if (CSR_matr.A)
        free_csr_matr();

    ec = input_matrix_dimensions(&CSR_matr.rows, &CSR_matr.cols);
    if (ec == SUCCESS_CODE)
        ec = input_non_zero_quantity(&CSR_matr.non_zero, CSR_matr.rows * CSR_matr.cols);

    if (ec == SUCCESS_CODE)
        ec = allocate_csr_matrix(CSR_matr.non_zero, CSR_matr.rows);

    if (ec == SUCCESS_CODE)
        ec = input_non_zero_elements(CSR_matr.A, CSR_matr.non_zero);

    if (ec == SUCCESS_CODE)
        ec = input_indexes_array(CSR_matr.JA, CSR_matr.non_zero, CSR_matr.rows);

    if (ec == SUCCESS_CODE)
        ec = input_max_non_zero_for_each_col_or_row(CSR_matr.IA, CSR_matr.rows, CSR_matr.non_zero);

    if (ec == SUCCESS_CODE)
        if (CSR_matr.IA[0] != 0 || CSR_matr.IA[CSR_matr.rows] != (int)CSR_matr.non_zero)
            ec = ERR_IO;

    // проверка монотонности IA
    if (ec == SUCCESS_CODE)
        for (size_t i = 1; i <= CSR_matr.rows && ec == SUCCESS_CODE; i++)
            if (CSR_matr.IA[i] < CSR_matr.IA[i - 1])
                ec = ERR_IO; 

    matrices_initialized_quantity += 1;
    if (ec != SUCCESS_CODE) 
        free_csr_matr();

    return ec;
}

status_t input_csc_matrix(void)
{
    status_t ec = SUCCESS_CODE;

    if (CSC_matr.B)
        free_csc_matr();

    ec = input_matrix_dimensions(&CSC_matr.rows, &CSC_matr.cols);
    if (ec == SUCCESS_CODE)
        ec = input_non_zero_quantity(&CSC_matr.non_zero, CSC_matr.rows * CSC_matr.cols);

    if (ec == SUCCESS_CODE)
        ec = allocate_csc_matrix(CSC_matr.non_zero, CSC_matr.cols);

    if (ec == SUCCESS_CODE)
        ec = input_non_zero_elements(CSC_matr.B, CSC_matr.non_zero);

    if (ec == SUCCESS_CODE)
        ec = input_indexes_array(CSC_matr.IB, CSC_matr.non_zero, CSC_matr.cols);

    if (ec == SUCCESS_CODE)
        ec = input_max_non_zero_for_each_col_or_row(CSC_matr.JB, CSC_matr.cols, CSC_matr.non_zero);

    if (ec == SUCCESS_CODE)
        if (CSC_matr.JB[0] != 0 || CSC_matr.JB[CSC_matr.cols] != (int)CSC_matr.non_zero)
            ec = ERR_IO;

    // проверка монотонности JB
    if (ec == SUCCESS_CODE)
        for (size_t i = 1; i <= CSC_matr.cols && ec == SUCCESS_CODE; i++)
            if (CSC_matr.JB[i] < CSC_matr.JB[i - 1])
                ec = ERR_IO; 

    matrices_initialized_quantity += 1;
    if (ec != SUCCESS_CODE) 
        free_csc_matr();

    return ec;
}

status_t input_dense_matr(dense_matrix_t *dense_matr)
{
    status_t ec = SUCCESS_CODE;

    // если матрица существует освобождаем ее, чтобы перезаписать
    if (dense_matr->data)
        free_dense_matrix(dense_matr);

    ec = input_matrix_dimensions(&dense_matr->rows, &dense_matr->cols);
    if (ec == SUCCESS_CODE)
        ec = allocate_dense_matrix(dense_matr, dense_matr->rows, dense_matr->cols);

    for (size_t i = 0; ec == SUCCESS_CODE && i < dense_matr->rows; i++)
        for (size_t j = 0; ec == SUCCESS_CODE && j < dense_matr->cols; j++)
        {
            printf("%sЭлемент [%ld][%ld]:%s ", BLUE, i, j, RESET);
            if (scanf("%d", &dense_matr->data[i][j]) != 1)
                ec = ERR_IO;
        }   

    matrices_initialized_quantity += 1;
    if (ec != SUCCESS_CODE) 
        free_dense_matrix(dense_matr);

    return ec;
}

status_t input_matrix_dimensions(size_t *rows, size_t *cols)
{   
    status_t ec = SUCCESS_CODE;
    int temp_size_1, temp_size_2;

    printf("%sВведите размеры матрицы:\n%s", BLUE, RESET);
    printf("%sСтроки:%s ", BLUE, RESET);

    if (scanf("%d", &temp_size_1) != 1 || temp_size_1 <= 0 || temp_size_1 > MAX_MATRIX_SIZE)
        ec = ERR_RANGE;

    if (ec == SUCCESS_CODE)
    {
        printf("%sСтолбцы:%s ", BLUE, RESET);
        if (scanf("%d", &temp_size_2) != 1 || temp_size_2 <= 0 || temp_size_2 > MAX_MATRIX_SIZE)
            ec = ERR_RANGE;
    }
    
    if (ec == SUCCESS_CODE)
    {
        *rows = (size_t)temp_size_1;
        *cols = (size_t)temp_size_2;
    }

    return ec;
}

status_t input_non_zero_quantity(size_t *non_zero_quantity, size_t max_non_zero_quantity)
{
    status_t ec = SUCCESS_CODE;
    int temp_non_zero;

    printf("%sВведите количество значимых элементов: %s", BLUE, RESET);
    if (scanf("%d", &temp_non_zero) != 1 || temp_non_zero <= 0 || (size_t)temp_non_zero > max_non_zero_quantity)
        ec = ERR_RANGE;
    if (ec == SUCCESS_CODE)
        *non_zero_quantity = (size_t)temp_non_zero;

    return ec;
}

status_t input_non_zero_elements(int *non_zero_arr, size_t non_zero_quantity)
{
    status_t ec = SUCCESS_CODE;

    printf("%sВведите значимые элементы матрицы:\n%s", BLUE, RESET);
    for (size_t i = 0; ec == SUCCESS_CODE && i < non_zero_quantity; i++)
    {
        printf("%sЭлемент №%ld%s: ", BLUE, i + 1, RESET);
        if (scanf("%d", &non_zero_arr[i]) != 1)
            ec = ERR_IO;
    }

    return ec;
}

status_t input_indexes_array(int *indexes_array, size_t non_zero_quantity, int max_possible_index)
{
    status_t ec = SUCCESS_CODE;

    printf("%sВведите индекс столбца/строки для каждого элемента:\n%s", BLUE, RESET);
    for (size_t i = 0; ec == SUCCESS_CODE && i < non_zero_quantity; i++)
    {
        printf("%sИндекс №%ld%s: ", BLUE, i + 1, RESET);
        if (scanf("%d", &indexes_array[i]) != 1 || indexes_array[i] < 0 || indexes_array[i] > max_possible_index)
            ec = ERR_IO;
    }

    return ec;
}

status_t input_max_non_zero_for_each_col_or_row(int *target_array, size_t range_ind, int non_zero_quantity)
{
    status_t ec = SUCCESS_CODE;

    printf("%sВведите кол-во значимых элементов для каждого столбца/строки:\n%s", BLUE, RESET);
    for (size_t i = 0; ec == SUCCESS_CODE && i < range_ind + 1; i++)
    {
        printf("%sЭлемент №%ld%s: ", BLUE, i + 1, RESET);
        if (scanf("%d", &target_array[i]) != 1 || target_array[i] < 0 || target_array[i] > non_zero_quantity)
            ec = ERR_IO;
    }

    return ec;
}