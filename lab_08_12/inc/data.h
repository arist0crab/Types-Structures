#ifndef __DATA_H__
#define __DATA_H__

#define MAX_STRING_LENGTH 256
#define MENU_OPTIONS_QUANTITY 11

typedef enum 
{
    SUCCESS_CODE,
    ERR_IO,
    ERR_RANGE,
    ERR_MEM,
    ERR_ARGS,
    ERR_INVALID_POINTER
} status_t;

typedef enum
{
    // выход из программы
    EXIT_PROGRAM,
    
    // работа с графом
    CLEAR_GRAPH,
    MANUALLY_DATA_INPUT,
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
    MANUAL_COMPLEX_INPUT
} manual_menu_option_t;

#endif