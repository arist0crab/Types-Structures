#include "input.h"

status_t input_cur_menu_opt(int *cur_menu_opt, int const menu_points_quantity)
{
    
    status_t ec = (cur_menu_opt) ? SUCCESS_CODE : ERR_INVALID_POINTER;
    char buffer[MAX_STRING_LENGTH], extra;
    int temp, tt;
   
    printf("%sВыберите пункт меню: %s", BLUE, RESET);

    if (ec == SUCCESS_CODE)
        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
            ec = ERR_IO;

    if (ec == SUCCESS_CODE)
    {
        if ((tt = sscanf(buffer, "%d %c", &temp, &extra)) != 1)
        {
            
            printf("im fucking here 2, tt = %d, buf = %s\n", tt, buffer);
            ec = ERR_IO;
        }
        else if (temp < 0 || temp >= menu_points_quantity)
            ec = ERR_RANGE;
        else
            *cur_menu_opt = temp;
    }

    return ec;
}

status_t input_time_gap(double *min_time, double *max_time)
{
    status_t ec = SUCCESS_CODE;
    double temp_min_time = 0, temp_max_time = 0;
    char extra;
    
    if (min_time == NULL || max_time == NULL)
        ec = ERR_INVALID_POINTER;

    if (ec == SUCCESS_CODE)
    {
        printf("%sВведите нижнюю границу: %s", BLUE, RESET);
        if (scanf("%lf", &temp_min_time) != 1)
            ec = ERR_IO;
    }

    if (ec == SUCCESS_CODE)
    {
        printf("%sВведите верхнюю границу: %s", BLUE, RESET);
        if (scanf("%lf", &temp_max_time) != 1)
            ec = ERR_IO;
    }

    // чистим буфер
    while ((extra = getchar()) != '\n' && extra != EOF);

    if (temp_min_time < temp_max_time)
    {
        *min_time = temp_min_time;
        *max_time = temp_max_time;
    }
    else 
        ec = ERR_RANGE;

    if (ec == SUCCESS_CODE)
        printf("%sИзменение временного промежутка прошло успешно!\n%s", GREEN_BOLD, RESET);

    return ec;
}