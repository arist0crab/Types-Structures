#include "input.h"

void input_lfloat(lfloat_t *lfnum, size_t max_lfloat_len, exit_status *status);
void lffloat_string_parse(lfloat_t *lfnum, char *string, exit_status *status);


/**
 * @brief Функция одновременного ввода двух чисел.
 * 
 * Функция позволяет ввести два 'больших' числа, записав каждое в указанную
 * структуру. При возникновении каких-либо ошибок соответствующий глобальный 
 * аргумент status будет изменен.
 * 
 * @param[in] first_lfnum Указатель на первую структуру, в которую записываются данные
 * @param[in] second_lfnum Указатель на вторую структуру, в которую записываются данные
 * @param[out] status Указатель на статус выполнения программы
*/
void input_two_lfloats(lfloat_t *first_lfnum, lfloat_t *second_lfnum, exit_status *status)
{
    input_lfloat(first_lfnum, FIRST_MANTISS_LENGTH, status);
    if (*status == SUCCESS_CODE)
    {
        print_parsed_number(first_lfnum);
        input_lfloat(second_lfnum, SECOND_MANTISS_LENGTH, status);
        if (*status == SUCCESS_CODE)
            print_parsed_number(second_lfnum);
    }
}


/**
 * @brief Информация
 * 
 * Функция для вывода информации о приложении и инструкций по его эксплуатации
*/
void print_instructions(void)
{
    printf("Программа для умножения двух действительных чисел.\n");
    printf("%sФормат ввода: [+-]?[0-9]*[.]?[0-9]*E[+-][0-9]*%s\n", BOLD, RESET);
    printf("Примечания:\n");
    printf("1. Знак перед мантиссой и порядком является необязательным.\n");
    printf("2. Точка может быть до числа (.55 => 0.55), может быть после (55. => 55.0).\n");
    printf("3. В числе должна присутствовать хотя бы одна цифра (до или после точки).\n");
    printf("4. Если при вводе используется 'E', то после него обязательно должна быть хотя бы одна цифра.\n");
    printf("5. Согласно условию длина 1-ого множителя до 35-ти значащих цифр, 2-ого - до 40, длина результата - до 40. Величина порядка 5 цифр.\n");
    printf("\n");
}


/**
 * @brief Ввод числа
 * 
 * Функция для ввода числа в необходимом формате из командной строки и ее парсинга, т.е.
 * записи в структуру lfnum. 
 * 
 * @param[out] lfnum Указатель на структуру, в которую записываются данные
 * @param[in] max_lfloat_len Максимальная кол-во значащих цифр в вводимом числе
 * @param[out] status Указатель на статус выполнения программы
*/

void input_lfloat(lfloat_t *lfnum, size_t max_lfloat_len, exit_status *status)
{
    char input_buffer[MAX_BUFFER_LENGTH];

    printf("%sВвод: ", BLUE);
    fgets(input_buffer, MAX_BUFFER_LENGTH, stdin);
    printf("%s\n", RESET);

    size_t cur_len = strlen(input_buffer);

    if (cur_len && input_buffer[cur_len - 1] == '\n')
        input_buffer[--cur_len] = '\0';

    if (!cur_len || cur_len > max_lfloat_len)
        *status = INCORRECT_NUM_LENGTH;

    lffloat_string_parse(lfnum, input_buffer, status);
}

/**
 * @brief Функция для парсинга числа из строки.
 * 
 * Парсит строку и вносит полученные из нее данные в указанную структуру.
 * Если при парсинге возникнут ошибки, будет изменен status (статус выполнения программы).
 * 
 * @param[out] lfnum Указатель на структуру, в которую будут внесены данные
 * @param[in] string Строка, которую необходимо распарсить
 * @param[out] status Глобальный статус выполнения программы
*/
void lffloat_string_parse(lfloat_t *lfnum, char *string, exit_status *status)
{
    const char *ptr = string; // Указатель для движения по строке
    bool order_sign = true;  // Знаки: true -> '+', false -> '-'
    lfnum->mant_sign = true;
    int lead_zrs_before_point = 0;  // Нули до точки
    int lead_zrs_after_point = 0;  // Нули после точки
    bool point_encountered = false;  // Флаг встречи точки

    // NOTE === ОБРАБОТКА ЗНАКА МАНТИССЫ И ВЕДУЩИХ НУЛЕЙ ===
    if (*ptr == '+')
        ptr++;
    else if (*ptr == '-')
    {
        ptr++;
        lfnum->mant_sign = false;
    }

    while ((*ptr == '0' || *ptr == '.') && *status == SUCCESS_CODE)
    {
        if (*ptr == '0')
            (point_encountered) ? lead_zrs_after_point++ : lead_zrs_before_point++;
        else 
        {
            if (!point_encountered)
                point_encountered = true;
            else 
                *status = INVALID_CHARACTER;
        }

        ptr++;         
    }       

    // NOTE === ПОИСК КОНЦА МАНТИССЫ И ПОДСЧЕТ ЕЕ ДЛИНЫ ===
    const char *mantissa_start = ptr;  // Начало значащей части мантиссы
    const char *mantissa_end = ptr - 1;  // Конец значащей части мантиссы
    int mantissa_length = 0;
    int digits_before_point = 0;  // Цифры до точки в значащей части (если есть)

    while (*ptr != '\0' && *ptr != 'E' && *status == SUCCESS_CODE)
    {
        if (*ptr >= '0' && *ptr <= '9')
        {
            if (!point_encountered)
                digits_before_point++;
            
            mantissa_end = ptr;
            mantissa_length++;
        }
        else if (*ptr == '.' && !point_encountered)
            point_encountered = true;
        else if (*ptr != ' ')
            *status = INVALID_CHARACTER;
        ptr++;
    }

    // NOTE === ПРОВЕРКА ОГРАНИЧЕНИЙ РАЗМЕРА МАНТИССЫ ===
    if (*status == SUCCESS_CODE && mantissa_length > MAX_MANTISS_LENGTH)
        *status = ERR_MANTISS_SIZE;
    
    // NOTE === ЗАПОЛЕНИЕ МАССИВА МАНТИССЫ ===
    if (*status == SUCCESS_CODE)
    {
        int write_index = mantissa_length - 1; // Записываем с конца массива
        point_encountered = 0; // Сбрасываем для второго прохода

        for (const char *curr = mantissa_start; curr <= mantissa_end; curr++) 
        {
            if (*curr == '.')
                point_encountered = 1;
            else 
                lfnum->mantiss[write_index--] = *curr - '0';  // '5' - '0' => 53 - 48 = 5
        }
        lfnum->mant_size = mantissa_length;

        // NOTE === ОБРАБОТКА ПОРЯДКА ====
        int order_value = 0;
        int order_digits_count = 0;

        if (*ptr != 'E')
        {
            if (mantissa_length > 0)
            {
                if (digits_before_point > 0)
                    lfnum->order = digits_before_point;
                else
                    lfnum->order = -lead_zrs_after_point - mantissa_length;
            }
            else
                lfnum->order = 0;
        }
        else
        {
            while (*(++ptr) == ' ');

            if (*ptr == '-')
            {
                ptr++;
                order_sign = false;
            }
            else if (*ptr == '+')
                ptr++;
            
            // Пропуск ведущих нулей
            while (*ptr == '0')
                ptr++;

            // Чтение значения порядка
            const char *order_start = ptr;
            while (*ptr >= '0' && *ptr <= '9')
            {
                order_value = order_value * 10 + (*ptr - '0');
                order_digits_count++;
                ptr++;
            }

            if (order_digits_count == 0 && ptr == order_start)
                *status = NO_NUM_AFTER_E;  // Если после 'E' нет цифр
            else if (order_digits_count > MAX_ORDER_LENGTH)
                *status = ERR_ORDER_SIZE;  // Если порядок больше дозволенного
            
            // NOTE === ВЫЧИСЛЕНИЕ ИТОГОВОГО ПОРЯДКА ===
            if (*status == SUCCESS_CODE)
            {
                order_value = (order_sign) ? order_value : -order_value;  // Если знак порядка отрицательный, меняем значение 
                lfnum->order = (digits_before_point) ? digits_before_point + order_value : order_value - lead_zrs_after_point - mantissa_length;
            }
        }
    }
}