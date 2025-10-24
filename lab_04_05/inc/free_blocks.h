#ifndef __FREE_BLOCKS_H__
#define __FREE_BLOCKS_H__

#include "data.h"
#include "color.h"
#include <stdio.h>

#define MAX_FREE_BLOCKS_ARRAY_SIZE 1000


typedef struct
{
    void *free_blocks[MAX_FREE_BLOCKS_ARRAY_SIZE];
    size_t size;
} free_blocks_array_t;

status_t print_free_blocks(free_blocks_array_t *free_blocks);
status_t add_to_free_blocks_array(free_blocks_array_t *free_blocks, void *address);

#endif