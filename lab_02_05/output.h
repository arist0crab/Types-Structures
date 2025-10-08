#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include "data.h"
#include "status.h"
#include <stdio.h>
#include <stdbool.h>

status_t print_theater_plays_table(theater_play_t *theater_plays_arr, int *theater_plays_keys, size_t theater_plays_q, bool print_by_keys);
status_t print_keys_table(theater_play_t *theater_plays_arr, int *theater_plays_keys, size_t theater_plays_q);

#endif