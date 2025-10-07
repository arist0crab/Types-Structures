#include "process.h"

status_t clean_arr(theater_play_t *theater_plays_arr, size_t *theater_plays_q);
status_t add_play(theater_play_t *theater_plays_arr, int *theater_plays_keys, size_t *theater_plays_q);
status_t input_string_to_delete(char *target_string);
status_t shift_array_with_delete_one_string_elem(theater_play_t *theater_plays_arr, size_t *theater_plays_q, size_t *current_pos, char *string_to_delete, char *source_string_to_compare);
status_t shift_array_with_delete_one_integer_elem(theater_play_t *theater_plays_arr, size_t *theater_plays_q, size_t *current_pos, int int_to_delete, int source_int_to_delete);
status_t delete_play(theater_play_t *theater_plays_arr, int *theater_plays_keys, size_t *theater_plays_q);

status_t allocate_memory(void **elems_arr, size_t elems_quantity, size_t elem_size)
{
    status_t rc = SUCCCESS_CODE;
    *elems_arr = NULL;

    if ((int)elems_quantity <= 0 || (int)elem_size <= 0)
        rc = INVALID_QUANTITY;

    if (rc == SUCCCESS_CODE)
    {
        *elems_arr = malloc(elems_quantity * elem_size);
        if (*elems_arr == NULL)
            rc = MEM_ALLOCATE_ERROR;
    }

    return rc;
}

status_t process_choice(choice_t choice, bool *program_running, theater_play_t *theater_plays_arr, int *theater_plays_keys, size_t *theater_plays_q)
{
    status_t rc = SUCCCESS_CODE;
    
    switch (choice)
    {
    case EXIT_THE_PROGRAM:
        *program_running = false;
        break;

    case READ_FILE:
        print_read_file_comments();
        rc = clean_arr(theater_plays_arr, theater_plays_q);
        if (rc == SUCCCESS_CODE)
            rc = read_file(theater_plays_arr, theater_plays_keys, theater_plays_q);
        if (rc == SUCCCESS_CODE)
            rc = print_theater_plays_table(theater_plays_arr, *theater_plays_q);
        break;

    case PRINT_DATA:
        rc = print_theater_plays_table(theater_plays_arr, *theater_plays_q);
        break;
    
    case PRINT_KEY_TABLE:
        rc = print_keys_table(theater_plays_arr, theater_plays_keys, *theater_plays_q);
        break;

    case ADD_THEATER_PLAY:
        rc = add_play(theater_plays_arr, theater_plays_keys, theater_plays_q);
        break;

    case DELETE_THEATER_PLAY:
        rc = delete_play(theater_plays_arr, theater_plays_keys, theater_plays_q);
        break;

    case PRICE_QUICK_SORT:
        rc = quick_sort_by_ticket_price(theater_plays_arr, 0, *theater_plays_q - 1);
        break;

    case PRICE_SLOW_SORT:
        rc = slow_sort_by_ticket_price(theater_plays_arr, theater_plays_q);
        break;

    case KEY_TABLE_QUICK_SORT:
        rc = quick_sort_by_keys(theater_plays_arr, theater_plays_keys, 0, *theater_plays_q - 1);
        break;

    case KEY_TABLE_SLOW_SORT:
        rc = slow_sort_by_keys(theater_plays_arr, theater_plays_keys, theater_plays_q);
        break;

    case PRINT_DATA_KEY_TABLE:
        break;

    case PRINT_EFFICIENCY_TABLE:
        break;

    case PRINT_BALETS:
        break;

    case CHOICES_QUANTITY:
        rc = INVALID_CHOICE;
        break;
    
    default:
        rc = INVALID_CHOICE;
        break;
    }

    if (rc == SUCCCESS_CODE)
        printf("%sSUCCESS%s\n\n", GREEN_BOLD, RESET);

    return rc;
}

status_t add_play(theater_play_t *theater_plays_arr, int *theater_plays_keys, size_t *theater_plays_q)
{
    status_t rc = SUCCCESS_CODE;
    theater_play_t new_play;
    int temp_int;

    if (*theater_plays_q >= MAX_PLAYS_QUANTITY)
        return RECORDS_ARR_OVERFLOWED;

    printf("%sВведите название театра: %s", BLUE, RESET);
    scanf("%s", new_play.theater_name);
    printf("%sВведите название спектакля: %s", BLUE, RESET);
    scanf("%s", new_play.play_name);
    printf("%sВведите цену билета: %s", BLUE, RESET);

    if (scanf("%lf", &new_play.ticket_price) != 1 || new_play.ticket_price <= 0.0)
        rc = INVALID_INPUT;
    
    if (rc == SUCCCESS_CODE)
    {
        printf("%sВведите максимальную цену билета: %s", BLUE, RESET);
        if (scanf("%lf", &new_play.max_ticket_price) != 1 || new_play.max_ticket_price <= 0.0)
            rc = INVALID_INPUT;
    }

    if (rc == SUCCCESS_CODE)
        if (new_play.ticket_price > new_play.max_ticket_price)
            rc = INVALID_INPUT;

    if (rc == SUCCCESS_CODE)
    {
        printf("%sВведите тип спектакля (целое число):\n0 - пьеса\n1- мюзикл\n%s", BLUE, RESET);
        if (scanf("%d", &temp_int) != 1 || temp_int < 0 || temp_int > 1)
            rc = INVALID_INPUT;
        else 
            new_play.play_type = (play_type_t)temp_int;
    }

    if (rc == SUCCCESS_CODE && new_play.play_type == PIECE)
    {
        printf("%sВведите тип пьесы (целое число):\n0 - драма\n1 - комедия\n2 - сказка\n%s", BLUE, RESET);
        if (scanf("%d", &temp_int) != 1 || temp_int < 0 || temp_int > 2)
            rc = INVALID_INPUT;
        else 
            new_play.play_data.piece_info.piece_genre = (piece_genre_t)temp_int;

        if (rc == SUCCCESS_CODE && new_play.play_data.piece_info.piece_genre == FAIRY_TALE)
        {
            printf("%sВведите возраст (целое число):\n0 - 3+\n1 - 10+\n2 - 16+\n%s", BLUE, RESET);
            if (scanf("%d", &temp_int) != 1 || temp_int < 0 || temp_int > 2)
                rc  = INVALID_INPUT;
            else 
                new_play.age_rating = (age_rating_t)temp_int;
        }

        if (rc == SUCCCESS_CODE && new_play.play_data.piece_info.piece_genre != FAIRY_TALE)
            new_play.age_rating = AGE_RATING_NONE;
    }

    if (rc == SUCCCESS_CODE && new_play.play_type == MUSICAL)
    {
        printf("%sВведите имя композитоора: %s", BLUE, RESET);
        scanf("%s", new_play.play_data.musical_info.composer);
        printf("%sВведите страну происхождения: %s", BLUE, RESET);
        scanf("%s", new_play.play_data.musical_info.country);
        printf("%sВведите вид мюзикла:\n0 - балет\n1 - опера\n2 - мюзикл\n%s", BLUE, RESET);
        if (scanf("%d", &temp_int) != 1 || temp_int < 0 || temp_int > 2)
            rc = INVALID_INPUT;
        else 
            new_play.play_data.musical_info.musical_genre = (musical_genre_t)temp_int;
        if (rc == SUCCCESS_CODE)
        {
            printf("%sВведите возраст (целое число):\n0 - 3+\n1 - 10+\n2 - 16+\n%s", BLUE, RESET);
            if (scanf("%d", &temp_int) != 1 || temp_int < 0 || temp_int > 2)
                rc  = INVALID_INPUT;
            else 
                new_play.age_rating = (age_rating_t)temp_int;
        }
        if (rc == SUCCCESS_CODE)
        {
            printf("%sВведите продолжительность в минутах (целое число): %s", BLUE, RESET);
            if (scanf("%d", &new_play.play_data.musical_info.duration) != 1 || temp_int < 0)
                rc = INVALID_INPUT;
        }
    }

    if (rc != SUCCCESS_CODE)
        while (getchar() != '\n');

    theater_plays_arr[*theater_plays_q] = new_play;
    theater_plays_keys[*theater_plays_q] = *theater_plays_q;
    (*theater_plays_q)++; 

    return rc;
}

status_t delete_play(theater_play_t *theater_plays_arr, int *theater_plays_keys, size_t *theater_plays_q)
{
    status_t rc = SUCCCESS_CODE;
    char string_to_delete[MAX_STR_LEN];
    int field_to_delete, field_in_field_to_delete;
    int duration_to_delete;
    double price_to_delete;

    if (*theater_plays_q <= 0 || theater_plays_arr == NULL || theater_plays_keys == NULL)
        return INVALID_DELETE;

    print_delete_instructions();

    if (scanf("%d", &field_to_delete) != 1 || field_to_delete < 1 || field_to_delete > 9)
        rc = INVALID_INPUT;

    if (rc == SUCCCESS_CODE)
    {
        switch (field_to_delete)
        {
            case 1: // название театра
                rc = input_string_to_delete(string_to_delete);
                if (rc == SUCCCESS_CODE)
                    for (size_t i = 0; i < *theater_plays_q; i++)
                        shift_array_with_delete_one_string_elem(theater_plays_arr, theater_plays_q, &i, string_to_delete, theater_plays_arr[i].theater_name);
                break;

            case 2:  // название спектакля
                rc = input_string_to_delete(string_to_delete);
                if (rc == SUCCCESS_CODE)
                    for (size_t i = 0; i < *theater_plays_q; i++)
                        shift_array_with_delete_one_string_elem(theater_plays_arr, theater_plays_q, &i, string_to_delete, theater_plays_arr[i].play_name);
                break;

            case 3:  // цена билета
                printf("%sВведите цену, записи с которой будут удалены: %s", BLUE, RESET);

                if (scanf("%lf", &price_to_delete) != 1 || price_to_delete < 0)
                    rc = INVALID_DELETE;

                if (rc == SUCCCESS_CODE)
                {
                    for (size_t i = 0; i < *theater_plays_q; i++)
                        if (fabs(price_to_delete - theater_plays_arr[i].ticket_price) < EPS)
                        {
                            for (size_t j = i; j < *theater_plays_q - 1; j++)
                                theater_plays_arr[j] = theater_plays_arr[j + 1];
                            (*theater_plays_q)--;
                            i--;
                        }
                }
                // TODO: ключи подвинуть
                break;

            case 4:  // тип спектакля
                printf("%s", BLUE);
                printf("Выберите значение для удаления:\n");
                printf("0 - пьесы\n");
                printf("1 - мюзиклы\n");
                printf("%s", RESET);

                if (scanf("%d", &field_in_field_to_delete) != 1 || field_in_field_to_delete < 0 || field_in_field_to_delete > 1)
                    rc = INVALID_DELETE;

                if (rc == SUCCCESS_CODE)
                    for (size_t i = 0; i < *theater_plays_q; i++)
                        shift_array_with_delete_one_integer_elem(theater_plays_arr, theater_plays_q, &i, field_in_field_to_delete, theater_plays_arr[i].play_type);
                break;

            case 5:  // возраст
                printf("%s", BLUE);
                printf("Выберите значение для удаления:\n");
                printf("0 - 3+\n");
                printf("1 - 10+\n");
                printf("2 - 16+\n");
                printf("%s", RESET);

                if (scanf("%d", &field_in_field_to_delete) != 1 || field_in_field_to_delete < 0 || field_in_field_to_delete > 2)
                    rc = INVALID_DELETE;

                if (rc == SUCCCESS_CODE)
                    for (size_t i = 0; i < *theater_plays_q; i++)
                        shift_array_with_delete_one_integer_elem(theater_plays_arr, theater_plays_q, &i, field_in_field_to_delete, theater_plays_arr[i].age_rating);

                break;

            case 6:  // композитор
                rc = input_string_to_delete(string_to_delete);
                if (rc == SUCCCESS_CODE)
                    for (size_t i = 0; i < *theater_plays_q; i++)
                        shift_array_with_delete_one_string_elem(theater_plays_arr, theater_plays_q, &i, string_to_delete, theater_plays_arr[i].play_data.musical_info.composer);
                break;

            case 7:  // страна
                rc = input_string_to_delete(string_to_delete);
                if (rc == SUCCCESS_CODE)
                    for (size_t i = 0; i < *theater_plays_q; i++)
                        shift_array_with_delete_one_string_elem(theater_plays_arr, theater_plays_q, &i, string_to_delete, theater_plays_arr[i].play_data.musical_info.country);
                break;

            case 8:  // тип выступления
                printf("%s", BLUE);
                printf("Выберите значение для удаления:\n");
                printf("0 - балет\n");
                printf("1 - опера\n");
                printf("2 - музыкальное шоу\n");
                printf("3 - драма\n");
                printf("4 - комедия\n");
                printf("5 - сказка\n");
                printf("%s", RESET);

                if (scanf("%d", &field_in_field_to_delete) != 1 || field_in_field_to_delete < 0 || field_in_field_to_delete > 5)
                    rc = INVALID_DELETE;

                if (rc == SUCCCESS_CODE)
                {
                    if (field_in_field_to_delete < 3)  // для мюзиклов
                        for (size_t i = 0; i < *theater_plays_q; i++)
                            shift_array_with_delete_one_integer_elem(theater_plays_arr, theater_plays_q, &i, field_in_field_to_delete, theater_plays_arr[i].play_data.musical_info.musical_genre);
                    else  // для пьес
                    {
                        field_in_field_to_delete -= 3;
                        for (size_t i = 0; i < *theater_plays_q; i++)
                            shift_array_with_delete_one_integer_elem(theater_plays_arr, theater_plays_q, &i, field_in_field_to_delete, theater_plays_arr[i].play_data.piece_info.piece_genre);
                    }
                }
                break;

            case 9:  // продолжительность
                printf("%sВведите длительность, записи с которой будут удалены: %s", BLUE, RESET);
                
                if (scanf("%d", &duration_to_delete) != 1 || duration_to_delete <= 0)
                    rc = INVALID_DELETE;

                if (rc == SUCCCESS_CODE)
                    for (size_t i = 0; i < *theater_plays_q; i++)
                        shift_array_with_delete_one_integer_elem(theater_plays_arr, theater_plays_q, &i, duration_to_delete, theater_plays_arr[i].play_data.musical_info.duration);
                break;
            
            default:
                rc = INVALID_DELETE;
                break;
        }
    }

    return rc;        
}

status_t input_string_to_delete(char *target_string)
{
    printf("%sВведите строку, по которой будет будет очищено поле: %s", BLUE, RESET);
    if (scanf("%s", target_string) != 1)
        return INVALID_DELETE;

    size_t target_string_len = strlen(target_string);
    if (target_string[target_string_len - 1] == '\n')
        target_string[target_string_len - 1] = '\0';

    return SUCCCESS_CODE;
}

status_t shift_array_with_delete_one_string_elem(theater_play_t *theater_plays_arr, size_t *theater_plays_q, size_t *current_pos, char *string_to_delete, char *source_string_to_compare)
{
    status_t rc = SUCCCESS_CODE;
    
    if (strcmp(string_to_delete, source_string_to_compare) == 0)
    {
        for (size_t j = *current_pos; j < *theater_plays_q - 1; j++)
            theater_plays_arr[j] = theater_plays_arr[j + 1];
        (*theater_plays_q)--;
        (*current_pos)--;
    }

    // TODO че с ключами
    return rc;
}

status_t shift_array_with_delete_one_integer_elem(theater_play_t *theater_plays_arr, size_t *theater_plays_q, size_t *current_pos, int int_to_delete, int source_int_to_delete)
{
    status_t rc = SUCCCESS_CODE;
    
    if (int_to_delete == source_int_to_delete)
    {
        for (size_t j = *current_pos; j < *theater_plays_q - 1; j++)
            theater_plays_arr[j] = theater_plays_arr[j + 1];
        (*theater_plays_q)--;
        (*current_pos)--;
    }

    // TODO: удалить из списка ключей

    return rc;
}

status_t clean_arr(theater_play_t *theater_plays_arr, size_t *theater_plays_q)
{
    if (theater_plays_arr == NULL || theater_plays_q == NULL)
        return ARR_PROCESSING_ERROR;

    *theater_plays_q = 0;
    memset(theater_plays_arr, 0, sizeof(theater_play_t) * MAX_PLAYS_QUANTITY);

    return SUCCCESS_CODE;
}