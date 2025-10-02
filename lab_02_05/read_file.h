#ifndef __FILE_READ_H__
#define __FILE_READ_H__

#include "data.h"
#include "status.h"
#include "color.h"
#include <stdio.h>
#include <string.h>

status_t read_file(theater_play_t *theater_plays_arr, size_t *theater_plays_q);

#endif