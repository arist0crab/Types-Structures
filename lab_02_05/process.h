#ifndef __PROCESS_H__
#define __PROCESS_H__

#include "data.h"
#include "status.h"
#include "color.h"
#include "input.h"
#include "output.h"
#include "read_file.h"
#include "print_instructions.h"
#include "sorting.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

status_t allocate_memory(void **elems_arr, size_t elems_quantity, size_t elem_size);
status_t process_choice(choice_t choice, bool *program_running, theater_play_t *theater_plays_arr, int *theater_plays_keys, size_t *theater_plays_q);

#endif