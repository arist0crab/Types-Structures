#ifndef __DATA_H__
#define __DATA_H__

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
    MULT_SPARSE_MATR_BY_VECTOR,
    MULT_FULL_MATR_BY_VECTOR,
    PRINT_MATR,
    COMPARE_ALGORITHMS
} menu_option_t;

#endif