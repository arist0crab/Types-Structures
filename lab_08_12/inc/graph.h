#ifndef __GRAPH_T__
#define __GRAPH_T__

#include "../inc/data.h"  // TODO
#include "auxilary.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char **cities_names;  // названия городов
    char *capital;  // название столицы
    size_t **roads;  // длины дорог
    size_t cities_quantity;  // количество городов
    size_t max_vertices_quantity;  // максимальное кол-во вершин графа
    size_t t_distance;  // TODO
} graph_t;

// TODO написать doxygen
status_t free_graph(graph_t *graph);
status_t clear_graph(graph_t *graph);
status_t set_graph_capital(graph_t *graph, const char *capital);
status_t set_graph_t_distance(graph_t *graph, size_t t_distance);

#endif