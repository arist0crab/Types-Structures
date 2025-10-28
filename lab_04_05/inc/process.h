#ifndef __PROCESS_H__
#define __PROCESS_H__

#include "data.h"
#include "input.h"
#include "arr_stack.h"
#include "list_stack.h"
#include "output.h"
#include <time.h>

status_t procces_menu_choice(int option, arr_stack_t *arr_stack, list_stack_t *list_stack, free_blocks_array_t *free_blocks);

#endif