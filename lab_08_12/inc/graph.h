#ifndef __GRAPH_T__
#define __GRAPH_T__

#include "../inc/data.h"  // TODO
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    char **cities_names;  // названия городов
    char *capital;  // название столицы
    size_t **roads;  // длины дорог
    size_t cities_quantity;  // количество городов
    size_t max_vertices_quantity;  // максимальное кол-во вершин графа
    size_t target_distance;  // TODO
} graph_t;

// TODO написать doxygen
status_t free_graph(graph_t *graph);

#endif