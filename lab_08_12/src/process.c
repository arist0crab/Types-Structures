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

            case MANUALLY_DATA_INPUT:
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

    if (!graph) ec = ERR_ARGS;

    // TODO напечатать меню для ручного ввода

    if (ec == SUCCESS_CODE)
        ec = input_cur_menu_opt((int *)&manual_menu_option, 4);

    if (ec == SUCCESS_CODE)
    {
        switch (manual_menu_option)
        {
            case BACK_TO_MENU:
                break;

            case MANUAL_ADD_CITY:
                // TODO
                break;
            
            case MANUAL_ADD_ROAD:
                // TODO
                break;
            
            case MANUAL_COMPLEX_INPUT:
                // TODO
                break;
            
            default:
                ec = ERR_RANGE;
                break;
        }
    }

    return ec;
}