#ifndef __DATA_H__
#define __DATA_H__

#define MAX_STRING_LENGTH 256
#define MENU_OPTIONS_QUANTITY 12

typedef enum 
{
    SUCCESS_CODE,
    ERR_IO,
    ERR_RANGE,
    ERR_MEM,
    ERR_ARGS,
    ERR_FILE,
    ERR_NOT_FOUND,
    ERR_ALREADY_EXISTS,
    ERR_INVALID_POINTER
} status_t;

typedef enum
{
    // выход из программы
    EXIT_PROGRAM,
    
    // работа с графом
    CREATE_GRAPH,   
    DESTROY_GRAPH,
    MANUALLY_GRAPH_SETTINGS,
    FILE_DATA_INPUT,

    // ввод параметров
    SET_CAPITAL,
    SET_T_DISTANCE,

    // алгоритмы обработки графовых структур
    FIND_SHORTEST_ROUTE_BETWEEN_TWO_CITIES,
    FIND_FURTHER_THAN_T_CITIES,
    CONNECTIVITY_CHECK,
    BUILD_MINIMAL_SKELETON,

    // функции вывода
    PRINT_GRAPH
} menu_option_t;

typedef enum
{
    BACK_TO_MENU,
    MANUAL_ADD_CITY,
    MANUAL_ADD_ROAD,
    MANUAL_REMOVE_CITY,
    MANUAL_REMOVE_ROAD
} manual_menu_option_t;

#endif