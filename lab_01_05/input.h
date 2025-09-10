#ifndef INPUT_H
#define INPUT_H

#include "defines.h"
#include "color.h"
#include "output.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void print_instructions(void);
void input_two_lfloats(lfloat_t *first_lfnum, lfloat_t *second_lfnum, exit_status *status);

#endif