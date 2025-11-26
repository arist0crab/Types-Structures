#ifndef __DATA_H__
#define __DATA_H__

#define MAX_QUEUE_SIZE 1000
#define MAX_STRING_LENGTH 256
#define MENU_OPTIONS_QUANTITY 8
#define CONFIG_MENU_OPTIONS_QUANTITY 5
#define EPS 1e-9

typedef enum
{
    SUCCESS_CODE,
    ERR_IO,
    ERR_INVALID_POINTER,
    ERR_RANGE,
    ERR_OVERFLOW,
    ERR_POP_EMPTY,
    ERR_MEM,
    ERR_CRITICAL_RECORDS_ATTITUDE
} status_t;

typedef enum 
{
    EXIT_PROGRAM,
    RUN_ARRAY_SIMULATION,
    RUN_LIST_SIMULATION,
    EVALUATE_EFFECTIVENESS,
    VIEW_MEMORY_INFORMATION_ARR,
    VIEW_MEMORY_INFORMATION_LIST,
    COMPARE_PUSH_POP,
    CONFIGURE_SIMULATION_PARAMETERS
} menu_option_t;

typedef enum 
{
    GO_BACK,
    CHANGE_ENTARANCE_T1,
    CHANGE_ENTARANCE_T2,
    CHANGE_PROCESSING_T1,
    CHANGE_PROCESSING_T2
} config_menu_option_t;

typedef enum
{
    TYPE_1,
    TYPE_2
} request_class_t;

typedef struct
{
    request_class_t request_class;
    double arrival_time;
} request_t;

typedef struct
{
    double min_time;
    double max_time;
} time_range_t;

#endif