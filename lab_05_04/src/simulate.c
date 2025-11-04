#include "simulate.h"

status_t print_simulation_summary(double total_time, const simulation_log_t *log1, const simulation_log_t *log2, double downtime);
status_t print_queue_lengths(const size_t records_processed_quantity, const arr_queue_t *queue1, const simulation_log_t *log1, const arr_queue_t *queue2, const simulation_log_t *log2);
status_t random_double(double min, double max, double *rand_result);
status_t min3(double a, double b, double c, double *min_val);

// задаем временные промежутки
time_range_t arrival_time_of_type_1 = { .min_time = 1, .max_time = 5 };  // время прибытия для 1 типа
time_range_t arrival_time_of_type_2 = { .min_time = 0, .max_time = 3 };  // время прибытия для 2 типа
time_range_t service_time_of_type_1 = { .min_time = 0, .max_time = 4 };  // время обслуживания для 1 типа
time_range_t service_time_of_type_2 = { .min_time = 0, .max_time = 1 };  // время обслуживания для 2 типа

status_t simulate_service_unit_by_arr(void)
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
                log1.failed_request_count++;

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
                log2.failed_request_count++;

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
        else if (fabs(current_service_end_time - next_event_time) < EPS || (queue1.size || queue2.size))
        {
            if (queue1.size && (queue2.size == 0 || last_served_type == TYPE_1))
            {
                pop_arr(&queue1, &popped_request);
                log1.request_out_count++;
                log1.total_wait_time += current_time - popped_request.arrival_time;
                log1.total_length += queue1.size;
                random_double(service_time_of_type_1.min_time, service_time_of_type_1.max_time, &temp_time);
                last_served_type = TYPE_1;
            }
            else if (queue2.size && (queue1.size == 0 || last_served_type == TYPE_2))
            {
                pop_arr(&queue2, &popped_request);
                log2.request_out_count++;
                log2.total_wait_time += current_time - popped_request.arrival_time;
                log2.total_length += queue2.size;
                random_double(service_time_of_type_2.min_time, service_time_of_type_2.max_time, &temp_time);
                last_served_type = TYPE_2;
            }
            else if (queue1.size == 0 && queue2.size == 0)
            {
                system_downtime += (fmin(next_arrival_type1, next_arrival_type2) - current_time);
                current_service_end_time = INFINITY;
            }

            log1.function_call_count++;
            log2.function_call_count++;
            // назначаем новый момент окончания обслуживания
            current_service_end_time = current_time + temp_time; 
        }

        // каждые 100 записей печатаем промежуточную информацию
        if (log1.request_out_count % 100 == 0 && last_print_checkpoint != log1.request_out_count)
        {
            print_queue_lengths(log1.request_out_count, &queue1, &log1, &queue2, &log2);
            last_print_checkpoint = log1.request_out_count;
        }
    }

    print_simulation_summary(current_time, &log1, &log2, system_downtime);
    
    return SUCCESS_CODE;
}

status_t print_queue_lengths(const size_t records_processed_quantity, const arr_queue_t *queue1, const simulation_log_t *log1, const arr_queue_t *queue2, const simulation_log_t *log2) 
{
    if (queue1 == NULL || log1 == NULL || queue2 == NULL || log2 == NULL)
        return ERR_INVALID_POINTER;

    printf("%s", BLUE);
    printf("=============== ОБРАБОТАНО %lu ЗАПИСЕЙ ===============\n", records_processed_quantity);
    printf("Очередь 1: текущая длина = %zu, средняя длина = %.2f\n", queue1->size, log1->request_out_count > 0 ? log1->total_length / log1->request_out_count : 0.0);
    printf("Очередь 2: текущая длина = %zu, средняя длина = %.2f\n", queue2->size, log2->request_out_count > 0 ? log2->total_length / log2->request_out_count : 0.0);
    printf("\n");
    printf("%s", RESET);
    
    return SUCCESS_CODE;
}

status_t print_simulation_summary(double total_time, const simulation_log_t *log1, const simulation_log_t *log2, double downtime) 
{
    if (log1 == NULL || log2 == NULL)
        return ERR_INVALID_POINTER;

    printf("%s", BLUE_BOLD);
    printf("Общее время моделирования: %.2f\n", total_time);
    printf("-----------------------------\n");
    printf("Заявки типа 1: вошло: %zu, вышло: %zu\n", log1->request_in_count, log1->request_out_count);
    printf("Заявки типа 2: вошло: %zu, вышло: %zu\n", log2->request_in_count, log2->request_out_count);
    printf("Время простоя аппарата: %lf\n", downtime);
    printf("\n");
    printf("%s", RESET);

    return SUCCESS_CODE;
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