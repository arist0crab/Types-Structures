#ifndef __INPUT_H__
#define __INPUT_H__

#include "status.h"
#include "data.h"
#include "color.h"
#include <stdio.h>

status_t input_choice(choice_t *choice);
status_t input_age_rating_and_duration(age_rating_t *target_age_rating, int *target_duration);

#endif