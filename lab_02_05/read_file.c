#include "read_file.h"

status_t get_and_open_file(FILE **filestream);
status_t read_base_fields(FILE *filestream, theater_play_t *current_play);
status_t read_selectable_fields(FILE *filestream, theater_play_t *current_play);
status_t read_piece_data(FILE *filestream, theater_play_t *current_play);
status_t read_musical_data(FILE *filestream, theater_play_t *current_play);
status_t get_age_rating_from_num(int age_num, age_rating_t *age_rating);


status_t read_file(theater_play_t *theater_plays_arr, int *theater_plays_keys, size_t *theater_plays_q)
{
    status_t rc = SUCCCESS_CODE;
    FILE *filestream = NULL;
    *theater_plays_q = 0;

    if (theater_plays_arr == NULL)
        rc = ARR_PROCESSING_ERROR;
        
    if (rc == SUCCCESS_CODE)
    {
        rc = get_and_open_file(&filestream);
        if (filestream == NULL)
            rc = FILE_OPEN_ERROR;
    }

    if (rc == SUCCCESS_CODE)
    {
        do
        {
            if (*theater_plays_q >= MAX_PLAYS_QUANTITY)
                rc = RECORDS_ARR_OVERFLOWED;
            
            if (rc == SUCCCESS_CODE)
                rc = read_base_fields(filestream, &theater_plays_arr[*theater_plays_q]);

            if (rc == SUCCCESS_CODE)
                rc = read_selectable_fields(filestream, &theater_plays_arr[*theater_plays_q]);

            *theater_plays_q += (rc == SUCCCESS_CODE);
        } while (!feof(filestream) && rc == SUCCCESS_CODE);
    }
    
    if (filestream)
        fclose(filestream);

    // инициализируем таблицу ключей
    if (rc == SUCCCESS_CODE)
        for (size_t i = 0; i < *theater_plays_q; i++)
            theater_plays_keys[i] = i;

    return rc;  
}


status_t get_and_open_file(FILE **filestream)
{
    status_t rc = SUCCCESS_CODE;
    char filename[MAX_STR_LEN];

    printf("%sВведите имя файла: %s", BLUE, RESET);
    if (scanf("%s", filename) != 1)
        rc = FILE_READ_ERROR;

    if (rc == SUCCCESS_CODE)
    {
        *filestream = fopen(filename, "r");
        if (*filestream == NULL)
            rc = FILE_OPEN_ERROR;
    }

    return rc;
}

status_t read_base_fields(FILE *filestream, theater_play_t *current_play)
{
    status_t rc = SUCCCESS_CODE;
    char play_type_str[MAX_STR_LEN];

    int fields_read = fscanf(filestream, "%s %s %lf %lf",
                        current_play->theater_name,
                        current_play->play_name,
                        &current_play->ticket_price,
                        &current_play->max_ticket_price);

    if (fields_read != 4)
        rc = FILE_READ_ERROR;

    if (rc == SUCCCESS_CODE && fscanf(filestream, "%s", play_type_str) != 1)
        rc = FILE_READ_ERROR;

    if (rc == SUCCCESS_CODE)
    {
        if (strcmp(play_type_str, "PIECE") == 0)
            current_play->play_type = PIECE;
        else if (strcmp(play_type_str, "MUSICAL") == 0)
            current_play->play_type = MUSICAL;
        else 
            rc = INVALID_INPUT;
    }

    return rc;
}

status_t read_selectable_fields(FILE *filestream, theater_play_t *current_play)
{
    status_t rc = SUCCCESS_CODE;

    switch (current_play->play_type)
    {
        case PIECE:
            rc = read_piece_data(filestream, current_play);
            break;

        case MUSICAL:
            rc = read_musical_data(filestream, current_play);
            break;

        default:
            rc = INVALID_INPUT;
            break;
    }

    return rc;
}

status_t read_piece_data(FILE *filestream, theater_play_t *current_play)
{
    status_t rc = SUCCCESS_CODE;
    char temp_piece_genre[MAX_STR_LEN];
    int temp_age;

    // читаем жанр пьесы
    if (fscanf(filestream, "%s", temp_piece_genre) != 1)
        rc = FILE_READ_ERROR;
    
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

    // если записываем сказку, считываем возраст
    if (rc == SUCCCESS_CODE && current_play->play_data.piece_info.piece_genre == FAIRY_TALE)
        if (fscanf(filestream, "%d", &temp_age) != 1)
            rc = FILE_READ_ERROR;

    if (rc == SUCCCESS_CODE)
    {
        if (current_play->play_data.piece_info.piece_genre == FAIRY_TALE)
            rc = get_age_rating_from_num(temp_age, &current_play->age_rating);
        else 
            current_play->age_rating = AGE_RATING_NONE;
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
    age_rating_t temp_age_rating;
    int temp_duration;
    int temp_age;

    if (fscanf(filestream, "%s %s %s %d %d", temp_composer, temp_country, temp_musical_genre_str, &temp_age, &temp_duration) != 5)
        rc = FILE_READ_ERROR;

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

    if (rc == SUCCCESS_CODE)
        rc = get_age_rating_from_num(temp_age, &temp_age_rating);

    if (rc == SUCCCESS_CODE)
        if (temp_duration <= 0)
            rc = INVALID_INPUT;

    if (rc == SUCCCESS_CODE)
    {
        strncpy(current_play->play_data.musical_info.composer, temp_composer, MAX_STR_LEN);
        strncpy(current_play->play_data.musical_info.country, temp_country, MAX_STR_LEN);
        current_play->play_data.musical_info.musical_genre = temp_musical_genre_num;
        current_play->age_rating = temp_age_rating;
        current_play->play_data.musical_info.duration = temp_duration;
    }

    return rc;
}

status_t get_age_rating_from_num(int age_num, age_rating_t *age_rating)
{
    status_t rc = SUCCCESS_CODE;

    switch (age_num)
    {
        case 3:
            *age_rating = AGE_3PLUS;
            break;
        case 10:
            *age_rating = AGE_10PLUS;
            break;
        case 16:
            *age_rating = AGE_16PLUS;
            break;
        default:
            rc = INVALID_INPUT;
            break;
    }

    return rc;
}