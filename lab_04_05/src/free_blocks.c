#include "free_blocks.h"


status_t add_to_free_blocks_array(free_blocks_array_t *free_blocks, void *address)
{
    status_t ec = (free_blocks && address) ? SUCCESS_CODE : ERR_INVALID_POINTER;

    if (free_blocks->size >= MAX_FREE_BLOCKS_ARRAY_SIZE)
        ec = ERR_RANGE;

    if (ec == SUCCESS_CODE)
    {
        free_blocks->free_blocks[free_blocks->size] = address;
        free_blocks->size++;
    }

    return ec;
}

status_t print_free_blocks(free_blocks_array_t *free_blocks)
{
    status_t ec = (free_blocks) ? SUCCESS_CODE : ERR_INVALID_POINTER;

    if (ec == SUCCESS_CODE && free_blocks->size == 0)
        printf("%s============ ТЕКУЩИЙ МАССИВ ПУСТ ============\n%s", BLUE_BOLD, RESET);
    else if (ec == SUCCESS_CODE)
    {
        printf("%sВсего освобождено блоков: %lu\n%s", BLUE, free_blocks->size, RESET);
        for (size_t i = 0; i < free_blocks->size; i++)
            printf("%s[%lu] Адрес: %p%s\n", BLUE, i + 1, free_blocks->free_blocks[i], RESET);
    }

    return ec;
}