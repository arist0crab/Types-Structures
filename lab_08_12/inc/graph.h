#ifndef __GRAPH_T__
#define __GRAPH_T__

#include "../inc/data.h"  // TODO
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    char **cities_names;  // названия городов
    size_t **roads;  // длины дорог
    size_t cities_quantity;  // количество вершин графа
} graph_t;

#endif