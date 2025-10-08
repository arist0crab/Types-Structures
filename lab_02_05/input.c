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

status_t input_age_rating_and_duration(age_rating_t *target_age_rating, int *target_duration)
{
    int temp_int;

    printf("%s", BLUE);
    printf("Введите искомый возрастной рейтинг:\n0 - 3+\n1 - 10+\n2 - 16+\n");
    printf("%s", RESET);
    if (scanf("%d", &temp_int) != 1 || temp_int < 0 || temp_int > 2)
        return INVALID_CHOICE;
    *target_age_rating = (age_rating_t)temp_int;

    printf("%s", BLUE);
    printf("Введите продолжительность балета (которая будет больше искомых): ");
    printf("%s", RESET);
    if (scanf("%d", target_duration) != 1 || *target_duration <= 0)
        return INVALID_INPUT;
    
    return SUCCCESS_CODE;
}