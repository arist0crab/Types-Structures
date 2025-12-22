#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include "data.h"
#include "color.h"
#include "graph.h"
#include <stdio.h>

/** @brief Функция для вывода основного меню.
 * @return Статус выполнения.
*/
status_t print_menu(void);

/** @brief Функция для вывода дополнительного меню для ручной настройки графа, 
 * то есть для ручного добавления/удаления городов/дорог.
 * @return Статус выполнения.
*/
status_t print_manual_input_menu(void);

/** @brief Функция для вывода результата выполнения программы.
 * @param[in] exit_code Статус выполнения программы.
 * @return Статус выполнения.
*/
status_t print_exit_code_result(status_t exit_code);

/** @brief Функция для результата проверки графа на связность.
 * @param[in] if_graph_connected Результат проверки графа на связность.
 * @return Статус выполнения.
*/
status_t print_graph_connectivity_status(bool if_graph_connected);

/** @brief Функция для вывода результата выполнения алгоритма Дейкстры, т.е.
 * печатает кратчайшее расстояния между двумя городами.
 * @param[in] grpah Указатель на исходный граф.
 * @param[in] distances Массив расстояний.
 * @param[in] index_start_city Индекс первого города.
 * @param[in] index_end_sity Индекс второго города.
 * @return Статус выполнения.
*/
status_t print_dijkstra_result(graph_t *graph, const size_t *distances, size_t index_start_city, size_t index_end_sity);

/** @brief Функция для вывода городов, которые находятся от столицы дальше, чем
 * расстояние distance_t.
 * @param[in] graph Указатель на исходный граф.
 * @param[in] farther_t_distance_cities Массив индексов результирующий городов.
 * @param[in] farther_t_distance_cities_quantity Количество результирующих городов.
 * @return Статус выполнения.
*/
status_t print_farther_than_t_distance_cities(graph_t *graph, size_t *farther_t_distance_cities, size_t farther_t_distance_cities_quantity);

#endif