#include "output.h"

status_t print_menu(void)
{
    printf("+--------------------------------------------------+\n");
    printf("|                        Меню                      |\n");
    printf("+--------------------------------------------------+\n");
    printf("| 0  - Выйти из программы                          |\n");
    printf("| 1  - Вывести все существующие матрицы            |\n");
    printf("| 2  - Ввести какую-либо матрицу                   |\n");
    printf("| 3  - Перемножить матрицы форматов CSR и CSC      |\n");
    printf("| 4  - Перемножить матрицы в обычной форме         |\n");
    printf("| 5  - Сравнить алогритмы перемножения             |\n");
    printf("+--------------------------------------------------+\n");

    return SUCCESS_CODE;
}

status_t print_compare_algorithms_warning(void)
{
    printf("%s", BLUE);
    printf("===========  В Н И М А Н И Е  ===========\n");
    printf("В результате этого действия БУДУТ перезаписаны ВСЕ\n");
    printf("матрицы. Возможно, стоило предупредить об этом раньше, но ладно...\n");
    printf("%s", RESET);

    return SUCCESS_CODE;
}

status_t print_final_common_result(status_t exit_code)
{
    if (exit_code == SUCCESS_CODE)
        printf("%sПрограмма завершилась успешно!%s\n", GREEN_BOLD, RESET);
    else 
        printf("%sПрограмма завершилась с ошибкой: %d%s\n", RED_BOLD, exit_code, RESET);

    return SUCCESS_CODE;
}

status_t print_result(status_t exit_code)
{
    switch (exit_code)
    {
        case SUCCESS_CODE:
            printf("%sВсе прошло хорошо :)\n%s", GREEN_BOLD, RESET);
            break;

        case ERR_IO:
            printf("%sПроизошла ошибка ввода/вывода :(\n%s", RED_BOLD, RESET);
            break;

        case ERR_RANGE:
            printf("%sПроизошла ошибка обработки массива или обработки количества чего-либо :(\n%s", RED_BOLD, RESET);
            break;

        case ERR_MEM:
            printf("%sПроизошла ошибка при работе с памятью :(\n%s", RED_BOLD, RESET);
            break;

        case ERR_FILE:
            printf("%sПроизошла ошибка при работе с файлом :(\n%s", RED_BOLD, RESET);
            break;
        
        default:
            return UNKNOWN_ERROR;
            break;
    }

    printf("\n");

    return SUCCESS_CODE;
}

status_t print_all_matrices(void)
{
    if (matrices_initialized_quantity == 0)
        printf("У вас нет инициализированных матриц :(\n");
    else 
    {
        printf("%sМатриц инициализировано: %d%s\n", BLUE, matrices_initialized_quantity, RESET);
        
        if (dense_matr_1.data)
        {
            printf("%s=== Плотная матрица №1 ===%s\n", BLUE, RESET);
            print_dense_matrix(&dense_matr_1);
        }

        if (dense_matr_2.data)
        {
            printf("%s=== Плотная матрица №2 ===%s\n", BLUE, RESET);
            print_dense_matrix(&dense_matr_2);
        }

        if (CSR_matr.A)
        {
            printf("%s====== CSR матрица ======%s\n", BLUE, RESET);
            print_csr_matrix(&CSR_matr);
        }

        if (CSC_matr.B)
        {
            printf("%s====== CSC матрица ======%s\n", BLUE, RESET);
            print_csc_matrix(&CSC_matr);
        }

        if (result_CSR_matr.A)
        {
            printf("%s=== результирующая CSR матрица ===%s\n", BLUE, RESET);
            print_csr_matrix(&result_CSR_matr);
        }
    }

    return SUCCESS_CODE;
}

status_t print_dense_matrix(const dense_matrix_t *dense_matr)
{
    for (size_t i = 0; i < dense_matr->rows; i++)
    {
        for (size_t j = 0; j < dense_matr->cols; j++)
            printf("%d ", dense_matr->data[i][j]);
        printf("\n");
    }
    printf("\n");

    return SUCCESS_CODE;
}

status_t print_csr_matrix(const CSR_matrix_t *csr_matr)
{
    printf("Количество значимых элементов: %ld\n", csr_matr->non_zero);
    printf("Количество строк: %ld\n", csr_matr->rows);
    printf("Количество столбцов: %ld\n", csr_matr->cols);
    printf("A (значения): ");
    for (size_t i = 0; i < csr_matr->non_zero; i++)
        printf("%d ", csr_matr->A[i]);
    printf("\n");

    printf("JA (столбцы): ");
    for (size_t i = 0; i < csr_matr->non_zero; i++)
        printf("%d ", csr_matr->JA[i]);
    printf("\n");

    printf("IA (строки): ");
    for (size_t i = 0; i < csr_matr->rows + 1; i++)
        printf("%d ", csr_matr->IA[i]);
    printf("\n");

    return SUCCESS_CODE;
}

status_t print_csc_matrix(const CSC_matrix_t *csc_matr)
{
    printf("Количество значимых элементов: %ld\n", csc_matr->non_zero);
    printf("Количество строк: %ld\n", csc_matr->rows);
    printf("Количество столбцов: %ld\n", csc_matr->cols);
    printf("B (значения): ");
    for (size_t i = 0; i < csc_matr->non_zero; i++)
        printf("%d ", csc_matr->B[i]);
    printf("\n");

    printf("IB (строки): ");
    for (size_t i = 0; i < csc_matr->non_zero; i++)
        printf("%d ", csc_matr->IB[i]);
    printf("\n");

    printf("JB (столбцы): ");
    for (size_t i = 0; i < csc_matr->cols + 1; i++)
        printf("%d ", csc_matr->JB[i]);
    printf("\n");

    return SUCCESS_CODE;
}
