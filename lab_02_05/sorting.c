#include "sorting.h"

status_t quick_sort_by_ticket_price(theater_play_t *theater_plays_arr, int left, int right)
{
    theater_play_t temp_play;

    if (theater_plays_arr == NULL)
        return INVALID_SORT;

    if (left < right)
    {
        int i = left, j = right;
        double pivot = theater_plays_arr[left].ticket_price;

        while (i <= j)
        {
            while (theater_plays_arr[i].ticket_price < pivot)
                i++;
            while (theater_plays_arr[j].ticket_price > pivot)
                j--;
            if (i <= j)
            {
                temp_play = theater_plays_arr[i];
                theater_plays_arr[i] = theater_plays_arr[j];
                theater_plays_arr[j] = temp_play;
                i++;
                j--;
            }
        }
        quick_sort_by_ticket_price(theater_plays_arr, left, j);
        quick_sort_by_ticket_price(theater_plays_arr, i, right);
    }

    return SUCCCESS_CODE;
}

status_t quick_sort_by_keys(theater_play_t *theater_plays_arr, int *theater_plays_keys, int left, int right)
{
    int temp_int;
    if (theater_plays_arr == NULL || theater_plays_keys == NULL)
        return INVALID_SORT;

    if (left < right)
    {
        int i = left, j = right;
        double pivot = theater_plays_arr[theater_plays_keys[left]].ticket_price;

        while (i <= j)
        {
            while (theater_plays_arr[theater_plays_keys[i]].ticket_price < pivot)
                i++;
            while (theater_plays_arr[theater_plays_keys[j]].ticket_price > pivot)
                j--;    
            if (i <= j)
            {
                temp_int = theater_plays_keys[i];
                theater_plays_keys[i] = theater_plays_keys[j];
                theater_plays_keys[j] = temp_int;
                i++;
                j--;
            }
        }
        if (left < j)
            quick_sort_by_keys(theater_plays_arr, theater_plays_keys, left, j);
        if (i < right)
            quick_sort_by_keys(theater_plays_arr, theater_plays_keys, i, right);
    }
    
    return SUCCCESS_CODE;
}

status_t slow_sort_by_keys(theater_play_t *theater_plays_arr, int *theater_plays_keys, size_t *theater_plays_q)
{
    int temp_int;
    if (theater_plays_arr == NULL || theater_plays_keys == NULL)
        return INVALID_SORT;

    for (size_t i = 0; i < *theater_plays_q - 1; i++)
        for (size_t j = 0; j < *theater_plays_q - i - 1; j++)
            if (theater_plays_arr[theater_plays_keys[j]].ticket_price > theater_plays_arr[theater_plays_keys[j + 1]].ticket_price)
            {
                temp_int = theater_plays_keys[j];
                theater_plays_keys[j] = theater_plays_keys[j + 1];
                theater_plays_keys[j + 1] = temp_int;
            }

    return SUCCCESS_CODE;
}

status_t slow_sort_by_ticket_price(theater_play_t *theater_plays_arr, size_t *theater_plays_q)
{
    theater_play_t temp_play;

    if (theater_plays_arr == NULL || *theater_plays_q <= 0)
        return INVALID_SORT;

    for (size_t i = 0; i < *theater_plays_q - 1; i++)
        for (size_t j = 0; j < *theater_plays_q - i - 1; j++)
            if (theater_plays_arr[j].ticket_price > theater_plays_arr[j + 1].ticket_price)
            {
                temp_play = theater_plays_arr[j];
                theater_plays_arr[j] = theater_plays_arr[j + 1];
                theater_plays_arr[j + 1] = temp_play;
            }

    return SUCCCESS_CODE;
}