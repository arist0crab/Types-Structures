#include "simulate.h"

// вспомогательные функции
status_t min3(double a, double b, double c, double *min_val);

// функции печати
status_t print_interim_results_table_header(void);
status_t print_interim_results_table_bottom(void);
status_t print_simulation_summary(double total_time, const simulation_log_t *log1, const simulation_log_t *log2, double downtime);
status_t print_interim_results_table_content_arr(const arr_queue_t *queue1, const simulation_log_t *log1, const arr_queue_t *queue2, const simulation_log_t *log2);
status_t print_interim_results_table_content_list(const list_queue_t *queue1, const simulation_log_t *log1, const list_queue_t *queue2, const simulation_log_t *log2);

// задаем временные промежутки
time_range_t arrival_time_of_type_1 = { .min_time = 1, .max_time = 5 };  // время прибытия для 1 типа
time_range_t arrival_time_of_type_2 = { .min_time = 0, .max_time = 3 };  // время прибытия для 2 типа
time_range_t service_time_of_type_1 = { .min_time = 0, .max_time = 4 };  // время обслуживания для 1 типа
time_range_t service_time_of_type_2 = { .min_time = 0, .max_time = 1 };  // время обслуживания для 2 типа

status_t simulate_service_unit_by_arr(bool verbose_mode)
{
    status_t ec = SUCCESS_CODE;

    // инициализируем очереди и их логи
    arr_queue_t queue1 = { 0 }, queue2 = { 0 };
    simulation_log_t log1 = { 0 }, log2 = { 0 };
    size_t last_print_checkpoint = 0;

    double current_time = 0.0;  // "виртуальное" (модельное) время
    double next_event_time = 0.0;  // время следующего события
    double current_service_end_time = INFINITY;  // время, когда закончится текущее обслуживание

    // временные переменные
    double temp_time = 0.0;
    double system_downtime = 0.0;  // время простоя системы
    double next_arrival_type1 = 0.0, next_arrival_type2 = 0.0;
    random_double(arrival_time_of_type_1.min_time, arrival_time_of_type_1.max_time, &next_arrival_type1);
    random_double(arrival_time_of_type_2.min_time, arrival_time_of_type_2.max_time, &next_arrival_type2);

    request_t current_request = { 0 };  // текущая заявка
    request_t popped_request = { 0 };  // заявка, которая будет удаляться
    request_class_t last_served_type = TYPE_1;  // тип последней обслуживаемой заявки

    if (verbose_mode)
        print_interim_results_table_header();  // печатаем заголовок таблицы и понеслась

    while (ec == SUCCESS_CODE && log1.request_out_count < REQUESTS_1_TYPE_QUEUE_LENGTH)
    {
        // определяем ближайшее событие
        min3(next_arrival_type1, next_arrival_type2, current_service_end_time, &next_event_time);
        current_time = next_event_time;

        // если появилась заявка типа 1
        if (fabs(next_arrival_type1 - next_event_time) < EPS)
        {
            current_request.request_class = TYPE_1;
            current_request.arrival_time = current_time;

            if (queue1.size < MAX_QUEUE_SIZE)
                push_arr(&queue1, &current_request);
            else 
            {
                ec = ERR_OVERFLOW;
                break;
            }

            if (current_service_end_time == INFINITY)
            {
                random_double(service_time_of_type_1.min_time, service_time_of_type_1.max_time, &temp_time);
                current_service_end_time = current_time + temp_time;
            }
            
            log1.request_in_count++;
            random_double(arrival_time_of_type_1.min_time, arrival_time_of_type_1.max_time, &temp_time);
            next_arrival_type1 += temp_time;
        }

        // если появилась заявка 2
        else if (fabs(next_arrival_type2 - next_event_time) < EPS)
        {
            current_request.request_class = TYPE_2;
            current_request.arrival_time = current_time;

            if (queue2.size < MAX_QUEUE_SIZE)
                push_arr(&queue2, &current_request);
            else 
            {
                ec = ERR_OVERFLOW;
                break;
            }

            if (current_service_end_time == INFINITY)
            {
                random_double(service_time_of_type_2.min_time, service_time_of_type_2.max_time, &temp_time);
                current_service_end_time = current_time + temp_time;
            }
            
            log2.request_in_count++;
            random_double(arrival_time_of_type_2.min_time, arrival_time_of_type_2.max_time, &temp_time);
            next_arrival_type2 += temp_time;
        }

        // если завершилось обслуживание (пытаемся обработать как можно скорее заявки первого типа)
        else if (fabs(current_service_end_time - next_event_time) < EPS)
        {
            if (queue1.size && (queue2.size == 0 || last_served_type == TYPE_1))
            {
                pop_arr(&queue1, &popped_request);
                log1.request_out_count++;
                log1.total_wait_time += current_time - popped_request.arrival_time;
                log1.total_length += queue1.size;
                random_double(service_time_of_type_1.min_time, service_time_of_type_1.max_time, &temp_time);
                current_service_end_time = current_time + temp_time;
                last_served_type = TYPE_1;
            }
            else if (queue2.size && (queue1.size == 0 || last_served_type == TYPE_2))
            {
                pop_arr(&queue2, &popped_request);
                log2.request_out_count++;
                log2.total_wait_time += current_time - popped_request.arrival_time;
                log2.total_length += queue2.size;
                random_double(service_time_of_type_2.min_time, service_time_of_type_2.max_time, &temp_time);
                current_service_end_time = current_time + temp_time;
                last_served_type = TYPE_2;
            }
            else if (queue1.size == 0 && queue2.size == 0)
            {
                system_downtime += (fmin(next_arrival_type1, next_arrival_type2) - current_time);
                current_service_end_time = INFINITY;
            }

            log1.function_call_count++;
            log2.function_call_count++;
        }

        // каждые 100 записей печатаем промежуточную информацию
        if (verbose_mode && log1.request_out_count % 100 == 0 && last_print_checkpoint != log1.request_out_count)
        {
            print_interim_results_table_content_arr(&queue1, &log1, &queue2, &log2);
            last_print_checkpoint = log1.request_out_count;
        }
    }

    if (ec == SUCCESS_CODE && verbose_mode)
    {
        print_interim_results_table_bottom();
        print_simulation_summary(current_time, &log1, &log2, system_downtime);
    }
    
    return ec;
}

status_t simulate_service_unit_by_list(bool verbose_mode, size_t *max_total_len_two_queues)
{
    status_t ec = SUCCESS_CODE;

    // инициализируем очереди и их логи
    list_queue_t queue1 = { 0, 0, 0 }, queue2 = { 0, 0, 0 };
    simulation_log_t log1 = { 0 }, log2 = { 0 };
    size_t last_print_checkpoint = 0;

    double current_time = 0.0;  // "виртуальное" (модельное) время
    double next_event_time = 0.0;  // время следующего события
    double current_service_end_time = INFINITY;  // время, когда закончится текущее обслуживание

    // заводим переменные для отлова максимальных длин очередей
    size_t max_length_queue_1 = 0;
    size_t max_length_queue_2 = 0;

    // временные переменные
    double temp_time = 0.0;
    double system_downtime = 0.0;
    double next_arrival_type1 = 0.0, next_arrival_type2 = 0.0;
    random_double(arrival_time_of_type_1.min_time, arrival_time_of_type_1.max_time, &next_arrival_type1);
    random_double(arrival_time_of_type_2.min_time, arrival_time_of_type_2.max_time, &next_arrival_type2);

    request_t current_request = { 0 };
    request_t popped_request = { 0 };
    request_class_t last_served_type = TYPE_1;

    if (verbose_mode)
        print_interim_results_table_header();

    while (ec == SUCCESS_CODE && log1.request_out_count < REQUESTS_1_TYPE_QUEUE_LENGTH)
    {
        min3(next_arrival_type1, next_arrival_type2, current_service_end_time, &next_event_time);
        current_time = next_event_time;

        // если появилась заявка типа 1
        if (fabs(next_arrival_type1 - next_event_time) < EPS)
        {
            current_request.request_class = TYPE_1;
            current_request.arrival_time = current_time;

            push_list(&queue1, &current_request);
            if (queue1.curr_size > max_length_queue_1)
                max_length_queue_1 = queue1.curr_size;

            if (current_service_end_time == INFINITY)
            {
                random_double(service_time_of_type_1.min_time, service_time_of_type_1.max_time, &temp_time);
                current_service_end_time = current_time + temp_time;
            }
            log1.request_in_count++;
            random_double(arrival_time_of_type_1.min_time, arrival_time_of_type_1.max_time, &temp_time);
            next_arrival_type1 += temp_time;
        }
        // если появилась заявка типа 2
        else if (fabs(next_arrival_type2 - next_event_time) < EPS)
        {
            current_request.request_class = TYPE_2;
            current_request.arrival_time = current_time;

            push_list(&queue2, &current_request);
            if (queue2.curr_size > max_length_queue_2)
                max_length_queue_2 = queue2.curr_size;

            if (current_service_end_time == INFINITY)
            {
                random_double(service_time_of_type_2.min_time, service_time_of_type_2.max_time, &temp_time);
                current_service_end_time = current_time + temp_time;
            }
            log2.request_in_count++;
            random_double(arrival_time_of_type_2.min_time, arrival_time_of_type_2.max_time, &temp_time);
            next_arrival_type2 += temp_time;
        }
        // если завершилось обслуживание (пытаемся обработать как можно скорее заявки первого типа)
        else if (fabs(current_service_end_time - next_event_time) < EPS)
        {
            if (queue1.curr_size && (queue2.curr_size == 0 || last_served_type == TYPE_1))
            {
                pop_list(&queue1, &popped_request);
                log1.request_out_count++;
                log1.total_wait_time += current_time - popped_request.arrival_time;
                log1.total_length += queue1.curr_size;
                random_double(service_time_of_type_1.min_time, service_time_of_type_1.max_time, &temp_time);
                current_service_end_time = current_time + temp_time;
                last_served_type = TYPE_1;
            }
            else if (queue2.curr_size && (queue1.curr_size == 0 || last_served_type == TYPE_2))
            {
                pop_list(&queue2, &popped_request);
                log2.request_out_count++;
                log2.total_wait_time += current_time - popped_request.arrival_time;
                log2.total_length += queue2.curr_size;
                random_double(service_time_of_type_2.min_time, service_time_of_type_2.max_time, &temp_time);
                current_service_end_time = current_time + temp_time;
                last_served_type = TYPE_2;
            }
            else if (queue1.curr_size == 0 && queue2.curr_size == 0)
            {
                system_downtime += (fmin(next_arrival_type1, next_arrival_type2) - current_time);
                current_service_end_time = INFINITY;
            }
            log1.function_call_count++;
            log2.function_call_count++;
        }

        /* каждые 100 записей печатаем промежуточную информацию и проверяем
        соотношение поступлений и обработок, чтобы выйти заблаговременно*/
        if (verbose_mode && log1.request_out_count % 100 == 0 && last_print_checkpoint != log1.request_out_count)
        {
            print_interim_results_table_content_list(&queue1, &log1, &queue2, &log2);
            last_print_checkpoint = log1.request_out_count;
            if (queue1.curr_size / log1.request_out_count >= CRITICAL_RECORDS_ATTITUDE || queue2.curr_size / log2.request_out_count >= CRITICAL_RECORDS_ATTITUDE)
                ec = ERR_CRITICAL_RECORDS_ATTITUDE;
        }
    }

    if (ec == SUCCESS_CODE && verbose_mode)
    {
        print_interim_results_table_bottom();
        print_simulation_summary(current_time, &log1, &log2, system_downtime);
    }

    if (max_total_len_two_queues)
        *max_total_len_two_queues = max_length_queue_1 + max_length_queue_2;
    
    destroy_list_queue(&queue1);
    destroy_list_queue(&queue2);

    return ec;
}

status_t change_simulation_configurations(void)
{
    status_t ec = SUCCESS_CODE;
    config_menu_option_t cur_menu_option;

    print_simulation_configs(&arrival_time_of_type_1, &arrival_time_of_type_2, &service_time_of_type_1, &service_time_of_type_2);
    print_simulation_config_menu();
    ec = input_cur_menu_opt((int *)&cur_menu_option, CONFIG_MENU_OPTIONS_QUANTITY);

    if (ec == SUCCESS_CODE)
    {
        switch (cur_menu_option)
        {
            case GO_BACK:
                break;

            case CHANGE_ENTARANCE_T1:
                input_time_gap(&arrival_time_of_type_1.min_time, &arrival_time_of_type_1.max_time);
                break;

            case CHANGE_ENTARANCE_T2:
                input_time_gap(&arrival_time_of_type_2.min_time, &arrival_time_of_type_2.max_time);
                break;

            case CHANGE_PROCESSING_T1:
                input_time_gap(&service_time_of_type_1.min_time, &service_time_of_type_1.max_time);
                break;

            case CHANGE_PROCESSING_T2:
                input_time_gap(&service_time_of_type_2.min_time, &service_time_of_type_2.max_time);
                break;
            
            default:
                ec = ERR_IO;
                break;
        }
    }

    return ec;
}

status_t min3(double a, double b, double c, double *min_val)
{
    if (min_val == NULL)
        return ERR_INVALID_POINTER;

    *min_val = a;
    if (b < *min_val) *min_val = b;
    if (c < *min_val) *min_val = c;

    return SUCCESS_CODE;
}

status_t random_double(double min, double max, double *rand_result)
{
    if (min >= max)
        return ERR_RANGE;
    if (rand_result == NULL)
        return ERR_INVALID_POINTER;

    *rand_result = min + (max - min) * (rand() / (double)RAND_MAX);   
    return SUCCESS_CODE; 
}

status_t print_interim_results_table_header(void)
{
    printf("%s", BLUE);
    printf("╔════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                                                     INTERIM RESULTS                                                    ║\n");
    printf("╠════════════════════════════╦════════════╦═════════════╦══════════════╦═══════════════╦════════════════╦════════════════╣\n");
    printf("║ requests (type1) processed ║ queue name ║ requests in ║ requests out ║ requests lost ║ current length ║ average length ║\n");
    printf("%s", RESET);

    return SUCCESS_CODE;
}

status_t print_interim_results_table_content_arr(const arr_queue_t *queue1, const simulation_log_t *log1, const arr_queue_t *queue2, const simulation_log_t *log2) 
{
    if (queue1 == NULL || log1 == NULL || queue2 == NULL || log2 == NULL)
        return ERR_INVALID_POINTER;

    printf("%s", BLUE);
    printf("╠════════════════════════════╬════════════╬═════════════╬══════════════╬═══════════════╬════════════════╬════════════════╣\n");
    printf("║ %-26lu ║ %-10s ║ %-11lu ║ %-12lu ║ %-13lu ║ %-14lu ║ %-14.2lf ║\n", log1->request_out_count, "queue_1", log1->request_in_count, log1->request_out_count, log1->failed_request_count, queue1->size, log1->request_out_count > 0 ? log1->total_length / log1->request_out_count : 0.0);
    printf("╠════════════════════════════╬════════════╬═════════════╬══════════════╬═══════════════╬════════════════╬════════════════╣\n");
    printf("║ %-26lu ║ %-10s ║ %-11lu ║ %-12lu ║ %-13lu ║ %-14lu ║ %-14.2lf ║\n", log1->request_out_count, "queue_2", log2->request_in_count, log2->request_out_count, log2->failed_request_count, queue2->size, log2->request_out_count > 0 ? log2->total_length / log2->request_out_count : 0.0);
    printf("%s", RESET);
    
    return SUCCESS_CODE;
}

status_t print_interim_results_table_content_list(const list_queue_t *queue1, const simulation_log_t *log1, const list_queue_t *queue2, const simulation_log_t *log2) 
{
    if (queue1 == NULL || log1 == NULL || queue2 == NULL || log2 == NULL)
        return ERR_INVALID_POINTER;

    printf("%s", BLUE);
    printf("╠════════════════════════════╬════════════╬═════════════╬══════════════╬═══════════════╬════════════════╬════════════════╣\n");
    printf("║ %-26lu ║ %-10s ║ %-11lu ║ %-12lu ║ %-13lu ║ %-14lu ║ %-14.2lf ║\n", log1->request_out_count, "queue_1", log1->request_in_count, log1->request_out_count, log1->failed_request_count, queue1->curr_size, log1->request_out_count > 0 ? (double)log1->total_length / log1->request_out_count : 0.0);
    printf("╠════════════════════════════╬════════════╬═════════════╬══════════════╬═══════════════╬════════════════╬════════════════╣\n");
    printf("║ %-26lu ║ %-10s ║ %-11lu ║ %-12lu ║ %-13lu ║ %-14lu ║ %-14.2lf ║\n", log1->request_out_count, "queue_2", log2->request_in_count, log2->request_out_count, log2->failed_request_count, queue2->curr_size, log2->request_out_count > 0 ? (double)log2->total_length / log2->request_out_count : 0.0);
    printf("%s", RESET);
    
    return SUCCESS_CODE;
}


status_t print_interim_results_table_bottom(void)
{
    printf("%s", BLUE);
    printf("╚════════════════════════════╩════════════╩═════════════╩══════════════╩═══════════════╩════════════════╩════════════════╝\n");
    printf("%s", RESET);

    return SUCCESS_CODE;
}

status_t print_simulation_summary(double total_time, const simulation_log_t *log1, const simulation_log_t *log2, double downtime) 
{
    if (log1 == NULL || log2 == NULL)
        return ERR_INVALID_POINTER;

    printf("%s", BLUE_BOLD);
    printf("╔════════════════════════════════════════════╗\n");
    printf("║              SIMULATION SUMMARY            ║\n");
    printf("╠═══════════════╦═════════════╦══════════════╣\n");
    printf("║ requests type ║ requests in ║ requests out ║\n");
    printf("╠═══════════════╬═════════════╬══════════════╣\n");
    printf("║ queue type 1  ║ %-11lu ║ %-12lu ║\n", log1->request_in_count, log1->request_out_count);
    printf("╠═══════════════╬═════════════╬══════════════╣\n");
    printf("║ queue type 2  ║ %-11lu ║ %-12lu ║\n", log2->request_in_count, log2->request_out_count);
    printf("╠╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╬╦╦╦╦╦╦╦╦╦╦╦╦╦╬╦╦╦╦╦╦╦╦╦╦╦╦╦╦╣\n");
    printf("║║║║║║║║║║║║║║║║║║║║║║║║║║║║║║║║║║║║║║║║║║║║║║\n");
    printf("╠╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╬╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╣\n");
    printf("║ total simulation time ║ %-18.2lf ║\n", total_time);
    printf("╠═══════════════════════╬════════════════════╣\n");
    printf("║    system dowmtime    ║ %-18.2lf ║\n", downtime);
    printf("╚═══════════════════════╩════════════════════╝\n");
    printf("\n");
    printf("%s", RESET);

    return SUCCESS_CODE;
}