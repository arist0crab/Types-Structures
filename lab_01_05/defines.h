#ifndef DEFINES_H
#define DEFINES_H

#define FIRST_MANTISS_LENGTH 35
#define SECOND_MANTISS_LENGTH 40
#define MAX_MANTISS_LENGTH 40

typedef struct 
{
    int mantiss[MAX_MANTISS_LENGTH];
    int mant_sign;
    int order;
    int sign;
} lfloat_t;


typedef enum 
{
    SUCCESS_CODE,
    ERROR
} exit_status;

#endif