#include "process.h"

// как то сжделать эту функцию общей для выделения ключей и для выделения постановок
status_t allocate_memory_for_plays(theater_play_t *theater_plays_arr, size_t plays_quantity)
{
    status_t rc = SUCCCESS_CODE;

    if ((int)plays_quantity <= 0)
        rc = INVALID_QUANTITY;

    if (rc == SUCCCESS_CODE)
    {
        theater_plays_arr = (theater_play_t *)malloc(plays_quantity * sizeof(theater_play_t));
        if (theater_plays_arr == NULL)
            rc = INVALID_POINTER;
    }

    

    return rc;
}