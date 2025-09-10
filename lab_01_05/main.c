#include "defines.h"
#include "input.h"


int main(void)
{
    exit_status status = SUCCESS_CODE;

    lfloat_t first_num;
    // lfloat_t second_num;
    // lfloat_t result_num;

    print_instructions();
    input_lfloat(&first_num, MAX_MANTISS_LENGTH, &status);

    if (status == SUCCESS_CODE)
    {
        printf("====== PARSE RESULT ======\n");
        printf("--> mantiss: ");
        for (int i = 0; i < first_num.mant_size; i++)
            printf("%d ", first_num.mantiss[i]);
        printf("\n");
        printf("--> mantiss size: %d\n", first_num.mant_size);
        printf("--> mantiss_sign: %d\n", first_num.mant_sign);
        printf("--> order: %d\n", first_num.order);
    }
    else 
        printf("Ошебка соре((((((\n");

    return status;
}