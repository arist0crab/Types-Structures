#include "output.h"

char *get_string_play_type(play_type_t play_type);
char *get_general_genre(theater_play_t *cur_play);
char *get_sring_piece_genre(piece_genre_t piece_genre);
char *get_sring_musical_genre(musical_genre_t musical_genre);
char *get_age_rating_string(theater_play_t *cur_play);
char *get_string_duration(int duration);


status_t print_theater_plays_table(theater_play_t *theater_plays_arr, size_t theater_plays_q)
{
    status_t rc = SUCCCESS_CODE;
    theater_play_t *current_theater_play;

    if (theater_plays_arr == NULL || (int)theater_plays_q < 0)
        rc = ARR_PROCESSING_ERROR;

    if (rc == SUCCCESS_CODE && theater_plays_q == 0)
    {
        printf("+------------------------------------------------------------------------------------------------------------------------------------------+\n");
        printf("|                                          М А С С И В   П У С Т   П Р О С Т И Т Е   :(((                                                  |\n");
        printf("+------------------------------------------------------------------------------------------------------------------------------------------+\n");
    }

    if (rc == SUCCCESS_CODE && theater_plays_q > 0)
    {
        printf("+------------------------------------------------------------------------------------------------------------------------------------------+\n");
        printf("|  N  |     Театр     |   Спектакль   |    Цена    |  Цена (max)  |  Тип  |  Жанр  |  Возраст  |  Время (мин)  |  Композитор  |   Страна   |\n");
        printf("+------------------------------------------------------------------------------------------------------------------------------------------+\n");
        for (size_t i = 0; i < theater_plays_q; i++)
        {
            current_theater_play = &theater_plays_arr[i];
            printf("| %-3ld | %-13s | %-13s | %-10.2lf | %-12.2lf | %-5s | %-6s | %-9s | %-13s | %-12s | %-10s |\n",
            i + 1,
            current_theater_play->theater_name,
            current_theater_play->play_name,
            current_theater_play->ticket_price,
            current_theater_play->max_ticket_price,
            get_string_play_type(current_theater_play->play_type),
            get_general_genre(current_theater_play),
            get_age_rating_string(current_theater_play),
            (current_theater_play->play_type == MUSICAL) ? get_string_duration(current_theater_play->play_data.musical_info.duration) : "",
            (current_theater_play->play_type == MUSICAL) ? current_theater_play->play_data.musical_info.composer : "",
            (current_theater_play->play_type == MUSICAL) ? current_theater_play->play_data.musical_info.country : ""
            );
            printf("+------------------------------------------------------------------------------------------------------------------------------------------+\n");

        }
    }

    return rc;
}

status_t print_keys_table(theater_play_t *theater_plays_arr, int *theater_plays_keys, size_t theater_plays_q)
{
    size_t theater_index;

    if (theater_plays_keys == NULL || (int)theater_plays_q < 0)
        return ARR_PROCESSING_ERROR;

    if (theater_plays_q <= 0)
    {
        printf("+------------------------------------------------------------------------------------------------------------------------------------------+\n");
        printf("|                                          М А С С И В   П У С Т   П Р О С Т И Т Е   :(((                                                  |\n");
        printf("+------------------------------------------------------------------------------------------------------------------------------------------+\n");
    }
    else 
    {
        printf("+-------------------------+\n");
        printf("|  Номер  |  Цена билета  |\n");
        printf("+-------------------------+\n");
        for (size_t i = 0; i < theater_plays_q; i++)
        {
            theater_index = theater_plays_keys[i];
            printf("| %-7ld | %-13.2lf |\n", theater_index + 1, theater_plays_arr[theater_index].ticket_price);
            printf("+-------------------------+\n");
        }
    }

    return SUCCCESS_CODE;
}

char *get_general_genre(theater_play_t *cur_play)
{
    switch (cur_play->play_type)
    {
        case PIECE: return get_sring_piece_genre(cur_play->play_data.piece_info.piece_genre);
        case MUSICAL: return get_sring_musical_genre(cur_play->play_data.musical_info.musical_genre);
        default: return "ERROR";
    }
}

char *get_sring_piece_genre(piece_genre_t piece_genre)
{
    switch (piece_genre)
    {
        case DRAMA: return "DRAMA";
        case COMEDY: return "COMEDY";
        case FAIRY_TALE: return "FAIRY";
        default: return "ERROR";
    }
}

char *get_sring_musical_genre(musical_genre_t musical_genre)
{
    switch (musical_genre)
    {
        case BALLET: return "BALLET";
        case OPERA: return "OPERA";
        case MUSICAL_SHOW: return "MSHOW";
        default: return "ERROR";
    }
}

char *get_string_duration(int duration)
{
    static char duration_string[MAX_STR_LEN];
    sprintf(duration_string, "%d min.", duration);
    return duration_string;
}

char *get_string_play_type(play_type_t play_type)
{
    switch (play_type)
    {
        case PIECE: return "PIECE";
        case MUSICAL: return "MUSIC";
        default: return "ERROR";
    }
}

char *get_age_rating_string(theater_play_t *cur_play)
{
    switch (cur_play->age_rating)
    {
        case AGE_RATING_NONE: return "";
        case AGE_3PLUS: return "3+";
        case AGE_10PLUS: return "10+";
        case AGE_16PLUS: return "16+";
        default: return "ERROR";
    }
}