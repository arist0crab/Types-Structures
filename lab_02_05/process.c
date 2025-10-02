#include "process.h"

status_t clean_arr(theater_play_t *theater_plays_arr, size_t *theater_plays_q);


status_t allocate_memory(void **elems_arr, size_t elems_quantity, size_t elem_size)
{
    status_t rc = SUCCCESS_CODE;
    *elems_arr = NULL;

    if ((int)elems_quantity <= 0 || (int)elem_size <= 0)
        rc = INVALID_QUANTITY;

    if (rc == SUCCCESS_CODE)
    {
        *elems_arr = malloc(elems_quantity * elem_size);
        if (*elems_arr == NULL)
            rc = MEM_ALLOCATE_ERROR;
    }

    return rc;
}

status_t process_choice(choice_t choice, bool *program_running, theater_play_t *theater_plays_arr, size_t *theater_plays_q)
{
    status_t rc = SUCCCESS_CODE;
    
    switch (choice)
    {
    case EXIT_THE_PROGRAM:
        *program_running = false;
        break;

    case READ_FILE:
        print_read_file_comments();
        rc = clean_arr(theater_plays_arr, theater_plays_q);
        if (rc == SUCCCESS_CODE)
            rc = read_file(theater_plays_arr, theater_plays_q);
        if (rc == SUCCCESS_CODE)
            rc = print_theater_plays_table(theater_plays_arr, *theater_plays_q);
        break;

    case PRINT_DATA:
        rc = print_theater_plays_table(theater_plays_arr, *theater_plays_q);
        break;
    
    case PRINT_KEY_TABLE:
        break;

    case ADD_THEATER_PLAY:
        break;

    case DELETE_THEATER_PLAY:
        break;

    case PRICE_QUICK_SORT:
        break;

    case PRICE_SLOW_SORT:
        break;

    case KEY_TABLE_QUICK_SORT:
        break;

    case KEY_TABLE_SLOW_SORT:
        break;

    case PRINT_DATA_KEY_TABLE:
        break;

    case PRINT_EFFICIENCY_TABLE:
        break;

    case PRINT_BALETS:
        break;

    case CHOICES_QUANTITY:
        rc = INVALID_CHOICE;
        break;
    
    default:
        rc = INVALID_CHOICE;
        break;
    }

    if (rc == SUCCCESS_CODE)
        printf("%sSUCCESS%s\n\n", GREEN_BOLD, RESET);

    return rc;
}

status_t clean_arr(theater_play_t *theater_plays_arr, size_t *theater_plays_q)
{
    if (theater_plays_arr == NULL || theater_plays_q == NULL)
        return ARR_PROCESSING_ERROR;

    *theater_plays_q = 0;
    memset(theater_plays_arr, 0, sizeof(theater_play_t) * MAX_PLAYS_QUANTITY);

    return SUCCCESS_CODE;
}