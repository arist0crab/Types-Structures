#ifndef __PROCESS_H__
#define __PROCESS_H__

#include "data.h"
#include "status.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

status_t allocate_memory(void **elems_arr, size_t elems_quantity, size_t elem_size);
status_t process_choice(choice_t choice, bool *program_running);

#endif