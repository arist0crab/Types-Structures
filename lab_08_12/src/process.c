#include "../inc/process.h"  // TODO

status_t process_manual_input(graph_t *graph);

status_t procces_menu_choice(menu_option_t menu_option, graph_t *graph)
{
    status_t ec = SUCCESS_CODE;
    // TODO упорядочить
    size_t graph_cities = 0;
    char *city1 = NULL, *city2 = NULL; 
    size_t indx1 = 0, indx2 = 0;
    char *word = NULL, *filename = NULL;
    char cmd[MAX_STRING_LENGTH];
    FILE *filestream = NULL;

    size_t *distances = NULL;

    if (!graph) ec = ERR_ARGS;

    if (ec == SUCCESS_CODE)
    {
        switch (menu_option)
        {
            case EXIT_PROGRAM:
                break;

            case CREATE_GRAPH:
                ec = input_size(&graph_cities, "Введите кол-во городов нового графа: ");
                if (ec == SUCCESS_CODE)
                    ec = init_graph(graph, graph_cities);
                break;

            case DESTROY_GRAPH:
                ec = free_graph(graph);
                break;

            case MANUALLY_GRAPH_SETTINGS:
                ec = process_manual_input(graph);
                break;

            case FILE_DATA_INPUT:
                if (graph && (graph->cities_names || graph->roads)) free_graph(graph);  // освобождаем старый граф

                if (ec == SUCCESS_CODE)
                    ec = input_string(&filename, "Введите имя файла с графом: ");
                if (ec == SUCCESS_CODE)
                {
                    filestream = fopen(filename, "r");
                    if (!filestream) ec = ERR_FILE;
                }

                if (ec == SUCCESS_CODE)
                    ec = input_graph_from_file(graph, filestream);

                if (filestream) fclose(filestream);

                break;

            case SET_CAPITAL:
                if (!graph || !graph->cities_names  || !graph->roads)
                    ec = ERR_GRAPH_DOESNT_EXIST;
                if (ec == SUCCESS_CODE)
                    ec = input_string(&word, "Введите новую столицу: ");
                if (ec == SUCCESS_CODE)
                    ec = set_graph_capital(graph, (const char *)word);
                break;

            case FIND_SHORTEST_ROUTE_BETWEEN_TWO_CITIES:
                if (!graph || !graph->cities_names  || !graph->roads)
                    ec = ERR_GRAPH_DOESNT_EXIST;
                if (ec == SUCCESS_CODE)
                    ec = input_string(&city1, "Введите город отправления: ");
                if (ec == SUCCESS_CODE)
                    ec = input_string(&city2, "Введите город назначения: ");
                if (ec == SUCCESS_CODE)
                    ec = get_cities_indexes(graph, (const char *)city1, (const char *)city2, &indx1, &indx2);
                if (ec == SUCCESS_CODE)
                    // т.к. граф направленный, берем обязательно город №1
                    ec = dijkstra_graph(graph, indx1, &distances);
                if (ec == SUCCESS_CODE)
                    print_dijkstra_result(graph, distances, indx1, indx2);
                if (distances) free(distances);
                break;

            case FIND_FURTHER_THAN_T_CITIES:
                // TODO
                break;

            case CONNECTIVITY_CHECK:
                // TODO
                break;

            case BUILD_MINIMAL_SKELETON:
                // TODO
                break;

            case PRINT_GRAPH:
                if (!graph || !graph->cities_names  || !graph->roads)
                    ec = ERR_GRAPH_DOESNT_EXIST;
                if (ec == SUCCESS_CODE)
                    ec = input_string(&filename, "Введите имя файла (картинка графа): ");
                if (ec == SUCCESS_CODE)
                    ec = export_graph_to_dot_file(graph, "file.dot");
                if (ec == SUCCESS_CODE)
                {
                    snprintf(cmd, sizeof(cmd), "dot -Tpng \"%s\" -o \"%s\"", "file.dot", filename);
                    ec = system(cmd);
                }
                break;
            
            default:
                ec = ERR_RANGE;
                break;
        }
    }

    return ec;
}

status_t process_manual_input(graph_t *graph)
{
    status_t ec = SUCCESS_CODE;
    manual_menu_option_t manual_menu_option = 0;
    char *city_name_1 = NULL, *city_name_2 = NULL;
    size_t distance_1_to_2 = 0, distance_2_to_1 = 0;
    size_t city1_index = 0, city2_index = 0;
    char *word = NULL;

    if (!graph) ec = ERR_ARGS;

    if (ec == SUCCESS_CODE)
    {
        print_manual_input_menu();
        ec = input_cur_menu_opt((int *)&manual_menu_option, 5);
    }

    if (ec == SUCCESS_CODE)
    {
        switch (manual_menu_option)
        {
            case BACK_TO_MENU:
                break;

            case MANUAL_ADD_CITY:
                if (!graph || !graph->cities_names  || !graph->roads)
                    ec = ERR_GRAPH_DOESNT_EXIST;
                if (ec == SUCCESS_CODE)
                    ec = input_string(&word, "Введите название нового города: ");
                if (ec == SUCCESS_CODE)
                    ec = add_city_to_graph(graph, (const char *)word);
                break;
            
            case MANUAL_ADD_ROAD:
                if (!graph || !graph->cities_names  || !graph->roads)
                    ec = ERR_GRAPH_DOESNT_EXIST;

                // вводим города и дистанции
                if (ec == SUCCESS_CODE)
                    ec = get_two_cities_and_distances(&city_name_1, &city_name_2, &distance_1_to_2, &distance_2_to_1);

                // проверяем корректность данных и получаем индексы городов в матрице
                if (ec == SUCCESS_CODE)
                    ec = get_cities_indexes(graph, (const char *)city_name_1, (const char *)city_name_2, &city1_index, &city2_index);
                    
                // добавляем дорогу
                if (ec == SUCCESS_CODE)
                    ec = add_road_to_graph(graph, city1_index, city2_index, distance_1_to_2, distance_2_to_1);
                break;

            case MANUAL_REMOVE_CITY:
                if (!graph || !graph->cities_names  || !graph->roads)
                    ec = ERR_GRAPH_DOESNT_EXIST;
                if (ec == SUCCESS_CODE)
                    ec = input_string(&word, "Введите название удаляемого города: ");
                if (ec == SUCCESS_CODE)
                    ec = remove_city_from_graph(graph, (const char *)word);
                break;

            case MANUAL_REMOVE_ROAD:
                if (!graph || !graph->cities_names  || !graph->roads)
                    ec = ERR_GRAPH_DOESNT_EXIST;

                // вводим города и дистанции
                if (ec == SUCCESS_CODE)
                    ec = get_two_cities_and_distances(&city_name_1, &city_name_2, &distance_1_to_2, &distance_2_to_1);

                // проверяем корректность данных и получаем индексы городов в матрице
                if (ec == SUCCESS_CODE)
                    ec = get_cities_indexes(graph, (const char *)city_name_1, (const char *)city_name_2, &city1_index, &city2_index);
                    
                // добавляем дорогу
                if (ec == SUCCESS_CODE)
                    ec = remove_road_from_graph(graph, city1_index, city2_index);
                break;
            
            default:
                ec = ERR_RANGE;
                break;
        }
    }

    return ec;
}