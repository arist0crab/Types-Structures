#ifndef DEFINES_H
#define DEFINES_H

#define FIRST_MANTISS_LENGTH 35
#define SECOND_MANTISS_LENGTH 40
#define MAX_MANTISS_LENGTH 40
#define MAX_BUFFER_LENGTH 50
#define MAX_ORDER_LENGTH 5

#include <stdbool.h>

typedef struct 
{
    int mantiss[MAX_MANTISS_LENGTH];
    int mant_size;
    int order;
    bool mant_sign;
} lfloat_t;


typedef enum 
{
    SUCCESS_CODE,
    INVALID_CHARACTER,
    ERR_MANTISS_SIZE,
    ERR_ORDER_SIZE,
    NO_NUM_AFTER_E,
    INCORRECT_NUM_LENGTH
} exit_status;

#endif