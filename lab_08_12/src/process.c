#include "../inc/process.h"  // TODO

status_t procces_menu_choice(menu_option_t menu_option)
{
    status_t ec = SUCCESS_CODE;

    switch (menu_option)
    {
        case EXIT_PROGRAM:
            break;

        case MANUALLY_DATA_INPUT:
            // TODO
            break;

        case FILE_DATA_INPUT:
            // TODO
            break;

        case SET_CAPITAL:
            // TODO
            break;

        case SET_T_DISTANCE:
            // TODO
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

    return ec;
}