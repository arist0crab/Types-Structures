#define _POSIX_C_SOURCE 199309L
#include "efficiency.h"

status_t print_efficiency_table(double average_arr_time, size_t average_arr_memory, double average_list_time, size_t average_list_memory);

status_t measure_efficiency(void)
{
    status_t ec = SUCCESS_CODE;
    double total_arr_time = 0, total_list_time = 0;
    double average_arr_time = 0, average_list_time = 0;
    size_t arr_memory = 0, list_memory = 0;
    struct timespec start_time, end_time;
    
    // замер эффективности для массива
    for (size_t i = 0; ec == SUCCESS_CODE && i < EFFICIENCY_ITERATIONS_QUANTITY; i++)
    {
        clock_gettime(CLOCK_MONOTONIC, &start_time);
        ec = simulate_service_unit_by_arr(false);
        clock_gettime(CLOCK_MONOTONIC, &end_time);
        total_arr_time += (end_time.tv_sec - start_time.tv_sec) * 1e9 + (end_time.tv_nsec - start_time.tv_nsec);
    }

    // замер эффективности для списка
    for (size_t i = 0; ec == SUCCESS_CODE && i < EFFICIENCY_ITERATIONS_QUANTITY; i++)
    {
        clock_gettime(CLOCK_MONOTONIC, &start_time);
        ec = simulate_service_unit_by_list(false);
        clock_gettime(CLOCK_MONOTONIC, &end_time);
        total_list_time += (end_time.tv_sec - start_time.tv_sec) * 1e9 + (end_time.tv_nsec - start_time.tv_nsec);
    }

    average_arr_time = total_arr_time / EFFICIENCY_ITERATIONS_QUANTITY;
    average_list_time = total_list_time / EFFICIENCY_ITERATIONS_QUANTITY;
    arr_memory = sizeof(arr_queue_t);
    list_memory = sizeof(list_queue_t) + MAX_LIST_LENGTH * sizeof(node_t);

    print_efficiency_table(average_arr_time, arr_memory, average_list_time, list_memory);

    return ec;
}

status_t print_efficiency_table(double average_arr_time, size_t average_arr_memory, double average_list_time, size_t average_list_memory)
{
    printf("%s", BLUE);
    printf("╔═════════════════════════════════════════╗\n");
    printf("║               EFFIENCY TABLE            ║\n");
    printf("╠════════════╦═══════════╦════════════════╣\n");
    printf("║ queue type ║ time (ms) ║ memory (bytes) ║\n"); 
    printf("╠════════════╬═══════════╬════════════════╣\n");
    printf("║    array   ║ %-9.2lf ║ %-14lu ║\n", average_arr_time, average_arr_memory);
    printf("╠════════════╬═══════════╬════════════════╣\n");
    printf("║    list    ║ %-9.2lf ║ %-14lu ║\n", average_list_time, average_list_memory);
    printf("╚════════════╩═══════════╩════════════════╝\n");
    printf("%s", RESET);

    return SUCCESS_CODE;
}