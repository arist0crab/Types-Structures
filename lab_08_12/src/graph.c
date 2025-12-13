#include "graph.h"

status_t find_city_in_graph(graph_t *graph, const char *city, ssize_t *index_city);
status_t find_start_matrix_data_ptr(size_t **matrix, size_t matrix_rows_quantity, size_t **matrix_data_start_ptr);

status_t export_graph_to_dot_file(graph_t *graph, const char *filename)
{
    status_t ec = SUCCESS_CODE;
    FILE *filestream = NULL;

    if (!graph || !graph->cities_names || !graph->roads || !filename || filename[0] == '\0')
        return ERR_ARGS;

    filestream = fopen(filename, "w");
    if (!filestream) ec = ERR_FILE;

    if (ec == SUCCESS_CODE)
    {
        fprintf(filestream, "digraph G {\n");
        fprintf(filestream, "   node [shape=\"circle\", style=\"filled\", fillcolor=\"blue\", fontcolor=\"#FFFFFF\"];\n");
        fprintf(filestream, "   rankdir=\"LR\";\n");

        for (size_t i = 0; i < graph->cities_quantity; i++)
            for (size_t j = 0; j < graph->cities_quantity; j++)
                if (graph->roads[i][j] > 0)
                    fprintf(filestream, "   \"%s\"->\"%s\" [label=\"%zu\", fontsize=12];\n", graph->cities_names[i], graph->cities_names[j], graph->roads[i][j]);
        fprintf(filestream, "}\n");
    }

    if (filestream) fclose(filestream);

    return ec;
}

status_t remove_road_from_graph(graph_t *graph, size_t index_city_1, size_t index_city_2)
{
    if (!graph || graph->roads)
        return ERR_ARGS;
    if (index_city_1 >= graph->cities_quantity || index_city_2 >= graph->cities_quantity || index_city_1 == index_city_2)
        return ERR_RANGE;

    graph->roads[index_city_1][index_city_2] = 0;
    graph->roads[index_city_2][index_city_1] = 0;

    return SUCCESS_CODE;
}

status_t add_road_to_graph(graph_t *graph, size_t index_city_1, size_t index_city_2, size_t distance_1_to_2, size_t distance_2_to_1)
{
    if (!graph || graph->roads || distance_1_to_2 == 0 || distance_2_to_1 == 0)
        return ERR_ARGS;
    if (index_city_1 >= graph->cities_quantity || index_city_2 >= graph->cities_quantity || index_city_1 == index_city_2)
        return ERR_RANGE;

    graph->roads[index_city_1][index_city_2] = distance_1_to_2;
    graph->roads[index_city_2][index_city_1] = distance_2_to_1;

    return SUCCESS_CODE;
}

status_t get_cities_indexes(graph_t *graph, const char *city_1, const char *city_2, size_t  *indx1, size_t *indx2)
{
    status_t ec = SUCCESS_CODE;
    bool found1 = false, found2 = false;

    if (!graph || !graph->cities_names || !graph->cities_quantity || !indx1 || !indx2)
        return ERR_ARGS;

    *indx1 = 0;
    *indx2 = 0;

    for (size_t i = 0; i < graph->cities_quantity; i++)
    {
        if (str_iequal(graph->cities_names[i], city_1) == 0)
        {
            found1 = true;
            *indx1 = i;
        }
        if (strcmp(graph->cities_names[i], city_2) == 0)
        {
            found2 = true;
            *indx2 = i;
        }
    }

    // имена городов одинаковые
    if (found1 && found2 && *indx1 == *indx2) ec = ERR_ARGS;
    // если какой-то город не был найден
    if (!found1 || !found2) ec = ERR_NOT_FOUND;

    return ec;
}

status_t remove_city_from_graph(graph_t *graph, const char *city)
{
    status_t ec = SUCCESS_CODE;
    ssize_t city_index = -1;
    ssize_t last_city = 0;

    if (!graph || !graph->cities_names || !graph->roads)
        return ERR_ARGS;

    ec = find_city_in_graph(graph, city, &city_index);
    if (ec == SUCCESS_CODE && city_index < 0)
        ec = ERR_NOT_FOUND;
    else if (ec == SUCCESS_CODE && city_index >= 0)
    {
        if (graph->cities_names[city_index])
            free(graph->cities_names[city_index]);
        graph->cities_names[city_index] = NULL;
        
        last_city = graph->cities_quantity - 1;
        if (city_index != last_city)
        {
            // копируем название последнего города
            graph->cities_names[city_index] = graph->cities_names[last_city];
            graph->cities_names[last_city] = NULL;
            
            // копируем всю строку матрицы дорог последнего города
            memcpy(graph->roads[city_index], graph->roads[last_city], graph->max_vertices_quantity * sizeof(size_t));
            
            // обновляем столбец последнего города (все строки)
            for (size_t i = 0; i < graph->cities_quantity; i++)
                graph->roads[i][city_index] = graph->roads[i][last_city];
        }
        graph->roads[last_city][last_city] = 0;  // диагональ = 0
        graph->cities_quantity--;
    }

    return ec;
}

status_t add_city_to_graph(graph_t *graph, const char *city)
{
    status_t ec = SUCCESS_CODE;
    ssize_t city_index = -1;

    if (!graph || !city || graph->cities_quantity >= graph->max_vertices_quantity) 
        return ERR_ARGS;

    // освобождаем стару строку, если она была
    if (graph->cities_names[graph->cities_quantity])
        free(graph->cities_names[graph->cities_quantity]);

    ec = find_city_in_graph(graph, city, &city_index);
    if (city_index >= 0) ec = ERR_ALREADY_EXISTS;

    if (ec == SUCCESS_CODE)
    {
        graph->cities_names[graph->cities_quantity] = str_dynamic_copy(city);
        if (graph->cities_names[graph->cities_quantity] == NULL) 
            ec = ERR_MEM;
    }

    graph->cities_quantity += (ec == SUCCESS_CODE);

    return ec;
}

status_t set_graph_t_distance(graph_t *graph, size_t t_distance)
{
    if (!graph) return ERR_ARGS;

    graph->t_distance = t_distance;

    return SUCCESS_CODE;
}

status_t set_graph_capital(graph_t *graph, const char *capital)
{
    status_t ec = SUCCESS_CODE;
    ssize_t city_index = -1;
    
    if (!graph || !capital || capital[0] == '\0') 
        ec = ERR_ARGS;

    // если такого города нет добавляем
    if (ec == SUCCESS_CODE)
    {
        ec = find_city_in_graph(graph, capital, &city_index);
        if (ec == SUCCESS_CODE && city_index < 0)
            ec = add_city_to_graph(graph, capital);
    }

    if (ec == SUCCESS_CODE)
    {
        if (graph->capital) free(graph->capital);
        graph->capital = str_dynamic_copy(capital);
        if (graph->capital == NULL) 
            ec = ERR_MEM;
    }

    return ec;
}

status_t find_city_in_graph(graph_t *graph, const char *city, ssize_t *index_city)
{
    if (!graph || !graph->cities_names || !city || city[0] == '\0' || !index_city)
        return ERR_ARGS;

    *index_city = -1;
    for (size_t i = 0; *index_city < 0 && i < graph->cities_quantity; i++)
        if (str_iequal(graph->cities_names[i], city) == 0)
            *index_city = i;

    return SUCCESS_CODE;
}

status_t clear_graph(graph_t *graph)
{
    size_t *matrix_elems_block_start = NULL;

    if (!graph) return ERR_ARGS;

    if (graph->cities_names)
        for (size_t i = 0; i < graph->max_vertices_quantity; i++)
            if (graph->cities_names[i]) free(graph->cities_names[i]);

    find_start_matrix_data_ptr(graph->roads, graph->cities_quantity, &matrix_elems_block_start);
    if (graph->roads) memset(matrix_elems_block_start, 0, graph->max_vertices_quantity * graph->max_vertices_quantity);

    if (graph->capital) free(graph->capital);
    graph->cities_quantity = 0;
    graph->t_distance = 0;

    return SUCCESS_CODE;
}

status_t init_graph(graph_t *graph, size_t cities_quantity)
{
    status_t ec = SUCCESS_CODE;
    size_t *roads_data = NULL;

    if (!graph || cities_quantity == 0) return ERR_ARGS;

    graph->max_vertices_quantity = cities_quantity * 2;
    graph->cities_quantity = cities_quantity;
    graph->t_distance = 0;
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
