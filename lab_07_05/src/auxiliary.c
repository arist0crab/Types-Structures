#include "auxiliary.h"


status_t get_words_arr_from_file(const char *filename, char ***words_arr, size_t *arr_length)
{
    status_t ec = SUCCESS_CODE;
    char buf[MAX_STRING_LENGTH];
    FILE *filestream = NULL;
    size_t index = 0;

    if (!filename || !words_arr || !arr_length)
        ec = ERR_ARGS;

    if (ec == SUCCESS_CODE)
    {
        *arr_length = 0;
        *words_arr = NULL;
        ec = safe_open_file((char *)filename, &filestream);
    }

    while (ec == SUCCESS_CODE && fscanf(filestream, "%s", buf) == 1)
        (*arr_length)++;

    if (ec == SUCCESS_CODE)
    {
        rewind(filestream);   
        *words_arr = malloc(*arr_length * sizeof(char *)); 
        if (!(*words_arr))
            ec = ERR_MEM;
    }

    for (; ec == SUCCESS_CODE && index < *arr_length; index++)
    {
        fscanf(filestream, "%s", buf);
        (*words_arr)[index] = str_dynamic_copy(buf);
        if (!(*words_arr)[index])
            ec = ERR_MEM;
    }

    if (filestream)
        fclose(filestream);

    if (ec == ERR_MEM && *words_arr)
    {
        for (size_t i = 0; i < index; i++)
            free((*words_arr)[i]);
        free(*words_arr);
        *words_arr = NULL;
        *arr_length = 0;
    }

    return ec;
}

status_t free_words_arr(char ***words_arr, size_t arr_length)
{
    status_t ec = SUCCESS_CODE;

    if (!words_arr || !*words_arr)
        ec = ERR_ARGS;
        
    for (size_t i = 0; ec == SUCCESS_CODE && i < arr_length; i++)
        if ((*words_arr)[i])
            free((*words_arr)[i]);

    if (ec == SUCCESS_CODE)
    {
        free(*words_arr);
        *words_arr = NULL;
    }

    return ec;
}


status_t safe_open_file(char *filename, FILE **filestream)
{
    status_t ec = SUCCESS_CODE;

    if (!filename || filename[0] == '\0' || !filestream)
        ec = ERR_ARGS;

    if (ec == SUCCESS_CODE)
    {
        *filestream = fopen(filename, "r");
        if (*filestream == NULL)
            ec = ERR_FILE;
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