#ifndef __DATA_H__
#define __DATA_H__

#define MENU_OPTIONS_QUANTITY 8
#define MAX_STRING_LENGTH 256

#include <stdio.h>

typedef enum 
{
    SUCCESS_CODE,
    ERR_IO,
    ERR_RANGE,
    ERR_MEM,
    ERR_ARGS,
    ERR_NOT_FOUND,
    ERR_FILE,
    ERR_EMPTY_TREE
} status_t;

typedef enum
{
    EXIT_PROGRAM,
    READ_TREE_FROM_FILE,
    PRINT_TREE,
    ADD_TREE_NODE,
    DELETE_TREE_NODE,
    FIND_WORD_IN_FILE,
    FIND_WORD_IN_TREE,
    COMPARE_FIND_TIME
} menu_option_t;

typedef struct tree_node
{
    char *word;
    size_t counted;
    struct tree_node *left;
    struct tree_node *right;
} tree_node_t;

#endif