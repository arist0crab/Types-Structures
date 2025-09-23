#include "process.h"

/**
 * @brief Сдвигает элементы массива влево на указанное количество позиций
 * 
 * Функция перемещает элементы массива влево, заполняя освободившуюся 
 * правую часть нулями. Эквивалентно удалению первых `shift` элементов
 * и сдвигу оставшихся в начало массива
 * 
 * @param[in,out] arr Указатель на массив для сдвига
 * @param[in] size Размер массива
 * @param[in] shift Количество позиций для сдвига влево
 * 
 */
static void shift_array_left(int *arr, size_t size, size_t shift)
{
    if (shift != 0)
    {
        // Копируем элементы со сдвигом
        memmove(arr, arr + shift, (size - shift) * sizeof(int));
        // Обнуляем хвост
        memset(arr + size - shift, 0, shift * sizeof(int));
    }
}

/**
* @brief Умножает два длинных вещественных числа
* 
* Функция выполняет умножение двух чисел в формате lfloat_t, обрабатывает
* мантиссы и порядки, выполняет нормализацию результата и округление
* при превышении максимальной длины мантиссы
* 
* @param[in] first_lfloat Указатель на первый множитель
* @param[in] second_lfloat Указатель на второй множитель  
* @param[out] result_lfloat Указатель на результат умножения
* @param[out] status Статус выполнения операции
* 
*/
void lfloat_multiply(lfloat_t *first_lfloat, lfloat_t *second_lfloat, lfloat_t *result_lfloat, exit_status *status)
{
    int arr[ARR_SIZE] = {0};  // Массив для хранения промежуточных результатов умножения

    // Проходим по каждой цифре первого числа, начиная с младших разрядов.
    for (size_t i = 0; i < (size_t)(first_lfloat->mant_size); i++) 
    {
        for (size_t j = 0; j < (size_t)(second_lfloat->mant_size); j++) 
        
        {
            size_t k = i + j;
            arr[k] += first_lfloat->mantiss[i] * second_lfloat->mantiss[j];
            // Если текущий элемент временного массива превышает BASE, переносим в следующий элемент
            while (arr[k] >= BASE && k < ARR_SIZE - 1) 
            {
                arr[k + 1] += arr[k] / BASE;
                arr[k] %= BASE;
                k++;
            }
        }
    }
    
    // Фактическое количество значащих цифр в результате умножения 
    size_t result_size = 0;
    for (size_t i = ARR_SIZE; i > 0; i--)
        if (arr[i - 1] != 0) 
        {
            result_size = i; 
            break;
        }

    if (result_size > MAX_MANTISS_LENGTH)
    {
        size_t diff_size = result_size - MAX_MANTISS_LENGTH;
        // Проверка отбрасываемых цифр и округление
        for (size_t i = 0; i < diff_size; i++)
            if (arr[i] >= 5)
                arr[i + 1]++;

        // Обработка переносов, которые могли возникнуть при округлении
        for (size_t i = diff_size; i < result_size && arr[i] > 9; i++)
        {
            arr[i] = 0;
            arr[i + 1]++;
        }

        // Поиск первой ненулевой цифры во временном массиве arr
        for (size_t i = diff_size; arr[i] == 0 && i < result_size; i++)
            diff_size++;

        shift_array_left(arr, result_size, diff_size);
        result_size -= diff_size;
    }

    // Записываем результат в структуру
    result_lfloat->mant_size = result_size;
    result_lfloat->mant_sign = (first_lfloat->mant_sign == second_lfloat->mant_sign) ? 1 : 0;

    // Определяем порядок результата основываясь на длине мантисс
    if (first_lfloat->mant_size == 0 || second_lfloat->mant_size == 0)
        result_lfloat->order = 0;
    else
    {
        int order_correction = result_size - first_lfloat->mant_size - second_lfloat->mant_size;
        result_lfloat->order = first_lfloat->order + second_lfloat->order + order_correction;
        result_lfloat->order += first_lfloat->raw_order + second_lfloat->raw_order;
        printf("order correction: %d\n", order_correction);
        printf("result_size %ld\n", result_size);
        printf("raw orders sum %d\n", first_lfloat->raw_order + second_lfloat->raw_order);
    }

    if (result_lfloat->order > MAX_ORDER_VAL || result_lfloat->order < MIN_ORDER_VAL)
        *status = WRONG_ORDER_VALUE;
    else 
        for (size_t i = 0; i < result_size; i++)
            result_lfloat->mantiss[i] = arr[i];
} 