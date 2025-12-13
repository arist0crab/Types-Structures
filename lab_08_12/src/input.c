#include "../inc/input.h"  // TODO

status_t input_cur_menu_opt(int *cur_menu_opt, int const menu_points_quantity)
{
    status_t ec = (cur_menu_opt) ? SUCCESS_CODE : ERR_ARGS;
    char buffer[MAX_STRING_LENGTH], extra;
    int temp, tt;
   
    printf("%sВыберите пункт меню: %s", BLUE, RESET);

    if (ec == SUCCESS_CODE)
        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
            ec = ERR_IO;

    if (ec == SUCCESS_CODE)
    {
        if ((tt = sscanf(buffer, "%d %c", &temp, &extra)) != 1)
            ec = ERR_IO;
        else if (temp < 0 || temp >= menu_points_quantity)
            ec = ERR_RANGE;
        else
            *cur_menu_opt = temp;
    }

    return ec;
}

status_t input_string(char **word, const char *message)
{
    status_t ec = SUCCESS_CODE;
    char buffer[MAX_STRING_LENGTH];
    char extra;

    if (!word)
        ec = ERR_ARGS;

    if (ec == SUCCESS_CODE)
    {
        printf("%s%s%s", BLUE, message, RESET);
        if (scanf("%s", buffer) != 1)
            ec = ERR_IO;
    }

    if (ec == SUCCESS_CODE)
    {
        *word = malloc(strlen(buffer) + 1);
        if (!(*word))
            ec = ERR_MEM;
    }

    if (ec == SUCCESS_CODE)
        strcpy(*word, buffer);

    // чистим буфер
    while ((extra = getchar()) != '\n' && extra != EOF);

    return ec;
}

status_t input_size(size_t *size, const char *message)
{
    status_t ec = SUCCESS_CODE;
    char extra;

    if (!size) ec = ERR_ARGS;

    if (ec == SUCCESS_CODE)
    {
        printf("%s%s%s", BLUE, message, RESET);
        if (scanf("%lu", size) != 1 || size == 0)
            ec = ERR_IO;
    }

    // чистим буфер
    while ((extra = getchar()) != '\n' && extra != EOF);

    return ec;
}

status_t get_two_cities_and_distances(char **city_name_1, char **city_name_2, size_t *distance_1_to_2, size_t *distance_2_to_1)
{
    status_t ec = SUCCESS_CODE;
    char message[MAX_STRING_LENGTH];

    // получаем названия городов
    ec = input_string(city_name_1, "Введите город №1: ");
    if (ec == SUCCESS_CODE)
        ec = input_string(city_name_2, "Введите город №2: ");

    // получаем дистанции
    if (ec == SUCCESS_CODE)
    {
        snprintf(message, MAX_STRING_LENGTH, "Введите длину дороги из %s в %s: ", *city_name_1, *city_name_2);
        ec = input_size(distance_1_to_2, message);
    }
    if (ec == SUCCESS_CODE)
    {
        snprintf(message, MAX_STRING_LENGTH, "Введите длину дороги из %s в %s: ", *city_name_2, *city_name_1);
        ec = input_size(distance_2_to_1, message);
    }

    return ec;
}