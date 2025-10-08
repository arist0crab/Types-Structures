#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include "data.h"
#include "status.h"
#include "color.h"
#include <stdio.h>
#include <stdbool.h>

status_t print_theater_plays_table(theater_play_t *theater_plays_arr, int *theater_plays_keys, size_t theater_plays_q, bool print_by_keys);
status_t print_keys_table(theater_play_t *theater_plays_arr, int *theater_plays_keys, size_t theater_plays_q);
status_t print_balets_by_conditions(theater_play_t *theater_plays_arr, size_t theater_plays_q, age_rating_t target_age_rating, int target_duration);
status_t print_efficiency_table(long long average_quick_sort_data_time, long long average_slow_sort_data_time, long long average_quick_sort_key_time, long long average_slow_sort_key_time);
status_t print_memory_data(size_t memory_used, size_t memory_used_keys);
status_t print_status_message(status_t rc);

#endif