#ifndef __GRAPH_T__
#define __GRAPH_T__

#include "../inc/data.h"  // TODO
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

// TODO написать doxygen
status_t free_graph(graph_t *graph);
status_t set_graph_capital(graph_t *graph, const char *capital);
status_t add_city_to_graph(graph_t *graph, const char *city);
status_t get_cities_indexes(graph_t *graph, const char *city_1, const char *city_2, size_t  *indx1, size_t *indx2);
status_t add_road_to_graph(graph_t *graph, size_t index_city_1, size_t index_city_2, size_t distance_1_to_2, size_t distance_2_to_1);
status_t remove_road_from_graph(graph_t *graph, size_t index_city_1, size_t index_city_2);
status_t remove_city_from_graph(graph_t *graph, const char *city);
status_t export_graph_to_dot_file(graph_t *graph, const char *filename);
status_t init_graph(graph_t *graph, size_t cities_quantity);
status_t input_graph_from_file(graph_t *graph, FILE *filestream);
status_t dijkstra_graph(graph_t *graph, size_t src, size_t **dist);
status_t find_cities_farther_than_t_distance(graph_t *graph, size_t distance_t, size_t **far_cities, size_t *count);
status_t is_graph_connected(const graph_t *graph, bool *is_connected);

#endif