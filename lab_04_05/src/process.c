#define _POSIX_C_SOURCE 199309L
#include "process.h"

status_t compare_performance(void);

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
            ec = compare_performance();
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


status_t compare_performance(void)
{
    status_t ec = SUCCESS_CODE;
    char expression[MAX_EXPRESSION_SIZE];
    char operations[OPERATIONS_QUANTITY] = { '+', '-', '/', '*' };
    size_t stack_sizes[STACK_SIZES_QUANTITY] = { 10, 50, 100, 500, 1000 };
    struct timespec start_time, end_time;
    int rand_num = 0, offset = 0;

    double total_time_array = 0.0;
    double total_time_list = 0.0;
    size_t memory_usage_array = 0;
    size_t memory_usage_list = 0;

    srand(time(NULL));

    // printf("+-----------+------------+----------------+---------------+--------------+-------------+\n");
    // printf("| operation | stack size | memory (array) | memory (list) | time (array) | time (list) |\n");
    // printf("+-----------+------------+----------------+---------------+--------------+-------------+\n");

    for (size_t i = 0; i < OPERATIONS_QUANTITY; i++)
    {
        for (size_t j = 0; j < STACK_SIZES_QUANTITY; j++)
        {
            total_time_array = 0.0;
            total_time_list = 0.0;
            memory_usage_array = 0;
            memory_usage_list = 0;

            for (size_t k = 0; k < PERFORMANCE_ITERATIONS; k++)
            {
                


            }

            printf("|     %c     | %-10lu | %-14lu | %-13lu | %-12lf | %-11lf |\n", operations[i], stack_sizes[j], memory_usage_array, memory_usage_list, total_time_array, total_time_list);
            printf("+-----------+------------+----------------+---------------+--------------+-------------+\n");
        }
    }

    return ec;
}

status_t create_random_expression(char *expression, char operation)
{
    status_t ec = SUCCESS_CODE;
    int offset = 0, rand_num = 0;

    if (expression == NULL)
        ec = ERR_INVALID_POINTER;
    else if (operation != '+' || operation != '-' || operation != '*' || operation != '/')
        ec = ERR_RANGE;
    
    // TODO

    // создаем случайное выражение для экспримента
    if (ec == SUCCESS_CODE)
    {
        memset(expression, 0, MAX_EXPRESSION_SIZE);
        offset = 0;
        rand_num = rand() % 100 + 1;
        offset += snprintf(expression + offset, MAX_EXPRESSION_SIZE - offset, "%d", rand_num);
        for (size_t num_index = 0; num_index < MAX_OPERANDS_QUANTITY - 1; num_index++)
        {
            rand_num = rand() % 100 + 1;
            offset += snprintf(expression + offset, MAX_EXPRESSION_SIZE - offset, " %c %d",  operations[i], rand_num);
        }
    }

    return ec;
}