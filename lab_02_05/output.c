#include "output.h"


status_t print_theater_plays_table(theater_play_t *theater_plays_arr, size_t theater_plays_q)
{
    status_t rc = SUCCCESS_CODE;
    theater_play_t current_theater_play;

    if (theater_plays_arr == NULL || (int)theater_plays_q <= 0)
        rc = ARR_PROCESSING_ERROR;

    if (rc == SUCCCESS_CODE)
    {
        printf("+------------------------------------------------------------------------------------------------------------------------------------------+\n");
        printf("|  N  |   Театр   |  Спектакль  |    Цена    |  Цена (max)  |  Тип  |  Жанр  |  Возраст  |  Время (мин)  |  Композитор  |  Страна  |  Вид  |\n");
        printf("+------------------------------------------------------------------------------------------------------------------------------------------+\n");
        for (size_t i = 0; i < theater_plays_q; i++)
        {
            current_theater_play = theater_plays_arr[i];
            printf("| %-3ld | %-9s | %-11s | %-10.2lf | %-12.2lf |\n",
            i + 1,
            current_theater_play.theater_name,
            current_theater_play.play_name,
            current_theater_play.ticket_price,
            current_theater_play.max_ticket_price

            
            );
            printf("+------------------------------------------------------------------------------------------------------------------------------------------+\n");
        }
    }

    return rc;
}