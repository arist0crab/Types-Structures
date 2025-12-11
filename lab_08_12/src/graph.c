#include "graph.h"

status_t find_start_matrix_data_ptr(size_t **matrix, size_t matrix_rows_quantity, size_t **matrix_data_start_ptr);

status_t init_graph(graph_t *graph, size_t cities_quantity)
{
    status_t ec = SUCCESS_CODE;
    size_t *roads_data = NULL;

    if (!graph || cities_quantity == 0) return ERR_ARGS;

    graph->max_vertices_quantity = cities_quantity * 2;
    graph->cities_quantity = cities_quantity;
    graph->target_distance = 0;
    graph->cities_names = NULL;
    graph->capital = NULL;
    graph->roads = NULL;
    
    // выделяем массив указателей на строки (массив слов)
    graph->cities_names = (char **)malloc(graph->max_vertices_quantity * sizeof(char *));
    if (graph->cities_names == NULL) ec = ERR_MEM;

    // инициализируем массив указателей
    for (size_t i = 0; ec == SUCCESS_CODE && i < graph->max_vertices_quantity; i++)
        graph->cities_names[i] = NULL;

    // выделяем массив указателей на rows матрицы (дороги)
    if (ec == SUCCESS_CODE)
    {
        graph->roads = (size_t **)calloc(graph->max_vertices_quantity, sizeof(size_t *));
        if (graph->roads == NULL) ec = ERR_MEM;
    }

    // выделяем память под элементы матрицы
    if (ec == SUCCESS_CODE)
    {
        roads_data = (size_t *)calloc(graph->max_vertices_quantity * graph->max_vertices_quantity, sizeof(size_t)); 
        if (roads_data == NULL) ec = ERR_MEM; 
    }

    // привязываем rows матрицы к соответствующим указателям
    for (size_t i = 0; ec == SUCCESS_CODE && i < graph->max_vertices_quantity; i++)
        graph->roads[i] = roads_data + i * graph->max_vertices_quantity;

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
        for (size_t i = 0; i < graph->max_vertices_quantity; i++)
            if (graph->cities_names[i]) free(graph->cities_names[i]);
        free(graph->cities_names);
    }

    if (graph->capital) free(graph->capital);

    find_start_matrix_data_ptr(graph->roads, graph->max_vertices_quantity, &matrix_elems_block_start);
    if (matrix_elems_block_start) free(matrix_elems_block_start);
    if (graph->roads) free(graph->roads);

    return SUCCESS_CODE;
}

status_t find_start_matrix_data_ptr(size_t **matrix, size_t matrix_rows_quantity, size_t **matrix_data_start_ptr)
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
