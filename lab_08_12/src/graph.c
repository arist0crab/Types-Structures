#include "graph.h"

status_t find_city_in_graph(graph_t *graph, const char *city, ssize_t *index_city);
status_t dfs(size_t current, bool *visited, const graph_t *graph);

status_t is_graph_connected(const graph_t *graph, bool *is_connected) 
{
    status_t ec = SUCCESS_CODE;
    *is_connected = false;
    bool *visited = NULL;

    if (!graph || !graph->cities_names || !graph->roads || !graph->capital || !is_connected)
        return ERR_ARGS;

    visited = calloc(graph->cities_quantity, sizeof(bool));
    if (!visited) ec = ERR_MEM;
    
    if (ec == SUCCESS_CODE)
        ec = dfs(0, visited, graph);
    
    if (ec == SUCCESS_CODE)
    {
        *is_connected = true;
        for (size_t i = 0; *is_connected && i < graph->cities_quantity; i++)
            if (!visited[i])
                *is_connected = false;
    }
    
    if (visited) free(visited);

    return ec;
}

/**
 * @brief Рекурсивный обход графа в глубину (DFS) для проверки достижимости вершин.
 * 
 * Помечает текущую вершину как посещенную и рекурсивно обходит всех непосещенных
 * соседей. При обнаружении ошибки немедленно возвращает статус вверх по стеку вызовов.
 * 
 * @param[in] current Индекс текущей вершины (город) для обхода.
 * @param[in,out] visited Массив флагов посещенных вершин.
 * @param[in] graph Указатель на структуру графа.
 * 
 * @return Статус выполнения.
 */
status_t dfs(size_t current, bool *visited, const graph_t *graph) 
{
    status_t ec = SUCCESS_CODE;
    size_t n = graph->cities_quantity;
    visited[current] = true;
    
    for (size_t neighbor = 0; ec == SUCCESS_CODE && neighbor < n; neighbor++) 
        if (neighbor != current && graph->roads[current] && graph->roads[current][neighbor] > 0 && !visited[neighbor])
            ec = dfs(neighbor, visited, graph);
    
    return ec;
}

status_t find_cities_farther_than_t_distance(graph_t *graph, size_t distance_t, size_t **far_cities, size_t *count)
{
    status_t ec = SUCCESS_CODE;
    ssize_t capital_index = -1;
    size_t *distances = NULL;

    if (!graph || !far_cities || !count || !graph->capital)
        return ERR_ARGS;
    
    // ищем индекс столицы
    ec = find_city_in_graph(graph, graph->capital, &capital_index);
    if (ec == SUCCESS_CODE && capital_index == -1) ec = ERR_NO_PATH;
    
    // запускаем Дейкстру от столицы
    if (ec == SUCCESS_CODE)
        ec = dijkstra_graph(graph, capital_index, &distances);
    
    if (ec == SUCCESS_CODE)
    {
        *far_cities = malloc(graph->cities_quantity * sizeof(size_t));
        if (!*far_cities) ec = ERR_MEM;
    }
    
    *count = 0;
    for (size_t i = 0; ec == SUCCESS_CODE && i < graph->cities_quantity; i++)
        if (distances[i] > distance_t && distances[i] != SIZE_MAX)
            (*far_cities)[(*count)++] = i;

    if (distances) free(distances);

    return ec;
}

/** @brief Выбирает следующую вершину для обработки в алгоритме Дейкстры 
 * (жадный алгоритм).
 * Относительно начальной точки (т.е. относительно одного из городов, между
 * которыми мы ищем кратчайшее растояние) ищет ближайший непосещенный город.
 * @param[in] dist Массив расстояний между городами
 * @param[in] included Массив статусов городов: посещен / не посещен.
 * @param[in] cities_quantity Количество городов.
 * @param[out] min_index Указатель на индекс до ближайшего непосещенного города.
 * @return Статус выполнения.
 */
status_t find_min_distance(size_t dist[], bool included[], size_t cities_quantity, size_t *min_index)
{
    bool found = false;
    size_t min = SIZE_MAX;
    *min_index = SIZE_MAX;
    
    for (size_t v = 0; v < cities_quantity; v++) 
    {
        if (!included[v] && dist[v] < min) 
        { 
            min = dist[v];
            *min_index = v;
            found = true;
        }
    }
    
    return found ? SUCCESS_CODE : ERR_NO_PATH;
}

status_t dijkstra_graph(graph_t *graph, size_t src, size_t **dist_out)
{
    status_t ec = SUCCESS_CODE;
    bool *included = NULL;
    size_t u;

    if (!graph || src >= graph->cities_quantity || !dist_out)
        return ERR_ARGS;
        
    *dist_out = calloc(graph->cities_quantity, sizeof(size_t));
    included = calloc(graph->cities_quantity, sizeof(bool));
    
    if (!*dist_out || !included) 
    {
        if (*dist_out) free(*dist_out); 
        if (included) free(included);
        ec = ERR_MEM;
    }
    
    // инициализация массива расстояний
    if (ec == SUCCESS_CODE)
    {
        for (size_t i = 0; i < graph->cities_quantity; i++) 
            (*dist_out)[i] = SIZE_MAX;
        (*dist_out)[src] = 0;
    }
    
    for (size_t count = 0; ec == SUCCESS_CODE && count < graph->cities_quantity - 1; count++) 
    {
        ec = find_min_distance(*dist_out, included, graph->cities_quantity, &u);
        if (ec == SUCCESS_CODE) included[u] = true;
        
        // обновляем соседей
        for (size_t v = 0; ec == SUCCESS_CODE && v < graph->cities_quantity; v++)
            if (!included[v] && graph->roads[u][v] != SIZE_MAX && graph->roads[u][v] > 0 && (*dist_out)[u] != SIZE_MAX && (*dist_out)[u] + graph->roads[u][v] < (*dist_out)[v])
                (*dist_out)[v] = (*dist_out)[u] + graph->roads[u][v];
    }
    
    if (included) free(included);

    return ec;
}

status_t input_graph_from_file(graph_t *graph, FILE *filestream)
{
    status_t ec = SUCCESS_CODE;
    size_t t_distance = 0, cities_quantity = 0;
    char city1[MAX_STRING_LENGTH], city2[MAX_STRING_LENGTH];
    size_t indx1 = -1, indx2 = -1;
    size_t distance_1_to_2 = 0, distance_2_to_1 = 0;
    char buf[MAX_STRING_LENGTH];
    int rc;

    if (graph->cities_names || graph->roads) free_graph(graph);

    rewind(filestream);
    ec = read_size_from_file(filestream, &t_distance);
    if (ec == SUCCESS_CODE)
        ec = read_size_from_file(filestream, &cities_quantity);

    if (ec == SUCCESS_CODE && cities_quantity == 0)
        ec = ERR_RANGE;

    if (ec == SUCCESS_CODE)
        ec = init_graph(graph, cities_quantity);

    for (size_t i = 0; ec == SUCCESS_CODE && i < cities_quantity; i++)
    {
        if (fscanf(filestream, "%s", buf) != 1)
            ec = ERR_FILE;
        if (ec == SUCCESS_CODE)
            ec = add_city_to_graph(graph, (const char *)buf);
    }

    if (ec == SUCCESS_CODE)
    {
        if (fscanf(filestream, "%s", buf) != 1)
            ec = ERR_FILE;
        else
            ec = set_graph_capital(graph, (const char *)buf);
    }
    
    if (ec == SUCCESS_CODE)
        rc = fscanf(filestream, "%s %s %lu %lu", city1, city2, &distance_1_to_2, &distance_2_to_1);
    while (rc == 4 && ec == SUCCESS_CODE)
    {
        ec = get_cities_indexes(graph, city1, city2, &indx1, &indx2);
        if (ec == SUCCESS_CODE)
        {
            ec = add_road_to_graph(graph, indx1, indx2, distance_1_to_2, distance_2_to_1);
            rc = fscanf(filestream, "%s %s %lu %lu", city1, city2, &distance_1_to_2, &distance_2_to_1);
        }
    }

    // если что-то пошло не так, уничтожаем все
    if (ec != SUCCESS_CODE) free_graph(graph);
    
    return ec;
}

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
        fprintf(filestream, "   node [shape=\"circle\", style=\"filled\", fillcolor=\"#8cafdd\", fontcolor=\"#102034\"];\n");
        fprintf(filestream, "   rankdir=\"LR\";\n");

        for (size_t i = 0; i < graph->cities_quantity; i++)
            if (graph->cities_names[i])
                fprintf(filestream, "   \"%s\";\n", graph->cities_names[i]);

        if (graph->capital && graph->capital[0] != '\0')
            fprintf(filestream, "   \"%s\" [fillcolor=\"#a38cde\", fontcolor=\"#1c1034ff\"];\n", graph->capital);

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
    if (!graph || !graph->roads)
        return ERR_ARGS;
    if (index_city_1 >= graph->cities_quantity || index_city_2 >= graph->cities_quantity || index_city_1 == index_city_2)
        return ERR_RANGE;

    graph->roads[index_city_1][index_city_2] = 0;
    graph->roads[index_city_2][index_city_1] = 0;

    return SUCCESS_CODE;
}

status_t add_road_to_graph(graph_t *graph, size_t index_city_1, size_t index_city_2, size_t distance_1_to_2, size_t distance_2_to_1)
{
    if (!graph || !graph->roads || distance_1_to_2 == 0 || distance_2_to_1 == 0)
        return ERR_ARGS;
    if (index_city_1 >= graph->cities_quantity || index_city_2 >= graph->cities_quantity || index_city_1 == index_city_2)
        return ERR_RANGE;

    graph->roads[index_city_1][index_city_2] = distance_1_to_2;
    graph->roads[index_city_2][index_city_1] = distance_2_to_1;

    return SUCCESS_CODE;
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

    if (!graph || !graph->cities_names  || !graph->roads)
        return ERR_GRAPH_DOESNT_EXIST;

    if (!city || graph->cities_quantity >= graph->max_vertices_quantity) 
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
        if (str_iequal(graph->cities_names[i], city_2) == 0)
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

status_t init_graph(graph_t *graph, size_t cities_quantity)
{
    status_t ec = SUCCESS_CODE;
    size_t *roads_data = NULL;

    if (!graph || cities_quantity == 0) return ERR_ARGS;

    graph->max_vertices_quantity = cities_quantity * 2;
    graph->cities_quantity = 0;
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

    if (graph->roads)
        find_start_matrix_data_ptr(graph->roads, graph->max_vertices_quantity, &matrix_elems_block_start);
    if (matrix_elems_block_start) free(matrix_elems_block_start);
    if (graph->roads) free(graph->roads);

    graph->cities_names = NULL;
    graph->capital = NULL;
    graph->roads = NULL;

    return SUCCESS_CODE;
}