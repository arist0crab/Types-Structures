#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include "data.h"
#include "color.h"
#include "graph.h"
#include <stdio.h>

// TODO добавить doxygen
status_t print_menu(void);
status_t print_exit_code_result(status_t exit_code);
status_t print_manual_input_menu(void);
status_t print_dijkstra_result(graph_t *graph, const size_t *distances, size_t index_start_city, size_t index_end_sity);
status_t print_farther_than_t_distance_cities(graph_t *graph, size_t *farther_t_distance_cities, size_t farther_t_distance_cities_quantity);
status_t print_graph_connectivity_status(bool if_graph_connected);

#endif