#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void print_instructions(void);
void input_lfloat(lfloat_t *lfnum, size_t max_lfloat_len, exit_status *status);

#endif