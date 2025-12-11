#ifndef __DATA_H__
#define __DATA_H__

// BST - Binary Search Tree
// HST - Hash Search table
// AVG - Adelson-Velsky and Landis (фамилии создателей)

#define MENU_OPTIONS_QUANTITY 16  // TODO
#define MAX_PREFIX_SIZE 256
#define MAX_STRING_LENGTH 256

typedef enum 
{
    SUCCESS_CODE,
    ERR_IO,
    ERR_MEM,
    ERR_ARGS,
    ERR_FILE,
    ERR_RANGE,
    ERR_NOT_FOUND,
    ERR_ALREADY_EXISTS,
    ERR_INVALID_POINTER,
    ERR_TABLE_DOESNT_EXIST
} status_t;

typedef enum 
{
    EXIT_PROGRAM,

    // работа с BST
    CREATE_BST_FROM_FILE,
    ADD_BST_ELEM,
    DELETE_BST_ELEM,
    CLEAR_BST,
    FIND_WORD_IN_BST,
    BALANCE_BST,

    // работа с HST
    CREATE_HST_FROM_FILE,
    ADD_HST_ELEM,
    DELETE_HST_ELEM,
    CLEAR_HST,
    RESIZE_HST,
    
    // печать
    PRINT_BST,
    PRINT_HST,

    // эффективность
    MEASURE_SEARCH_TIME
} menu_option_t;

#endif