#include "file_functional.h"

// TODO подумать как сделать эту функцию проще
status_t find_word_in_file(char *filename, char *word, ssize_t *word_num)
{
    status_t ec = SUCCESS_CODE;
    char buf[MAX_STRING_LENGTH];
    FILE *filestream = NULL;
    bool found = 0;

    if (!filename || !word_num || !word || word[0] == '\0')
        ec = ERR_ARGS;

    if (ec == SUCCESS_CODE)
    {
        *word_num = -1;
        ec = safe_open_file(filename, &filestream);
    }

    if (ec == SUCCESS_CODE)
    {
        while (fscanf(filestream, "%s", buf) == 1 && !found)
        {
            (*word_num)++;
            if (strcmp(buf, word) == 0)
                found = true;
        }
    }

    if (ec == SUCCESS_CODE && !found)
        *word_num = -1;

    if (filestream)
        fclose(filestream);

    return ec;
}

status_t read_tree_from_file(tree_node_t **root, char *filename)
{
    status_t ec = SUCCESS_CODE;
    FILE *filestream = NULL;
    char buf[MAX_STRING_LENGTH];

    if (!filename || filename[0] == '\0')
        ec = ERR_ARGS;

    if (ec == SUCCESS_CODE)
        ec = safe_open_file(filename, &filestream);

    if (ec == SUCCESS_CODE)
        while (fscanf(filestream, "%s", buf) == 1)
            insert_tree_node(root, buf);
    
    if (filestream)
        fclose(filestream);

    return ec;
}

status_t insert_word_to_file(const char *filename, char *target_word)
{
    status_t ec = SUCCESS_CODE;
    FILE *filestream = NULL;

    if (!filename || !target_word)
        ec = ERR_ARGS;

    if (ec == SUCCESS_CODE)
    {
        filestream = fopen(filename, "a");
        if (!filestream)
            ec = ERR_FILE;
    }

    if (ec == SUCCESS_CODE)
        if (fprintf(filestream, " %s", target_word) < 0)
            ec = ERR_FILE;

    if (filestream)
        fclose(filestream);

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

