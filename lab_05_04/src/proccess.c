#include "proccess.h"

status_t procces_menu_choice(menu_option_t menu_option)
{
    status_t ec = SUCCESS_CODE;

    switch (menu_option)
    {
        case EXIT_PROGRAM:
            break;

        case RUN_ARRAY_SIMULATION:
            ec = simulate_service_unit_by_arr(true);
            break;

        case RUN_LIST_SIMULATION:
            ec = simulate_service_unit_by_list(true, NULL);
            break;

        case EVALUATE_EFFECTIVENESS:
            ec = measure_efficiency();
            break;

        case VIEW_MEMORY_INFORMATION_ARR:
            ec = fill_and_empty_arr_queue();
            break;

        case VIEW_MEMORY_INFORMATION_LIST:
            ec = fill_and_empty_list_queue();
            break;

        case CONFIGURE_SIMULATION_PARAMETERS:
            ec = change_simulation_configurations();
            break;

        case COMPARE_PUSH_POP:
            ec = compare_push_and_pop();
            break;
        
        default:
            ec = ERR_RANGE;
            break;
    }

    return ec;
}