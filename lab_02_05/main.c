#include "status.h"
#include "data.h"
#include "color.h"
#include "input.h"
#include "process.h"
#include "print_instructions.h"
#include <stdbool.h>

static theater_play_t *theater_plays_arr = NULL;
static int *theater_plays_keys = NULL;
static size_t theater_plays_q = 0;

int main(void)
{
    status_t exit_code = SUCCCESS_CODE;
    bool program_running = false;
    choice_t current_choice;

    exit_code = allocate_memory((void **)&theater_plays_arr, MAX_PLAYS_QUANTITY, sizeof(theater_play_t));
    if (exit_code == SUCCCESS_CODE)
        exit_code = allocate_memory((void **)&theater_plays_keys, MAX_PLAYS_QUANTITY, sizeof(int));

    if (exit_code == SUCCCESS_CODE)
        program_running = true;    

    while (program_running && exit_code == SUCCCESS_CODE)
    {
        exit_code = print_menu();
        exit_code = input_choice(&current_choice);
        if (exit_code == SUCCCESS_CODE)
            process_choice(current_choice, &program_running, theater_plays_arr, theater_plays_keys, &theater_plays_q);
    }

    // освобождаем выделенную память
    if (theater_plays_arr)
        free(theater_plays_arr);

    return exit_code;
}