#include "proccess.h"

status_t procces_menu_choice(menu_option_t menu_option)
{
    status_t ec = SUCCESS_CODE;

    switch (menu_option)
    {
        case EXIT_PROGRAM:
            break;

        case RUN_ARRAY_SIMULATION:
            ec = simulate_service_unit_by_arr();
            break;

        case RUN_LIST_SIMULATION:
            break;

        case EVALUATE_EFFECTIVENESS:
            break;

        case VIEW_MEMORY_INFORMATION:
            break;

        case CONFIGURE_SIMULATION_PARAMETERS:
            break;
        
        default:
            ec = ERR_RANGE;
            break;
    }

    return ec;
}