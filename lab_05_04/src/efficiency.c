#define _POSIX_C_SOURCE 199309L
#include "efficiency.h"

status_t print_address_arrs(const void **add_addrs, const void **remove_addrs, const size_t addrs_quantity);
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
    // TODO не MAX_QUEUE_SIZE, а брать максимум суммы двух очередей которые получились в процессе симуляции
    list_memory = sizeof(list_queue_t) + MAX_QUEUE_SIZE * sizeof(node_t);

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

status_t fill_and_empty_arr_queue(void) 
{
    status_t ec = SUCCESS_CODE;

    arr_queue_t arr_queue = { 0 };
    request_t request = { 0 };

    void *added_addrs[FILL_N_EMPTY_ITERATIONS_QUANTITY];
    void *removed_addrs[FILL_N_EMPTY_ITERATIONS_QUANTITY];
    size_t added_count = 0, removed_count = 0;

    // заполняем очередь
    for (size_t i = 0; ec == SUCCESS_CODE && i < FILL_N_EMPTY_ITERATIONS_QUANTITY; i++) 
    {
        ec = push_arr(&arr_queue, &request);
        if (ec == SUCCESS_CODE)
            added_addrs[added_count++] = (void *)(&arr_queue.data[arr_queue.pin == 0 ? FILL_N_EMPTY_ITERATIONS_QUANTITY - 1 : arr_queue.pin - 1]);
    }

    // очищаем очередь
    for (size_t i = 0; ec == SUCCESS_CODE && i < added_count; i++) 
    {
        ec = pop_arr(&arr_queue, &request);
        if (ec == SUCCESS_CODE)
            removed_addrs[removed_count++] = (void *)(&arr_queue.data[arr_queue.pout == 0 ? FILL_N_EMPTY_ITERATIONS_QUANTITY - 1 : arr_queue.pout - 1]);
    }

    if (added_count == removed_count)
        print_address_arrs((const void **)added_addrs, (const void **)removed_addrs, added_count);

    return ec;
}

status_t fill_and_empty_list_queue(void)
{
    status_t ec = SUCCESS_CODE;

    list_queue_t list_queue = { 0 };
    request_t request = { 0 };

    void *added_addrs[FILL_N_EMPTY_ITERATIONS_QUANTITY];
    void *removed_addrs[FILL_N_EMPTY_ITERATIONS_QUANTITY];
    size_t added_count = 0, removed_count = 0;

    // заполняем очередь
    for (size_t i = 0; ec == SUCCESS_CODE && i < FILL_N_EMPTY_ITERATIONS_QUANTITY; i++) 
    {
        ec = push_list(&list_queue, &request);
        if (ec == SUCCESS_CODE && added_count < FILL_N_EMPTY_ITERATIONS_QUANTITY)
            added_addrs[added_count++] = (void *)list_queue.pout;
    }
    // очищаем очередь
    for (size_t i = 0; ec == SUCCESS_CODE && i < added_count; i++) 
    {
        if (removed_count < FILL_N_EMPTY_ITERATIONS_QUANTITY)
            removed_addrs[removed_count++] = (void *)list_queue.pin;
        ec = pop_list(&list_queue, NULL);
    }

    if (added_count == removed_count)
        print_address_arrs((const void **)added_addrs, (const void **)removed_addrs, added_count);

    return SUCCESS_CODE;
}


status_t print_address_arrs(const void **add_addrs, const void **remove_addrs, const size_t addrs_quantity)
{
    printf("%s", BLUE);
    printf("╔══════╦═════════════════════════╦═════════════════════════╗\n");
    printf("║ indx ║     added addresses     ║    removed addresses    ║\n"); 

    for (size_t i = 0; i < addrs_quantity; i++)
    {
        printf("╠══════╬═════════════════════════╬═════════════════════════╣\n");
        printf("║ %-4lu ║ %-23p ║ %-23p ║\n", i + 1, add_addrs[i], remove_addrs[i]);
    }

    printf("╚══════╩═════════════════════════╩═════════════════════════╝\n");
    printf("%s", RESET);

    return SUCCESS_CODE;
}