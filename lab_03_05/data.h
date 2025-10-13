#ifndef __DATA_H__
#define __DATA_H__

#define MAX_STR_LEN 256
#define MAX_MATRIX_SIZE 1000

typedef enum
{
    SUCCESS_CODE,
    ERR_IO,  // ошибка ввода-вывода
    ERR_RANGE,  // ошибка обработки массива или кол-ва чего-либо
    ERR_MEM,  // ошибка при работе с памятью
    ERR_FILE,  // ошибка при работе с файлом
    UNKNOWN_ERROR  // неизвестная ошибка
} status_t;

typedef enum 
{
    EXIT_PROGRAM,
    PRINT_MATR,
    INPUT_MATR,
    CSR_CSC_MATR_MULT,
    COMMON_MATR_MULT,
    COMPARE_ALGORITHMS
} menu_option_t;

#endif