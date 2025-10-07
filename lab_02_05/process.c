#include "process.h"

status_t clean_arr(theater_play_t *theater_plays_arr, size_t *theater_plays_q);
status_t add_play(theater_play_t *theater_plays_arr, int *theater_plays_keys, size_t *theater_plays_q);

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

status_t process_choice(choice_t choice, bool *program_running, theater_play_t *theater_plays_arr, int *theater_plays_keys, size_t *theater_plays_q)
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
            rc = read_file(theater_plays_arr, theater_plays_keys, theater_plays_q);
        if (rc == SUCCCESS_CODE)
            rc = print_theater_plays_table(theater_plays_arr, *theater_plays_q);
        break;

    case PRINT_DATA:
        rc = print_theater_plays_table(theater_plays_arr, *theater_plays_q);
        break;
    
    case PRINT_KEY_TABLE:
        rc = print_keys_table(theater_plays_arr, theater_plays_keys, *theater_plays_q);
        break;

    case ADD_THEATER_PLAY:
        rc = add_play(theater_plays_arr, theater_plays_keys, theater_plays_q);
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

status_t add_play(theater_play_t *theater_plays_arr, int *theater_plays_keys, size_t *theater_plays_q)
{
    status_t rc = SUCCCESS_CODE;
    theater_play_t new_play;
    int temp_int;

    if (*theater_plays_q >= MAX_PLAYS_QUANTITY)
        return RECORDS_ARR_OVERFLOWED;

    printf("%sВведите название театра: %s", BLUE, RESET);  // TODO добавить в инструкции что это будет обрезаться как мне будет угодно
    scanf("%s", new_play.theater_name);
    printf("%sВведите название спектакля: %s", BLUE, RESET);
    scanf("%s", new_play.play_name);
    printf("%sВведите цену билета: %s", BLUE, RESET);

    if (scanf("%lf", &new_play.ticket_price) != 1 || new_play.ticket_price <= 0.0)
        rc = INVALID_INPUT;
    
    if (rc == SUCCCESS_CODE)
    {
        printf("%sВведите максимальную цену билета: %s", BLUE, RESET);
        if (scanf("%lf", &new_play.max_ticket_price) != 1 || new_play.max_ticket_price <= 0.0)
            rc = INVALID_INPUT;
    }

    if (rc == SUCCCESS_CODE)
        if (new_play.ticket_price > new_play.max_ticket_price)
            rc = INVALID_INPUT;

    if (rc == SUCCCESS_CODE)
    {
        printf("%sВведите тип спектакля (целое число):\n0 - пьеса\n1- мюзикл\n%s", BLUE, RESET);
        if (scanf("%d", &temp_int) != 1 || temp_int < 0 || temp_int > 1)
            rc = INVALID_INPUT;
        else 
            new_play.play_type = (play_type_t)temp_int;
    }

    if (rc == SUCCCESS_CODE && new_play.play_type == PIECE)
    {
        printf("%sВведите тип пьесы (целое число):\n0 - драма\n1 - комедия\n2 - сказка\n%s", BLUE, RESET);
        if (scanf("%d", &temp_int) != 1 || temp_int < 0 || temp_int > 2)
            rc = INVALID_INPUT;
        else 
            new_play.play_data.piece_info.piece_genre = (piece_genre_t)temp_int;

        if (rc == SUCCCESS_CODE && new_play.play_data.piece_info.piece_genre == FAIRY_TALE)
        {
            printf("%sВведите возраст (целое число):\n0 - 3+\n1 - 10+\n2 - 16+\n%s", BLUE, RESET);
            if (scanf("%d", &temp_int) != 1 || temp_int < 0 || temp_int > 2)
                rc  = INVALID_INPUT;
            else 
                new_play.age_rating = (age_rating_t)temp_int;
        }

        if (rc == SUCCCESS_CODE && new_play.play_data.piece_info.piece_genre != FAIRY_TALE)
            new_play.age_rating = AGE_RATING_NONE;
    }

    if (rc == SUCCCESS_CODE && new_play.play_type == MUSICAL)
    {
        printf("%sВведите имя композитоора: %s", BLUE, RESET);
        scanf("%s", new_play.play_data.musical_info.composer);
        printf("%sВведите страну происхождения: %s", BLUE, RESET);
        scanf("%s", new_play.play_data.musical_info.country);
        printf("%sВведите вид мюзикла:\n0 - балет\n1 - опера\n2 - мюзикл\n%s", BLUE, RESET);
        if (scanf("%d", &temp_int) != 1 || temp_int < 0 || temp_int > 2)
            rc = INVALID_INPUT;
        else 
            new_play.play_data.musical_info.musical_genre = (musical_genre_t)temp_int;
        if (rc == SUCCCESS_CODE)
        {
            printf("%sВведите возраст (целое число):\n0 - 3+\n1 - 10+\n2 - 16+\n%s", BLUE, RESET);
            if (scanf("%d", &temp_int) != 1 || temp_int < 0 || temp_int > 2)
                rc  = INVALID_INPUT;
            else 
                new_play.age_rating = (age_rating_t)temp_int;
        }
        if (rc == SUCCCESS_CODE)
        {
            printf("%sВведите продолжительность в минутах (целое число): %s", BLUE, RESET);
            if (scanf("%d", &new_play.play_data.musical_info.duration) != 1 || temp_int < 0)
                rc = INVALID_INPUT;
        }
    }

    if (rc != SUCCCESS_CODE)
        while (getchar() != '\n');

    theater_plays_arr[*theater_plays_q] = new_play;
    theater_plays_keys[*theater_plays_q] = *theater_plays_q;
    (*theater_plays_q)++; 

    

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