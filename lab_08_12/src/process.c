#include "../inc/process.h"  // TODO

status_t process_manual_input(graph_t *graph);

status_t procces_menu_choice(menu_option_t menu_option, graph_t *graph)
{
    status_t ec = SUCCESS_CODE;
    size_t t_distance = 0;
    char *word = NULL;

    if (!graph) ec = ERR_ARGS;

    if (ec == SUCCESS_CODE)
    {
        switch (menu_option)
        {
            case EXIT_PROGRAM:
                break;

            case CLEAR_GRAPH:
                ec = clear_graph(graph);
                break;

            case MANUALLY_GRAPH_SETTINGS:
                ec = process_manual_input(graph);
                break;

            case FILE_DATA_INPUT:
                // TODO
                break;

            case SET_CAPITAL:
                ec = input_string(&word, "Введите новую столицу: ");
                if (ec == SUCCESS_CODE)
                    ec = set_graph_capital(graph, (const char *)word);
                break;

            case SET_T_DISTANCE:
                ec = input_size(&t_distance, "Введите расстояние Т: ");
                if (ec == SUCCESS_CODE)
                    ec = set_graph_t_distance(graph, t_distance);
                break;

            case FIND_SHORTEST_ROUTE_BETWEEN_TWO_CITIES:
                // TODO
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
                // TODO
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
    char message[MAX_STRING_LENGTH];
    char *word = NULL;

    if (!graph) ec = ERR_ARGS;

    if (ec == SUCCESS_CODE)
    {
        print_manual_input_menu();
        ec = input_cur_menu_opt((int *)&manual_menu_option, 6);
    }

    if (ec == SUCCESS_CODE)
    {
        switch (manual_menu_option)
        {
            case BACK_TO_MENU:
                break;

            // TODO кажется надо добаить функию удаления городов и дорог)))))))))

            case MANUAL_ADD_CITY:
                ec = input_string(&word, "Введите название нового города: ");
                if (ec == SUCCESS_CODE)
                    ec = add_city_to_graph(graph, (const char *)word);
                break;
            
            case MANUAL_ADD_ROAD:
                // получаем названия городов
                ec = input_string(&city_name_1, "Введите город №1: ");
                if (ec == SUCCESS_CODE)
                    ec = input_string(&city_name_2, "Введите город №2: ");

                // проверяем их корректность и получаем индексы в матрице
                if (ec == SUCCESS_CODE)
                    ec = get_cities_indexes(graph, (const char *)city_name_1, (const char *)city_name_2, &city1_index, &city2_index);
                
                // получаем дистанции
                if (ec == SUCCESS_CODE)
                {
                    snprintf(message, MAX_STRING_LENGTH, "Введите длину дороги из %s в %s: ", city_name_1, city_name_2);
                    ec = input_size(&distance_1_to_2, message);
                }
                if (ec == SUCCESS_CODE)
                {
                    snprintf(message, MAX_STRING_LENGTH, "Введите длину дороги из %s в %s: ", city_name_2, city_name_1);
                    ec = input_size(&distance_2_to_1, message);
                }

                // добавляем дорогу
                if (ec == SUCCESS_CODE)
                    ec = add_road_to_graph(graph, city1_index, city2_index, distance_1_to_2, distance_2_to_1);
                break;
            
            case MANUAL_COMPLEX_INPUT:
                // TODO
                break;

            case MANUAL_REMOVE_CITY:
                // TODO
                break;

            case MANUAL_REMOVE_ROAD:
                // TODO
                break;
            
            default:
                ec = ERR_RANGE;
                break;
        }
    }

    return ec;
}