#include "process.h"

status_t read_file(theater_play_t *theater_plays_arr, size_t *theater_plays_q);
status_t read_piece_data(FILE *filestream, theater_play_t *current_play);
status_t read_musical_data(FILE *filestream, theater_play_t *current_play);

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
        // TODO: стереть предыдущие записи, вывести предупреждение об этом
        rc = read_file(theater_plays_arr, theater_plays_q);
        if (rc == SUCCCESS_CODE)
            rc = print_theater_plays_table(theater_plays_arr, *theater_plays_q);
        if (rc == SUCCCESS_CODE)
            printf("%sSUCCESS%s\n", GREEN_BOLD, RESET);
        break;

    case PRINT_DATA:
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

    return rc;
}


status_t read_file(theater_play_t *theater_plays_arr, size_t *theater_plays_q)
{
    status_t rc = SUCCCESS_CODE;
    char filename[MAX_STR_LEN];
    char play_type_str[MAX_STR_LEN];
    FILE *filestream = NULL;
    *theater_plays_q = 0;
    int fields_read = 0;

    if (theater_plays_arr == NULL)
        rc = ARR_PROCESSING_ERROR;

    if (rc == SUCCCESS_CODE)
    {
        printf("%sВведите имя файла (предполагаемой базы данных): %s", BLUE, RESET);
        if (scanf("%s", filename) != 1)
            rc = INVALID_INPUT;
    }
        
    if (rc == SUCCCESS_CODE)
    {
        filestream = fopen(filename, "r");
        if (filestream == NULL)
            rc = FILE_OPEN_ERROR;
    }

    if (rc == SUCCCESS_CODE)
    {
        do
        {
            if (*theater_plays_q >= MAX_PLAYS_QUANTITY)
                rc = RECORDS_ARR_OVERFLOWED;
            else 
            {
                fields_read = fscanf(filestream, "%s %s %lf %lf",
                                    theater_plays_arr[*theater_plays_q].theater_name,
                                    theater_plays_arr[*theater_plays_q].play_name,
                                    &theater_plays_arr[*theater_plays_q].ticket_price,
                                    &theater_plays_arr[*theater_plays_q].max_ticket_price);
            }

            if (rc == SUCCCESS_CODE && fields_read != 4)
                rc = FILE_READ_ERROR;

            if (rc == SUCCCESS_CODE)
            {
                if (fscanf(filestream, "%s", play_type_str) != 1)
                    rc = INVALID_INPUT;

                if (strcmp(play_type_str, "PIECE") == 0)
                    theater_plays_arr[*theater_plays_q].play_type = PIECE;
                else if (strcmp(play_type_str, "MUSICAL") == 0)
                    theater_plays_arr[*theater_plays_q].play_type = MUSICAL;
                else 
                    rc = INVALID_INPUT;
            }

            if (rc == SUCCCESS_CODE)
            {
                switch (theater_plays_arr[*theater_plays_q].play_type)
                {
                    case PIECE:
                        rc = read_piece_data(filestream, &theater_plays_arr[*theater_plays_q]);
                        break;

                    case MUSICAL:
                        rc = read_musical_data(filestream, &theater_plays_arr[*theater_plays_q]);
                        break;

                    default:
                        rc = INVALID_INPUT;
                        break;
                }
            }

            *theater_plays_q += (rc == SUCCCESS_CODE);
        } while (!feof(filestream) && rc == SUCCCESS_CODE);
    }
    
    if (filestream)
        fclose(filestream);

    // TODO: инициализировать массив ключей

    return rc;  
}


status_t read_piece_data(FILE *filestream, theater_play_t *current_play)
{
    status_t rc = SUCCCESS_CODE;
    char temp_piece_genre[MAX_STR_LEN];
    int temp_age;

    // COMMENT: возраст указывается как число (т.е. в годах)
    if (fscanf(filestream, "%s %d", temp_piece_genre, &temp_age) != 2)
    {
        printf("HERE\n");
        rc = FILE_READ_ERROR;
    }
    
    if (rc == SUCCCESS_CODE)
    {
        if (strcmp(temp_piece_genre, "DRAMA") == 0)
            current_play->play_data.piece_info.piece_genre = DRAMA;
        else if (strcmp(temp_piece_genre, "COMEDY") == 0)
            current_play->play_data.piece_info.piece_genre = COMEDY;
        else if (strcmp(temp_piece_genre, "FAIRY_TALE") == 0)
            current_play->play_data.piece_info.piece_genre = FAIRY_TALE;
        else 
            rc = INVALID_INPUT;
    }

    if (rc == SUCCCESS_CODE)
    {
        switch (temp_age)
        {
            case 3:
                current_play->play_data.piece_info.age_rating = AGE_3PLUS;
                break;
            case 10:
                current_play->play_data.piece_info.age_rating = AGE_10PLUS;
                break;
            case 16:
                current_play->play_data.piece_info.age_rating = AGE_16PLUS;
                break;
            default:
                rc = INVALID_INPUT;
                break;
        }
    }

    return rc;
}

status_t read_musical_data(FILE *filestream, theater_play_t *current_play)
{
    status_t rc = SUCCCESS_CODE;
    char temp_composer[MAX_STR_LEN];
    char temp_country[MAX_STR_LEN];
    char temp_musical_genre_str[MAX_STR_LEN];
    musical_genre_t temp_musical_genre_num = MUSICAL_SHOW;
    int temp_age;
    int temp_duration;

    // COMMENT: длительность указывается в минутах
    if (fscanf(filestream, "%s %s %s %d %d", temp_composer, temp_country, temp_musical_genre_str, &temp_age, &temp_duration) != 5)
        rc = INVALID_INPUT;

    if (rc == SUCCCESS_CODE)
    {
        if (strcmp(temp_musical_genre_str, "BALLET") == 0)
            temp_musical_genre_num = BALLET;
        else if (strcmp(temp_musical_genre_str, "OPERA") == 0)
            temp_musical_genre_num = OPERA;
        else if (strcmp(temp_musical_genre_str, "MUSICAL_SHOW") == 0)
            temp_musical_genre_num = MUSICAL_SHOW;
        else
            rc = INVALID_INPUT;
    }

    // TODO: выделить этот блок в отдельную функцию
    if (rc == SUCCCESS_CODE)
    {
        switch (temp_age)
        {
            case 3:
                temp_age = AGE_3PLUS;
                break;
            case 10:
                temp_age = AGE_10PLUS;
                break;
            case 16:
                temp_age = AGE_16PLUS;
                break;
            default:
                rc = INVALID_INPUT;
                break;
        }
    }

    if (rc == SUCCCESS_CODE)
        if (temp_duration <= 0)
            rc = INVALID_INPUT;

    if (rc == SUCCCESS_CODE)
    {
        strncpy(current_play->play_data.musical_info.composer, temp_composer, MAX_STR_LEN);
        strncpy(current_play->play_data.musical_info.country, temp_country, MAX_STR_LEN);
        current_play->play_data.musical_info.musical_genre = temp_musical_genre_num;
        current_play->play_data.musical_info.age_rating = temp_age;
        current_play->play_data.musical_info.duration = temp_duration;
    }

    return rc;
}