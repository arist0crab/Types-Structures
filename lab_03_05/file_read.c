#include "file_read.h"

status_t get_filename(char *filename);
status_t safe_open_file(char *filename, FILE **filestream);
status_t read_dimensions_from_file(FILE *filestream, size_t *rows, size_t *cols);
status_t read_non_zero_elements(FILE *filestream, int *non_zero_arr, size_t non_zero_quantity);
status_t read_indexes_array(FILE *filestream, int *indexes_array, size_t non_zero_quantity, int max_possible_index);
status_t read_max_non_zero_for_each_col_or_row(FILE *filestream, int *target_array, size_t range_ind, int non_zero_quantity);

status_t read_dense_from_file(dense_matrix_t *dense_matr)
{
    status_t ec = SUCCESS_CODE;
    size_t inputed_matrix_elems = 0;
    char filename[MAX_FILENAME_LENGTH];
    FILE *filestream = NULL;

    if (dense_matr == NULL)
        return ERR_MEM;

    free_dense_matrix(dense_matr);    

    // открываем файл 
    ec = get_filename(filename);
    if (ec == SUCCESS_CODE)
        ec = safe_open_file(filename, &filestream);

    // читаем размеры
    if (ec == SUCCESS_CODE)
        ec = read_dimensions_from_file(filestream, &dense_matr->rows, &dense_matr->cols);

    // выделяем память под матрицу
    if (ec == SUCCESS_CODE)
        ec = allocate_dense_matrix(dense_matr, dense_matr->rows, dense_matr->cols);
    
    // читаем элементы матрицы
    for (size_t i = 0; ec == SUCCESS_CODE && !feof(filestream) && i < dense_matr->rows; i++)
        for (size_t j = 0; ec == SUCCESS_CODE && !feof(filestream) && j < dense_matr->cols; j++)
            if (fscanf(filestream, "%d", &dense_matr->data[i][j]) != 1)
                ec = ERR_IO;
            else 
                inputed_matrix_elems++;

    // если прочитали меньше, чем нужно, то ошибка
    if (ec == SUCCESS_CODE && inputed_matrix_elems != (dense_matr->rows * dense_matr->cols))
        ec = ERR_IO;
    
    matrices_initialized_quantity += (ec == SUCCESS_CODE);
    
    if (filestream != NULL)
        fclose(filestream);

    return ec;
}

status_t read_csr_from_file(CSR_matrix_t *csr_matrix)
{
    status_t ec = SUCCESS_CODE;
    char filename[MAX_FILENAME_LENGTH];
    FILE *filestream = NULL;

    if (csr_matrix == NULL) 
        return ERR_MEM;

    free_csr_matr();

    // открываем файл 
    ec = get_filename(filename);
    if (ec == SUCCESS_CODE)
        ec = safe_open_file(filename, &filestream);

    // читаем размеры
    if (ec == SUCCESS_CODE)
        ec = read_dimensions_from_file(filestream, &csr_matrix->rows, &csr_matrix->cols);

    // считываем кол-во ненулевых
    if (ec == SUCCESS_CODE)
        if (fscanf(filestream, "%lu", &csr_matrix->non_zero) != 1 || csr_matrix->non_zero == 0)
            ec = ERR_IO;

    if (ec == SUCCESS_CODE)
        ec = allocate_csr_matrix(csr_matrix->non_zero, csr_matrix->rows);

    if (ec == SUCCESS_CODE)
        ec = read_non_zero_elements(filestream, csr_matrix->A, csr_matrix->non_zero);

    if (ec == SUCCESS_CODE)
        ec = read_indexes_array(filestream, csr_matrix->JA, csr_matrix->non_zero, csr_matrix->rows);

    if (ec == SUCCESS_CODE)
        ec = read_max_non_zero_for_each_col_or_row(filestream, csr_matrix->IA, csr_matrix->rows, csr_matrix->non_zero);

    if (ec == SUCCESS_CODE)
        if (csr_matrix->IA[0] != 0 || csr_matrix->IA[csr_matrix->rows] != (int)csr_matrix->non_zero)
            ec = ERR_IO;

    // проверка монотонности IA
    if (ec == SUCCESS_CODE)
        for (size_t i = 1; i <= csr_matrix->rows && ec == SUCCESS_CODE; i++)
            if (csr_matrix->IA[i] < csr_matrix->IA[i - 1])
                ec = ERR_IO;

    matrices_initialized_quantity += (ec == SUCCESS_CODE);
    if (ec != SUCCESS_CODE)
        free_csr_matr();

    return ec;
}

status_t read_csc_from_file(CSC_matrix_t *csc_matrix)
{
    status_t ec = SUCCESS_CODE;
    char filename[MAX_FILENAME_LENGTH];
    FILE *filestream = NULL;

    if (csc_matrix == NULL) 
        return ERR_MEM;

    free_csr_matr();

    // открываем файл 
    ec = get_filename(filename);
    if (ec == SUCCESS_CODE)
        ec = safe_open_file(filename, &filestream);

    // читаем размеры
    if (ec == SUCCESS_CODE)
        ec = read_dimensions_from_file(filestream, &csc_matrix->rows, &csc_matrix->cols);

    // считываем кол-во ненулевых
    if (ec == SUCCESS_CODE)
        if (fscanf(filestream, "%lu", &csc_matrix->non_zero) != 1 || csc_matrix->non_zero == 0)
            ec = ERR_IO;

    if (ec == SUCCESS_CODE)
        ec = allocate_csc_matrix(csc_matrix->non_zero, csc_matrix->cols);

    if (ec == SUCCESS_CODE)
        ec = read_non_zero_elements(filestream, csc_matrix->B, csc_matrix->non_zero);

    if (ec == SUCCESS_CODE)
        ec = read_indexes_array(filestream, csc_matrix->IB, csc_matrix->non_zero, csc_matrix->cols);

    if (ec == SUCCESS_CODE)
        ec = read_max_non_zero_for_each_col_or_row(filestream, csc_matrix->JB, csc_matrix->cols, csc_matrix->non_zero);

    if (ec == SUCCESS_CODE)
        if (csc_matrix->JB[0] != 0 || csc_matrix->JB[csc_matrix->cols] != (int)csc_matrix->non_zero)
            ec = ERR_IO;

    // проверка монотонности IA
    if (ec == SUCCESS_CODE)
        for (size_t i = 1; i <= csc_matrix->rows && ec == SUCCESS_CODE; i++)
            if (csc_matrix->JB[i] < csc_matrix->JB[i - 1])
                ec = ERR_IO;

    matrices_initialized_quantity += (ec == SUCCESS_CODE);
    if (ec != SUCCESS_CODE)
        free_csc_matr();

    return ec;
}

status_t read_max_non_zero_for_each_col_or_row(FILE *filestream, int *target_array, size_t range_ind, int non_zero_quantity)
{
    status_t ec = SUCCESS_CODE;

    if (filestream == NULL)
        return ERR_FILE;
    else if (target_array == NULL)
        return ERR_MEM;

    for (size_t i = 0; ec == SUCCESS_CODE && i < range_ind + 1; i++)
        if (fscanf(filestream, "%d", &target_array[i]) != 1 || target_array[i] < 0 || target_array[i] > non_zero_quantity)
            ec = ERR_IO;

    return ec;
}

status_t read_non_zero_elements(FILE *filestream, int *non_zero_arr, size_t non_zero_quantity)
{
    status_t ec = SUCCESS_CODE;

    if (filestream == NULL)
        return ERR_FILE;
    else if (non_zero_arr == NULL)
        return ERR_MEM;

    for (size_t i = 0; ec == SUCCESS_CODE && i < non_zero_quantity; i++)
        if (fscanf(filestream, "%d", &non_zero_arr[i]) != 1)
            ec = ERR_IO;

    return ec;
}

status_t read_indexes_array(FILE *filestream, int *indexes_array, size_t non_zero_quantity, int max_possible_index)
{
    status_t ec = SUCCESS_CODE;

    if (filestream == NULL)
        return ERR_FILE;
    else if (indexes_array == NULL)
        return ERR_MEM;

    for (size_t i = 0; ec == SUCCESS_CODE && i < non_zero_quantity; i++)
        if (fscanf(filestream, "%d", &indexes_array[i]) != 1 || indexes_array[i] < 0 || indexes_array[i] > max_possible_index)
            ec = ERR_IO;

    return ec;
}

status_t read_dimensions_from_file(FILE *filestream, size_t *rows, size_t *cols)
{
    status_t ec = SUCCESS_CODE;
    int temp1, temp2;

    if (filestream == NULL)
        ec = ERR_FILE;
    else if (rows == NULL || cols == NULL)
        ec = ERR_MEM;

    if (ec == SUCCESS_CODE)
    {
        rewind(filestream);
        if (fscanf(filestream, "%d", &temp1) != 1 || temp1 <= 0)
            ec = ERR_IO;
        if (ec == SUCCESS_CODE && (fscanf(filestream, "%d", &temp2) != 1 || temp2 <= 0))
            ec = ERR_IO;
    }

    if (ec == SUCCESS_CODE) 
    {
        *rows = (size_t)temp1;
        *cols = (size_t)temp2;
    }

    return ec;
}

status_t get_filename(char *filename)
{
    status_t ec = SUCCESS_CODE;

    if (filename == NULL)
        return ERR_MEM;

    printf("%sВведите имя файла: %s", BLUE, RESET);
    if (scanf("%s", filename) != 1)
        ec = ERR_IO;

    return ec;
}

status_t safe_open_file(char *filename, FILE **filestream)
{
    status_t ec = SUCCESS_CODE;

    if (filename == NULL)
        return ERR_FILE;

    if (*filestream)
    {
        fclose(*filestream);
        *filestream = NULL;
    }

    *filestream = fopen(filename, "r");
    if (*filestream == NULL)
        ec = ERR_FILE;

    return ec;
}