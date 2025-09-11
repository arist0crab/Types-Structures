#include "defines.h"
#include "input.h"
#include "process.h"
#include "output.h"


int main(void)
{
    exit_status status = SUCCESS_CODE;

    lfloat_t first_num;
    lfloat_t second_num;
    lfloat_t result_num;

    print_instructions();
    input_two_lfloats(&first_num, &second_num, &status);
    lfloat_multiply(&first_num, &second_num, &result_num, &status);
    print_parsed_number(&result_num);

    return status;
}