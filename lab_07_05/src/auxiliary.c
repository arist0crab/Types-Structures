#include "../inc/auxiliary.h"


result_t get_words_arr_from_file(const char *filename, char ***words_arr, size_t *arr_length)
{
    result_t ec = OK_CODE;
    char buf[MAX_STRING_LENGTH];
    FILE *filestream = NULL;
    size_t index = 0;

    if (!filename || !words_arr || !arr_length)
        ec = INVALID_PTR_CODE;

    if (ec == OK_CODE)
    {
        *arr_length = 0;
        *words_arr = NULL;
        ec = safe_open_file((char *)filename, &filestream);
    }

    while (ec == OK_CODE && fscanf(filestream, "%s", buf) == 1)
        (*arr_length)++;

    if (ec == OK_CODE)
    {
        rewind(filestream);   
        *words_arr = malloc(*arr_length * sizeof(char *)); 
        if (!(*words_arr))
            ec = MEMORY_ERR_CODE;
    }

    for (; ec == OK_CODE && index < *arr_length; index++)
    {
        fscanf(filestream, "%s", buf);
        (*words_arr)[index] = str_dynamic_copy(buf);
        if (!(*words_arr)[index])
            ec = MEMORY_ERR_CODE;
    }

    if (filestream)
        fclose(filestream);

    if (ec == MEMORY_ERR_CODE && *words_arr)
    {
        for (size_t i = 0; i < index; i++)
            free((*words_arr)[i]);
        free(*words_arr);
        *words_arr = NULL;
        *arr_length = 0;
    }

    return ec;
}

result_t free_words_arr(char ***words_arr, size_t arr_length)
{
    result_t ec = OK_CODE;

    if (!words_arr || !*words_arr)
        ec = INVALID_PTR_CODE;
        
    for (size_t i = 0; ec == OK_CODE && i < arr_length; i++)
        if ((*words_arr)[i])
            free((*words_arr)[i]);

    if (ec == OK_CODE)
    {
        free(*words_arr);
        *words_arr = NULL;
    }

    return ec;
}


result_t safe_open_file(const char *filename, FILE **filestream)
{
    result_t ec = OK_CODE;

    if (!filename || filename[0] == '\0' || !filestream)
        ec = INVALID_PTR_CODE;

    if (ec == OK_CODE)
    {
        *filestream = fopen(filename, "r");
        if (*filestream == NULL)
            ec = FILE_OPEN_ERR_CODE;
    }

    return ec;
}

char *str_dynamic_copy(const char *s) 
{
    char *result = NULL;
    size_t len = 0;

    if (s)
    {
        len = strlen(s);
        result = malloc((len + 1) * sizeof(char));
        if (result)
            memcpy(result, s, (len + 1) * sizeof(char));
    }

    return result;
}