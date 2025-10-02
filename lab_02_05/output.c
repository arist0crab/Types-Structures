#include "output.h"

char *get_string_play_type(play_type_t play_type);
char *get_general_genre(theater_play_t *cur_play);
char *get_sring_piece_genre(piece_genre_t piece_genre);
char *get_sring_musical_genre(musical_genre_t musical_genre);
char *get_age_rating_string(theater_play_t *cur_play);


status_t print_theater_plays_table(theater_play_t *theater_plays_arr, size_t theater_plays_q)
{
    status_t rc = SUCCCESS_CODE;
    theater_play_t *current_theater_play;

    if (theater_plays_arr == NULL || (int)theater_plays_q <= 0)
        rc = ARR_PROCESSING_ERROR;

    if (rc == SUCCCESS_CODE)
    {
        printf("+------------------------------------------------------------------------------------------------------------------------------------------+\n");
        printf("|  N  |   Театр   |  Спектакль  |    Цена    |  Цена (max)  |  Тип  |  Жанр  |  Возраст  |  Время (мин)  |  Композитор  |  Страна  |  Вид  |\n");
        printf("+------------------------------------------------------------------------------------------------------------------------------------------+\n");
        for (size_t i = 0; i < theater_plays_q; i++)
        {
            current_theater_play = &theater_plays_arr[i];
            printf("| %-3ld | %-9s | %-11s | %-10.2lf | %-12.2lf | %-5s | %-6s | %-9s | %-13d | %-12s | %-8s | %-5s |\n",
            i + 1,
            current_theater_play->theater_name,
            current_theater_play->play_name,
            current_theater_play->ticket_price,
            current_theater_play->max_ticket_price,
            get_string_play_type(current_theater_play->play_type),
            get_general_genre(current_theater_play),
            get_age_rating_string(current_theater_play),
            (current_theater_play->play_type == PIECE) ? 0 : current_theater_play->play_data.musical_info.duration,
            (current_theater_play->play_type == MUSICAL) ? current_theater_play->play_data.musical_info.composer : "",
            (current_theater_play->play_type == MUSICAL) ? current_theater_play->play_data.musical_info.country : "",
            (current_theater_play->play_type == MUSICAL) ? get_sring_musical_genre(current_theater_play->play_data.musical_info.musical_genre) : ""
            );
            printf("+------------------------------------------------------------------------------------------------------------------------------------------+\n");
        }
    }

    return rc;
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


char *get_age_rating_string(theater_play_t *cur_play)
{
    age_rating_t cur_age_rating;

    switch (cur_play->play_type)
    {
        case PIECE:
            cur_age_rating = cur_play->play_data.piece_info.age_rating;
            break;

        case MUSICAL:
            cur_age_rating = cur_play->play_data.musical_info.age_rating;
            break;
        
        default:
            cur_age_rating = -1;
            break;
    }

    switch (cur_age_rating)
    {
        case AGE_3PLUS: return "3+";
        case AGE_10PLUS: return "10+";
        case AGE_16PLUS: return "16+";
        default: return "ERROR";
    }

}