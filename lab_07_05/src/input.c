#include "../inc/input.h"

void clear_input_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

result_t input_menu_option(menu_option_t *current_option)
{
    result_t exit_code = OK_CODE;
    int temp;

    printf("Выберите пункт меню: ");

    if (scanf("%d", &temp) != 1 || temp < 0 || temp > MENU_OPTIONS)
    {
        // printf("HERE\n");
        exit_code = INPUT_ERR_CODE;
    }
    
    *current_option = (menu_option_t)temp;

    clear_input_buffer();

    return exit_code;
}

result_t input_filename(char *filename)
{
    result_t exit_code = OK_CODE;

    printf("Введите имя файла: ");

    if (!fgets(filename, MAX_FILENAME_LENGTH, stdin))
        exit_code = INPUT_ERR_CODE;
    else
        filename[strcspn(filename, "\n")] = '\0';

    return exit_code;
}

result_t input_integer(int *number)
{
    result_t exit_code = OK_CODE;

    printf("Введите целое число: ");

    if (scanf("%d", number) != 1)
        exit_code = INPUT_ERR_CODE;

    clear_input_buffer();

    return exit_code;
}

result_t input_string(char **word, char *message)
{
    result_t ec = OK_CODE;
    char buffer[MAX_STRING_LENGTH];
    char extra;

    if (!word)
        ec = INVALID_PTR_CODE;

    if (ec == OK_CODE)
    {
        printf("%s",message);
        if (scanf("%s", buffer) != 1)
            ec = INPUT_ERR_CODE;
    }

    if (ec == OK_CODE)
    {
        *word = malloc(strlen(buffer) + 1);
        if (!(*word))
            ec = MEMORY_ERR_CODE;
    }

    if (ec == OK_CODE)
        strcpy(*word, buffer);

    // чистим буфер
    while ((extra = getchar()) != '\n' && extra != EOF);

    return ec;
}

result_t input_size(size_t *number)
{
    result_t exit_code = OK_CODE;

    printf("Введите размер (натуральное число): ");

    if (scanf("%zu", number) != 1 || *number == 0)
        exit_code = INPUT_ERR_CODE;

    clear_input_buffer();

    return exit_code;
}

result_t add_number_to_file(const char *filename, const int number)
{
    result_t exit_code = OK_CODE;
    FILE *file = NULL;

    if (!(file = fopen(filename, "a")))
        exit_code = FILE_OPEN_ERR_CODE;

    if (exit_code == OK_CODE)
        fprintf(file, " %d", number);

    fclose(file);

    return exit_code;
}

result_t get_integers_from_file(const char *filename, int **arr, size_t *arr_length)
{
    result_t exit_code = OK_CODE;
    FILE *filestream = NULL;
    int x;
    size_t index = 0;

    if (!filename || !arr || !arr_length)
        return INVALID_PTR_CODE;

    *arr = NULL;
    *arr_length = 0;

    filestream = fopen(filename, "r");
    if (!filestream)
        return FILE_OPEN_ERR_CODE;

    if (exit_code != OK_CODE)
        return exit_code;

    while (fscanf(filestream, "%d", &x) == 1)
        (*arr_length)++;

    if (*arr_length == 0) {
        fclose(filestream);
        return OK_CODE;
    }

    *arr = malloc(*arr_length * sizeof(int));
    if (!*arr) {
        fclose(filestream);
        return MEMORY_ERR_CODE;
    }

    rewind(filestream);
    index = 0;

    while (index < *arr_length && fscanf(filestream, "%d", &x) == 1) {
        (*arr)[index++] = x;
    }

    fclose(filestream);

    if (index < *arr_length) {
        free(*arr);
        *arr = NULL;
        *arr_length = 0;
        return FILE_READ_ERR_CODE;
    }

    return OK_CODE;
}

result_t get_words_from_file(const char *filename, char ***arr, size_t *arr_length)
{
    result_t exit_code = OK_CODE;
    FILE *filestream = NULL;
    char buffer[MAX_STRING_LENGTH];
    size_t index = 0;

    if (!filename || !arr || !arr_length)
        exit_code = INVALID_PTR_CODE;
    else
    {
        *arr = NULL;
        *arr_length = 0;

        filestream = fopen(filename, "r");
        if (!filestream)
            exit_code = FILE_OPEN_ERR_CODE;
    }

    if (exit_code == OK_CODE)
    {
        while (fscanf(filestream, "%s", buffer) == 1)
            (*arr_length)++;

        if (*arr_length == 0) {
            fclose(filestream);
            return OK_CODE;
        }

        *arr = malloc(*arr_length * sizeof(char *));
        if (!*arr) {
            fclose(filestream);
            exit_code =  MEMORY_ERR_CODE;
        }
    }

    if (exit_code == OK_CODE)
    {

        rewind(filestream);
        index = 0;

        while ((exit_code == OK_CODE) && index < *arr_length && fscanf(filestream, "%s", buffer) == 1) {
            (*arr)[index] = str_dynamic_copy(buffer);  
            if (!(*arr)[index]) {
                for (size_t i = 0; i < index; i++)
                    free((*arr)[i]);
                free(*arr);
                *arr = NULL;
                *arr_length = 0;
                fclose(filestream);
                exit_code =  MEMORY_ERR_CODE;
            }
            index++;
        }

        fclose(filestream);
    }

    if (exit_code == OK_CODE)
    {
        if (index < *arr_length) {
            for (size_t i = 0; i < index; i++)
                free((*arr)[i]);
            free(*arr);
            *arr = NULL;
            *arr_length = 0;
            exit_code = FILE_READ_ERR_CODE;
        }
    }

    return exit_code;
}
