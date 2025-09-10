#include "output.h"


/**
 * @brief Печать структуры 'большого числа'
 * 
 * Позволяет вывести информацию о полях структуры в читаемом виде в консоль.
 * 
 * @param *lfloat_number Указатель на структуру, информацию о которой необходимо вывести
*/
void print_parsed_number(lfloat_t *lfloat_number)
{
    printf("====== PARSE RESULT ======\n");
    printf("--> mantiss: ");
    for (int i = 0; i < lfloat_number->mant_size; i++)
        printf("%d ", lfloat_number->mantiss[i]);
    printf("\n");
    printf("--> mantiss size: %d\n", lfloat_number->mant_size);
    printf("--> mantiss_sign: %d\n", lfloat_number->mant_sign);
    printf("--> order: %d\n\n", lfloat_number->order);
}