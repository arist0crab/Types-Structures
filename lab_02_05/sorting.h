#ifndef __SORTING_H__
#define __SORTING_H__

#include "status.h"
#include "data.h"
#include <stdio.h>

status_t quick_sort_by_ticket_price(theater_play_t *theater_plays_arr, size_t left, size_t right);
status_t slow_sort_by_ticket_price(theater_play_t *theater_plays_arr, size_t *theater_plays_q);

#endif