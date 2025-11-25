#include "file_functional.h"

status_t safe_open_file(char *filename, FILE **filestream);

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

