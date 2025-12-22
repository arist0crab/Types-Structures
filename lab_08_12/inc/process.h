#ifndef __PROCESS_H__
#define __PROCESS_H__

#include "data.h"
#include "graph.h"
#include "input.h"
#include "output.h"
#include "auxilary.h"

/** @brief Функция для обработки выбранного пункта меню.
 * @param[in] menu_option Выбранная опция меню.
 * @param[in] graph Указатель на исходный граф.
 * @return Статус выполнения.
*/
status_t procces_menu_choice(menu_option_t menu_option, graph_t *graph);

#endif