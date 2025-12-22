#ifndef __INPUT_H__
#define __INPUT_H__

#include "data.h" 
#include "color.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/** @brief Функция для ввода строки.
 * @param[in, out] word Указатель на слово, которое необходимо ввести.
 * @param[in] message Приглашение для ввода.
 * @return Статус выполнения.
*/
status_t input_string(char **word, const char *message);

/** @brief Функция для ввода размера чего-либо.
 * @param[out] size Указатель на размер, который необходимо ввести.
 * @param[in] message Приглашение для ввода.
 * @return Статус выполнения.
*/
status_t input_size(size_t *size, const char *message);

/** @brief Функция для ввода функции меню.
 * @param[out] cur_menu_opt Указатель на вводимую опцию меню.
 * @param[in] menu_points_quantity Число возможных опций меню. Т.е. вводимая опция меню
 * должна быть числом, меньшим чем menu_points_quantity.
 * @return Статус выполнения.
*/
status_t input_cur_menu_opt(int *cur_menu_opt, int const menu_points_quantity);

/** @brief Функция для получения двух названий городов, а также двух расстояний.
 * Получает название двух городов, а потом получает длину дороги из города №1 в
 * город №2, а потом из города №2 в город №1.
 * @param[out] city_name_1 Указатель на название первого города.
 * @param[out] city_name_2 Указатель на название второго города.
 * @param[out] distance_1_to_2 Длина дороги от города №1 до города №2.
 * @param[out] distance_2_to_1 Длина дороги от города №2 до города №1.
 * @return Статус выполнения.
*/
status_t get_two_cities_and_distances(char **city_name_1, char **city_name_2, size_t *distance_1_to_2, size_t *distance_2_to_1);

#endif