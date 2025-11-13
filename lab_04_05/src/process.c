#define _POSIX_C_SOURCE 199309L
#include "process.h"

status_t compare_pop_push(free_blocks_array_t *free_blocks);
status_t compare_performance(free_blocks_array_t *free_blocks);
status_t create_random_expression(char *expression, const size_t expression_size, char operation);

status_t procces_menu_choice(int option, arr_stack_t *arr_stack, list_stack_t *list_stack, free_blocks_array_t *free_blocks)
{
    status_t ec = SUCCESS_CODE;
    char expression[MAX_EXPRESSION_SIZE];
    int stack_elem = 0;

    if (arr_stack == NULL)
        return ERR_INVALID_POINTER;

    switch (option)
    {
        case EXIT_PROGRAM:
            break;

        case CALC_ARITHMETIC_EXPRESSION_BY_ARRAY:
            ec = get_expression(expression);
            if (ec == SUCCESS_CODE)
                ec = calc_arithmetic_expr_by_arr(expression, &stack_elem);
            if (ec == SUCCESS_CODE)
                print_calc_result(stack_elem);
            break;

        case ADD_STACK_ELEMENT_TO_ARRAY:
            ec = get_stack_elem(&stack_elem);
            if (ec == SUCCESS_CODE)
                ec = push_arr_stack(arr_stack, stack_elem);
            break;

        case DELETE_STACK_ELEMENT_FROM_ARRAY:
            ec = pop_arr_stack(arr_stack, &stack_elem);
            if (ec == SUCCESS_CODE)
                print_pop_result(stack_elem);
            break;

        case PRINT_STACK_FROM_ARRAY:
            ec = print_arr_stack(arr_stack);
            break;

        case CALC_ARITHMETIC_EXPRESSION_BY_LIST:
            ec = get_expression(expression);
            if (ec == SUCCESS_CODE)
                ec = calc_arithmetic_expr_by_list(expression, &stack_elem, free_blocks);
            if (ec == SUCCESS_CODE)
                print_calc_result(stack_elem);
            break;

        case ADD_STACK_ELEMENT_TO_LIST:
            ec = get_stack_elem(&stack_elem);
            if (ec == SUCCESS_CODE)
                ec = push_list_stack(list_stack, stack_elem);
            break;

        case DELETE_STACK_ELEMENT_FROM_LIST:
            ec = pop_list_stack(list_stack, &stack_elem, free_blocks);
            if (ec == SUCCESS_CODE)
                print_pop_result(stack_elem);
            break;

        case PRINT_STACK_FROM_LIST:
            ec = print_list_stack(list_stack);
            break;

        case COMPARE_PERFORMANCE:
            ec = compare_performance(free_blocks);
            break;

        case COMPARE_POP_PUSH:
            ec = compare_pop_push(free_blocks);
            break;

        case PRINT_ARRAY_OF_FREE_MEMORY_AREAS:
            ec = print_free_blocks(free_blocks);
            break;
        
        default:
            ec = ERR_RANGE;
            break;
    }

    return ec;
}

status_t compare_pop_push(free_blocks_array_t *free_blocks)
{
    status_t ec = SUCCESS_CODE;
    arr_stack_t arr_stack = { { 0 }, 0 };
    list_stack_t list_stack = { NULL, 0 };;
    struct timespec start_time, end_time;

    double time_array_push = 0.0;
    double time_array_pop = 0.0;
    double time_list_push = 0.0;
    double time_list_pop = 0.0;

    srand(time(NULL));

    // замеряем время push для array
    for (size_t i = 0; i < MAX_ARR_STACK_SIZE; i++)
    {
        clock_gettime(CLOCK_MONOTONIC, &start_time);
        push_arr_stack(&arr_stack, rand() % 100 + 1);
        clock_gettime(CLOCK_MONOTONIC, &end_time);
        time_array_push += (end_time.tv_sec - start_time.tv_sec) * 10e9 + (end_time.tv_nsec - start_time.tv_nsec);
    }

    // замеряем время pop для array
    for (size_t i = 0; i < MAX_ARR_STACK_SIZE; i++)
    {
        clock_gettime(CLOCK_MONOTONIC, &start_time);
        pop_arr_stack(&arr_stack, NULL);
        clock_gettime(CLOCK_MONOTONIC, &end_time);
        time_array_pop += (end_time.tv_sec - start_time.tv_sec) * 10e9 + (end_time.tv_nsec - start_time.tv_nsec);
    }

    // замеряем время push для list
    for (size_t i = 0; i < MAX_ARR_STACK_SIZE; i++)
    {
        clock_gettime(CLOCK_MONOTONIC, &start_time);
        push_list_stack(&list_stack, rand() % 100 + 1);
        clock_gettime(CLOCK_MONOTONIC, &end_time);
        time_list_push += (end_time.tv_sec - start_time.tv_sec) * 10e9 + (end_time.tv_nsec - start_time.tv_nsec);
    }

    // замеряем время pop для list
    for (size_t i = 0; i < MAX_ARR_STACK_SIZE; i++)
    {
        clock_gettime(CLOCK_MONOTONIC, &start_time);
        pop_list_stack(&list_stack, NULL, free_blocks);
        clock_gettime(CLOCK_MONOTONIC, &end_time);
        time_list_pop += (end_time.tv_sec - start_time.tv_sec) * 10e9 + (end_time.tv_nsec - start_time.tv_nsec);
    }

    time_array_push /= MAX_ARR_STACK_SIZE;
    time_array_pop /= MAX_ARR_STACK_SIZE;
    time_list_push /= MAX_ARR_STACK_SIZE;
    time_list_pop /= MAX_ARR_STACK_SIZE;

    print_push_pop_table(time_array_push, time_array_pop, time_list_push, time_list_pop);

    // освобождаем list
    free_list_stack(&list_stack);

    return ec;
}

status_t compare_performance(free_blocks_array_t *free_blocks)
{
    status_t ec = SUCCESS_CODE;
    char expression[MAX_COMPARE_EXPRESSION_SIZE];
    char operations[OPERATIONS_QUANTITY] = { '+', '-', '/', '*' };
    size_t stack_sizes[STACK_SIZES_QUANTITY] = { 10, 50, 100, 500, 1000 };
    struct timespec start_time, end_time;
    int calculation_result = 0;

    double total_time_array = 0.0;
    double total_time_list = 0.0;
    size_t memory_usage_array = 0;
    size_t memory_usage_list = 0;

    srand(time(NULL));

    if (ec == SUCCESS_CODE)
        print_compare_table_header(); 

    for (size_t i = 0; ec == SUCCESS_CODE && i < OPERATIONS_QUANTITY; i++)
    {
        for (size_t j = 0; ec == SUCCESS_CODE && j < STACK_SIZES_QUANTITY; j++)
        {
            total_time_array = 0.0;
            total_time_list = 0.0;
            memory_usage_array = 0;
            memory_usage_list = 0;

            for (size_t k = 0; ec == SUCCESS_CODE && k < PERFORMANCE_ITERATIONS; k++)
            {
                ec = create_random_expression(expression,stack_sizes[j], operations[i]);
                if (ec == SUCCESS_CODE)
                {
                    clock_gettime(CLOCK_MONOTONIC, &start_time);
                    ec = calc_arithmetic_expr_by_arr(expression, &calculation_result);
                    clock_gettime(CLOCK_MONOTONIC, &end_time);
                    total_time_array += (end_time.tv_sec - start_time.tv_sec) * 10e9 + (end_time.tv_nsec - start_time.tv_nsec);
                }
            }

            for (size_t k = 0; ec == SUCCESS_CODE && k < PERFORMANCE_ITERATIONS; k++)
            {
                ec = create_random_expression(expression, stack_sizes[j], operations[i]);
                if (ec == SUCCESS_CODE)
                {
                    clock_gettime(CLOCK_MONOTONIC, &start_time);
                    ec = calc_arithmetic_expr_by_list(expression, &calculation_result, free_blocks);
                    clock_gettime(CLOCK_MONOTONIC, &end_time);
                    total_time_list += (end_time.tv_sec - start_time.tv_sec) * 10e9 + (end_time.tv_nsec - start_time.tv_nsec);
                }
            }

            memory_usage_array += sizeof(arr_stack_t);
            memory_usage_list += sizeof(list_stack_t) + sizeof(node_t) * stack_sizes[j];
            total_time_array /= PERFORMANCE_ITERATIONS;
            total_time_list /= PERFORMANCE_ITERATIONS;

            print_table_raw(operations[i], stack_sizes[j], memory_usage_array, memory_usage_list, total_time_array, total_time_list);
        }
    }

    return ec;
}

status_t create_random_expression(char *expression, const size_t expression_size, char operation)
{
    status_t ec = SUCCESS_CODE;
    int offset = 0, rand_num = 0;

    if (expression == NULL)
        ec = ERR_INVALID_POINTER;
    else if (operation != '+' && operation != '-' && operation != '*' && operation != '/')
        ec = ERR_RANGE;

    // создаем случайное выражение для экспримента
    if (ec == SUCCESS_CODE)
    {
        memset(expression, 0, MAX_COMPARE_EXPRESSION_SIZE);
        offset = 0;
        rand_num = rand() % 100 + 1;
        offset += snprintf(expression + offset, MAX_COMPARE_EXPRESSION_SIZE - offset, "%d", rand_num);
        for (size_t num_index = 0; num_index < expression_size - 1; num_index++)
        {
            rand_num = rand() % 100 + 1;
            offset += snprintf(expression + offset, MAX_COMPARE_EXPRESSION_SIZE - offset, " %c %d",  operation, rand_num);
        }
    }

    return ec;
}