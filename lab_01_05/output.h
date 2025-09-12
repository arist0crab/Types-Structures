#ifndef OUTPUT_H
#define OUTPUT_H

#include "defines.h"
#include "color.h"
#include <stdio.h>

void print_result(const lfloat_t *num, exit_status *status);
void print_parsed_number(lfloat_t *lfloat_number);
void print_normalized_lfloat(const lfloat_t *num);

#endif