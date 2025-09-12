#include "output.h"

/**
 * @brief Вывод информации о выполнении программы
 * 
 * @param[in] num Результирующее число типа lfloat_t
 * @param[in] Статус выполнения программы
*/
void print_result(const lfloat_t *num, exit_status *status)
{
    switch (*status)
    {
    case SUCCESS_CODE:
        print_normalized_lfloat(num);
        break;
    case INVALID_CHARACTER:
        printf("%sНеверный символ, пожалуйста, проверьте ввод%s\n\n", RED_BOLD, RESET);
        break;
    case ERR_MANTISS_SIZE:
        printf("%sПревышен размер мантиссы, пожалуйста, проверьте ввод%s\n\n", RED_BOLD, RESET);
        break;
    case ERR_ORDER_SIZE:
        printf("%sПревышен размер порядка, пожалуйста, проверьте ввод%s\n\n", RED_BOLD, RESET);
        break;
    case NO_NUM_AFTER_E:
        printf("%sОтстутствует число после 'E', пожалуйста, проверьте ввод%s\n\n", RED_BOLD, RESET);
        break;
    case INCORRECT_NUM_LENGTH:
        printf("%sНекорректная длина числа, пожалуйста, проверьте ввод%s\n\n", RED_BOLD, RESET);
        break;
    case WRONG_ORDER_VALUE:
        printf("%sНекорректное значение порядка, пожалуйста, проверьте ввод%s\n\n", RED_BOLD, RESET);
        break;
    }
}

/**
 * @brief Вывод числа в нормализованном формате ±0.m1 Е ±K1
 * 
 * @param[in] num Указатель на структуру lfloat_t для вывода
 */
void print_normalized_lfloat(const lfloat_t *num)
{
    printf("%sРезультат: %s", BLUE_BOLD, num->mant_sign ? "+" : "-");
    printf("0.");

    for (int i = num->mant_size - 1; i >= 0; i--)
        printf("%d", num->mantiss[i]);

    printf(" Е %s%d%s\n\n", (num->order >= 0) ? "+" : "-", num->order, RESET);
}

/**
 * @brief Печать структуры 'большого числа'
 * 
 * Позволяет вывести информацию о полях структуры в читаемом виде в консоль.
 * 
 * @param[in] lfloat_number Указатель на структуру, информацию о которой необходимо вывести
*/
void print_parsed_number(lfloat_t *lfloat_number)
{
    printf("====== PARSE RESULT ======\n");
    printf("--> reversed_mantiss: ");
    for (int i = 0; i < lfloat_number->mant_size; i++)
        printf("%d ", lfloat_number->mantiss[i]);
    printf("\n");
    printf("--> mantiss_size: %d\n", lfloat_number->mant_size);
    printf("--> mantiss_sign: %d\n", lfloat_number->mant_sign);
    printf("--> order: %d\n\n", lfloat_number->order);
}