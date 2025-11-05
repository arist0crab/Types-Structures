#ifndef __SIMULATE_H__
#define __SIMULATE_H__

#include "data.h"
#include "arr.h"
#include "color.h"
#include "output.h"
#include "list.h"
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#define REQUESTS_1_TYPE_QUEUE_LENGTH 1000

extern time_range_t arrival_time_of_type_1;
extern time_range_t arrival_time_of_type_2;
extern time_range_t service_time_of_type_1;
extern time_range_t service_time_of_type_2;

typedef struct
{
    size_t request_in_count;  // кол-во заявок, которые вошли в систему
    size_t request_out_count;  // кол-во заявок, которые вышли из системы
    size_t failed_request_count;  // кол-во потерянных заявок
    size_t function_call_count;  // кол-во обращений к ОА
    double total_length;  // суммарная длина очереди на момент извлечения каждой заявки
    double total_wait_time;  // суммарное время ожидания всех заявок в очереди
} simulation_log_t;


/** @brief Функция для симуляции работы системы (обработки n-time раз записей 1-ого типа) на 
 * основе работы со статическим массивом для хранения очередей.
 * @return Статус выполнения функции.
*/
status_t simulate_service_unit_by_arr(void);

// TODO написать документацию по рофлу
status_t simulate_service_unit_by_list(void);

#endif