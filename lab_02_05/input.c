#include "input.h"

status_t input_choice(choice_t *choice)
{
    status_t rc = SUCCCESS_CODE;

    int temp_num;
    printf("%sВыберите пункт меню: %s", BLUE, RESET);
    if (scanf("%d", &temp_num) != 1)
        rc = INVALID_INPUT;

    if (rc == SUCCCESS_CODE)
        if (temp_num < 0 || temp_num >= CHOICES_QUANTITY)
            rc = INVALID_INPUT;

    if (rc == SUCCCESS_CODE)
        *choice = temp_num;

    return rc;
}