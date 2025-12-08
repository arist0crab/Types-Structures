#include "hst.h"

status_t destroy_hash_table(hash_table_t **table);
status_t create_hash_table(hash_table_t **table, size_t table_size);
status_t hash_djb2(const char *str, size_t table_size, size_t *hash_value);
status_t find_word_in_hash_table(const hash_table_t *table, const char *word, hash_node_t **found_node);

status_t build_hash_from_file(hash_table_t **table, const char *filename)
{
    status_t ec = SUCCESS_CODE;
    char **words = NULL;
    size_t words_quantity = 0;

    if (!table || !(*table) || !filename)
        ec = ERR_ARGS;

    if (ec == SUCCESS_CODE)
        ec = get_words_arr_from_file(filename, &words, &words_quantity);

    for (size_t i = 0; ec == SUCCESS_CODE && i < words_quantity; i++)
        ec = insert_hst_node(*table, words[i]);
    
    free_words_arr(&words, words_quantity);

    return ec;
}

status_t user_init_table(hash_table_t **table)
{
    status_t ec = SUCCESS_CODE;
    size_t temp_size = 0;

    if (!table) ec = ERR_ARGS;

    if (ec == SUCCESS_CODE)
        ec = input_size(&temp_size);

    if (ec == SUCCESS_CODE)
        ec = create_hash_table(table, temp_size);

    return ec;
}

status_t create_hash_table(hash_table_t **table, size_t table_size)
{
    status_t ec = SUCCESS_CODE;

    if (!table) ec = ERR_ARGS;

    if (ec == SUCCESS_CODE && *table)
        destroy_hash_table(table);

    if (ec == SUCCESS_CODE)
    {
        *table = malloc(sizeof(hash_table_t));
        if (*table == NULL)
            ec = ERR_MEM;
    }

    if (ec == SUCCESS_CODE)
    {
        (*table)->data = calloc(table_size, sizeof(hash_node_t *));
        if ((*table)->data == NULL)
            ec = ERR_MEM;
    }

    if (ec == SUCCESS_CODE)
    {
        (*table)->max_size = table_size;
        (*table)->uniq_words_quantity = 0;
    }

    if (ec != SUCCESS_CODE)
        ec = destroy_hash_table(table);

    return ec;
}

status_t destroy_hash_table(hash_table_t **table)
{
    status_t ec = SUCCESS_CODE;
    hash_node_t *current = NULL, *next = NULL;

    if (!table || !(*table)) 
        ec = ERR_ARGS;

    for (size_t i = 0; ec == SUCCESS_CODE && i < (*table)->max_size; i++)
    {
        current = (*table)->data[i];
        while (current)
        {
            next = current->next;
            if (current->word) free(current->word);
            free(current);
            current = next;
        }
    }

    if (ec == SUCCESS_CODE)
    {
        free((*table)->data);
        free(*table);
        *table = NULL;
    }

    return ec;
}

status_t insert_hst_node(hash_table_t *table, const char *word)
{
    status_t ec = SUCCESS_CODE;

    size_t idx = 0;
    bool exists = false;
    hash_node_t *node = NULL, *search_node = NULL;

    if (!table || !word || word[0] == '\0')
        ec = ERR_ARGS;

    if (ec == SUCCESS_CODE)
    {
        find_word_in_hash_table(table, word, &search_node);
        exists = (search_node != NULL);
    }

    if (ec == SUCCESS_CODE && !exists)
        ec = hash_djb2(word, table->max_size, &idx);

    if (ec == SUCCESS_CODE && !exists) 
    {
        node = malloc(sizeof(hash_node_t));
        if (!node) ec = ERR_MEM;
    }

    if (ec == SUCCESS_CODE && !exists) 
    {
        node->word = str_dynamic_copy(word);
        if (!node->word) ec = ERR_MEM;
    }

    if (ec == SUCCESS_CODE && !exists) 
    {
        node->next = table->data[idx];
        table->data[idx] = node;
        table->uniq_words_quantity++;
        node->tail_length = (node->next) ? node->next->tail_length + 1 : 0;
    }

    if (ec != SUCCESS_CODE && node) 
    {
        if (node->word) free(node->word);
        free(node);
    }

    return ec;
}

status_t find_word_in_hash_table(const hash_table_t *table, const char *word, hash_node_t **found_node)
{
    status_t ec = SUCCESS_CODE;

    size_t idx = 0;
    hash_node_t *current = NULL;

    if (!table || !word || !found_node)
        ec = ERR_ARGS;

    if (ec == SUCCESS_CODE)
        *found_node = NULL;

    if (ec == SUCCESS_CODE)
        ec = hash_djb2(word, table->max_size, &idx);

    if (ec == SUCCESS_CODE)
    {
        current = table->data[idx];
        while (current)
        {
            if (strcmp(current->word, word) == 0)
            {
                *found_node = current;
                break;
            }
            current = current->next;
        }
    }

    return ec;
}

status_t hash_djb2(const char *str, size_t table_size, size_t *hash_value)
{
    status_t ec = SUCCESS_CODE;
    size_t hash = 5381;
    int c = 0;

    if (!str || !hash_value)
        ec = ERR_ARGS;

    if (ec == SUCCESS_CODE) 
    {
        while ((c = *str++))
            hash = ((hash << 5) + hash) + c;  // hash * 33 + c (djb2)
        
        *hash_value = hash % table_size;
    }

    return ec;
}