#ifndef __AUXILIARY_H__
#define __AUXILIARY_H__

#include "data.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/** @brief Функция для копирования строки s. Динамически выделяет память 
 * для строки, аналогичной строке s и возвращает указатель на новую строку.
 * @param[in] s Указатель на начало исходной строки.
 * @return Указатель на новую скопированную строку в случае успеха, иначе NULL.
 */
char *str_dynamic_copy(const char *s);

/** @brief Функция для сравнения двух строк без учета их регистра на основе
 * латинницы и кириллицы.
 * @param[in] s1 Первая строка для сравнения.
 * @param[in] s2 Вторая строка для сравнения.
 * @return Целое число по аналогии со strcmp.
*/
int str_iequal(const char *s1, const char *s2);

/** @brief Функция для чтения значения размера (ч-л) из файла.
 * @param[in] filestream Файловый поток из которого необходимо считать данные.
 * @param[out] size Указатель на значение, которое должно быть считано из файла.
 * @return Статус выполнения.
*/
status_t read_size_from_file(FILE *filestream, size_t *size);

/** @brief Функция для нахождения указателя на начало выделенного блока памяти. 
 * @param[in] Матрица значений, блок памяти, указатель на начало которого необходимо найти.
 * @param[in] Количество строк матрицы значений.
 * @param[out] Указатель на указатель на начало блока памяти.
 * @return Статус выполнения.
*/
status_t find_start_matrix_data_ptr(size_t **matrix, size_t matrix_rows_quantity, size_t **matrix_data_start_ptr);

#endif