#include "graph.h"

status_t free_graph(graph_t *graph);
status_t find_start_matrix_data_ptr(const size_t **matrix, size_t matrix_rows_quantity, const size_t **matrix_data_start_ptr);

status_t init_graph(graph_t *graph, size_t cities_quantity)
{
    status_t ec = SUCCESS_CODE;
    size_t *roads_data = NULL;

    if (!graph || cities_quantity == 0) return ERR_ARGS;

    graph->cities_quantity = cities_quantity;
    graph->cities_names = NULL;
    graph->roads = NULL;
    
    // выделяем массив указателей на строки (массив слов)
    graph->cities_names = (char **)malloc(cities_quantity * sizeof(char *));
    if (graph->cities_names == NULL) ec = ERR_MEM;

    // инициализируем массив указателей
    for (size_t i = 0; ec == SUCCESS_CODE && i < cities_quantity; i++)
        graph->cities_names[i] = NULL;

    // выделяем массив указателей на rows матрицы (дороги)
    if (ec == SUCCESS_CODE)
    {
        graph->roads = (size_t **)calloc(cities_quantity, sizeof(size_t *));
        if (graph->roads == NULL) ec = ERR_MEM;
    }

    // выделяем память под элементы матрицы
    if (ec == SUCCESS_CODE)
    {
        roads_data = (size_t *)calloc(cities_quantity * cities_quantity, sizeof(size_t)); 
        if (roads_data == NULL) ec = ERR_MEM; 
    }

    // привязываем rows матрицы к соответствующим указателям
    for (size_t i = 0; ec == SUCCESS_CODE && i < cities_quantity; i++)
        graph->roads[i] = roads_data + i * cities_quantity;

    // если что-то пошло не так, освобождаем все
    if (ec == ERR_MEM) free_graph(graph);

    return ec;
}

status_t free_graph(graph_t *graph)
{
    size_t *matrix_elems_block_start = NULL;

    if (!graph) return ERR_ARGS;

    if (graph->cities_names)
    {
        for (size_t i = 0; i < graph->cities_quantity; i++)
            if (graph->cities_names[i]) free(graph->cities_names[i]);
        free(graph->cities_names);
    }

    find_start_matrix_data_ptr((const size_t **)graph->roads, graph->cities_quantity, (const size_t **)&matrix_elems_block_start);
    if (matrix_elems_block_start) free(matrix_elems_block_start);
    if (graph->roads) free(graph->roads);

    return SUCCESS_CODE;
}

status_t find_start_matrix_data_ptr(const size_t **matrix, size_t matrix_rows_quantity, const size_t **matrix_data_start_ptr)
{
    status_t ec = SUCCESS_CODE;
    
    if (!matrix || !matrix_data_start_ptr || matrix_rows_quantity == 0) 
        return ERR_ARGS;

    *matrix_data_start_ptr = matrix[0];
    for (size_t i = 1; ec == SUCCESS_CODE && i < matrix_rows_quantity; i++)
        if (matrix[i] < *matrix_data_start_ptr)
            *matrix_data_start_ptr = matrix[i];
    
    return ec;
}
