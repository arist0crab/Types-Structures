#ifndef __GRAPH_T__
#define __GRAPH_T__

#include "data.h"
#include "input.h"
#include "auxilary.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdint.h>
#include <limits.h>

typedef struct
{
    char **cities_names;  // названия городов
    char *capital;  // название столицы
    size_t **roads;  // длины дорог
    size_t cities_quantity;  // количество городов
    size_t max_vertices_quantity;  // максимальное кол-во вершин графа
} graph_t;

/** @brief Функция для ввода графа из файла (с помощью файлового потока filestream).
 * В процессе выполнения граф уничтожается, а затем перевыделяется снова для 
 * новых данных.
 * @param[in, out] Указатель на исходный граф.
 * @param[in] filestream Файловый поток, откуда будут считаны данные.
 * @return Статус выполнения.
*/
status_t input_graph_from_file(graph_t *graph, FILE *filestream);

/** @brief Функция для добавления города на граф.
 * @param[in, out] graph Указатель на исходный граф.
 * @param[in] city Название города, который необходимо добавить.
 * @return Статус выполнения.
 * 
*/
status_t add_city_to_graph(graph_t *graph, const char *city);

/** @brief Функция для добавления дороги на граф.
 * @param[in, out] Указатель на исходный граф.
 * @param[in] index_city_1 Индекс первого города у новой дороги.
 * @param[in] index_city_2 Индекс второго города у новой дороги.
 * @param[in] distance_1_to_2 Длина дороги от первого города ко второму.
 * @param[in] distance_2_to_1 Длина дороги от второго города к первому.
 * @return Статус выполнения.
*/
status_t add_road_to_graph(graph_t *graph, size_t index_city_1, size_t index_city_2, size_t distance_1_to_2, size_t distance_2_to_1);

/** @brief Функция для удаления дороги из графа.
 * Принимает индексы двух городов, дорогу между которыми необходимо удалить, 
 * затем ищет соответствующие ячейки в матрице "дорог" и зануляет их.
 * @param[in, out] graph Указатель на исходный граф.
 * @param[in] index_city_1 Индекс первого города в массиве имен и дорог.
 * @param[in] index_city_2 Индекс второго города в массиве имен и дорог.
 * @return Статус выполнения.
*/
status_t remove_road_from_graph(graph_t *graph, size_t index_city_1, size_t index_city_2);

/** @brief Функция для удаления города с графа.
 * @param[in, out] graph Указатель на исходный граф.
 * @param[in] city Название города, который необходимо удалить.
 * @return Статус выполнения.
*/
status_t remove_city_from_graph(graph_t *graph, const char *city);

/** @brief Функция для перевода графа (в его представлении структурой)
 * в dot-файл.
 * @param[in] graph Указатель на исходный граф.
 * @param[in, out] filename Имя для выходного dot-файла.
 * @return Статус выполнения.
*/
status_t export_graph_to_dot_file(graph_t *graph, const char *filename);

/** @brief Функция для поиска кратчаших расстояний между городами.
 * Ищет кратчашие расстояния между двумя городами, записывает результат в dist,
 * память для которого выделяется внутри функции (ее необходимо будет освободить).
 * @param[in] graph Указатель на исходный граф.
 * @param[in] src Индекс города, от которого ищутся расстояния.
 * @param[out] dist Указатель на указатель на массив расстояний.
 * @return Статус выполнения.
*/
status_t dijkstra_graph(graph_t *graph, size_t src, size_t **dist);

/** @brief Функция для поиска городов, расстояние до которых от указанного города
 * меньше расстояния distance_t.
 * @param[in] graph Указатель на исходный граф.
 * @param[in] distance_t Растояние Т, на основе которого осуществляется поиск.
 * @param[out] far_cities Массив индексов подходящих городов.
 * @param[out] count Указатель на количество подходящих городов.
 * @return Статус выполнения.
*/
status_t find_cities_farther_than_t_distance(graph_t *graph, size_t distance_t, size_t **far_cities, size_t *count);

/** @brief Функция для проверки связности графа.
 * @param[in] graph Указатель на исходный граф.
 * @param[out] is_connected Указатель на результат проверки связности.
 * @return Статус выполнения.
*/
status_t is_graph_connected(const graph_t *graph, bool *is_connected);

/** @brief Функция для обновления столицы графа.
 * @param[in, out] graph Указатель на исходный граф.
 * @param[in] capital Название города, который необходимо сделать столицей. Если
 * указанного города нет на графе, то он будет добавлен в список городов.
 * @return Статус выполнения.
*/
status_t set_graph_capital(graph_t *graph, const char *capital);

/** @brief Функция для получения индексов двух городов по их названиям.
 * @param[in] graph
 * @param[in] city_1 Название первого города для получения индекса.
 * @param[in] city_2 Название второго города для получения индекса.
 * @param[out] indx1 Указатель на индекс первого города.
 * @param[out] indx2 Указатель на индекс второго города.
 * @return Статус выполнения.
*/
status_t get_cities_indexes(graph_t *graph, const char *city_1, const char *city_2, size_t  *indx1, size_t *indx2);

/** @brief Функция для инициализации графа.
 * @param[in, out] graph Указатель на исходный граф.
 * @param[in] cities_quantity Количество городов, которое должно быть в новом графе.
 * @return Статус выполнения.
*/
status_t init_graph(graph_t *graph, size_t cities_quantity);

/** @brief Функция для уничтожения (освобождения) графа.
 * @param[in, out] graph Указатель на исходный граф.
 * @return Статус выполнения.
*/
status_t free_graph(graph_t *graph);

#endif