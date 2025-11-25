#include "input.h"

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

status_t input_string(char **word, char *message)
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

    // чистим буфе
    while ((extra = getchar()) != '\n' && extra != EOF);

    return ec;
}
