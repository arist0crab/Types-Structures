#define _GNU_SOURCE
#include "../inc/hst.h"
#include <math.h>

static int current_hash_func = 1;

hst_chaining_t *create_hash_table_chaining(int size) 
{
    hst_chaining_t *ht = (hst_chaining_t *)malloc(sizeof(hst_chaining_t));

    if (!ht) return NULL;

    ht->size = size;
    ht->count = 0;
    ht->collisions = 0;
    ht->total_comparisons = 0;
    ht->searches_count = 0;

    ht->table = (hash_node_t **)calloc(size, sizeof(hash_node_t *));
    if (!ht->table) 
    {
        free(ht);
        return NULL;
    }
    return ht;
}

void free_hash_table_chaining(hst_chaining_t *ht) 
{
    if (!ht) return;

    for (int i = 0; i < ht->size; i++) 
    {
        hash_node_t *current = ht->table[i];
        while (current) 
        {
            hash_node_t *temp = current;
            current = current->next;
            free(temp->word);
            free(temp);
        }
    }
    free(ht->table);
    free(ht);
}

// djb2
unsigned int hash_function1(const char *word, int table_size) 
{
    unsigned long hash = 5381;
    int c;

    while ((c = (unsigned char)*word++))
        hash = ((hash << 5) + hash) + c;

    return (unsigned int)(hash % table_size);
}

// sdbm
unsigned int hash_function2(const char *word, int table_size) 
{
    unsigned long hash = 0;
    int c;

    while ((c = (unsigned char)*word++))
        hash = c + (hash << 6) + (hash << 16) - hash;

    return (unsigned int)(hash % table_size);
}

static unsigned int hash_fixed(const char *word, int table_size, int func_id) 
{
    return (func_id == 1) ? hash_function1(word, table_size) : hash_function2(word, table_size);
}

unsigned int hash_function(const char *word, int table_size) 
{
    return hash_fixed(word, table_size, current_hash_func);
}

void hash_table_insert_chaining(hst_chaining_t *ht, const char *word) 
{
    if (!ht || !word) return;

    int saved_hash_func = current_hash_func;

    if ((double)(ht->count + 1) / ht->size > MAX_LOAD_FACTOR) 
    {
        printf("Загруженность превышена (%.2f > %.2f). Рехеширование...\n",
               (double)ht->count / ht->size, MAX_LOAD_FACTOR);
        rehash_chaining(ht);
    }

    unsigned int index = hash_fixed(word, ht->size, saved_hash_func);
    hash_node_t *current = ht->table[index];
    int chain_length = 0;

    // Проверяем существование
    while (current) 
    {
        chain_length++;
        if (!current->is_deleted && strcmp(current->word, word) == 0) 
        {
            current->count++;
            return;
        }
        current = current->next;
    }

    // Если цепочка слишком длинная - меняем функцию
    if (chain_length >= 4) 
    {
        printf("Длина цепочки %d превышает лимит. Меняем хеш-функцию...\n", chain_length);
        current_hash_func = (current_hash_func == 1) ? 2 : 1;
        return; 
    }

    // Создаем новый узел
    hash_node_t *new_node = malloc(sizeof(hash_node_t));
    if (!new_node) return;

    new_node->word = str_dynamic_copy(word);
    new_node->count = 1;
    new_node->is_deleted = false;
    new_node->next = ht->table[index];

    if (ht->table[index]) ht->collisions++;
    ht->table[index] = new_node;
    ht->count++;
}

hash_node_t *hash_table_search_chaining(hst_chaining_t *ht, const char *word, int *comparisons) 
{
    if (!ht) return NULL;

    int saved_hash_func = current_hash_func;
    *comparisons = 0;

    unsigned int index = hash_fixed(word, ht->size, saved_hash_func);
    hash_node_t *current = ht->table[index];

    while (current) 
    {
        (*comparisons)++;
        ht->total_comparisons++;

        if (!current->is_deleted && strcmp(current->word, word) == 0) 
        {
            ht->searches_count++;
            return current;
        }
        current = current->next;
    }

    ht->searches_count++;
    if (*comparisons == 0) *comparisons = 1;
    return NULL;
}

void hash_table_delete_chaining(hst_chaining_t *ht, const char *word) 
{
    if (!ht) return;

    int saved_hash_func = current_hash_func;
    unsigned int index = hash_fixed(word, ht->size, saved_hash_func);
    hash_node_t *current = ht->table[index];
    hash_node_t *prev = NULL;

    while (current) 
    {
        if (!current->is_deleted && strcmp(current->word, word) == 0) 
        {
            hash_node_t *next_node = current->next;

            if (prev == NULL)
                ht->table[index] = next_node;
            else
                prev->next = next_node;

            free(current->word);
            free(current);
            ht->count--;
            return;
        }
        prev = current;
        current = current->next;
    }
}

double calculate_load_factor_chaining(hst_chaining_t *ht) 
{
    return (double)ht->count / ht->size;
}

void rehash_chaining(hst_chaining_t *ht) 
{
    if (!ht) return;

    int old_size = ht->size;
    hash_node_t **old_table = ht->table;
    int saved_hash_func = current_hash_func;

    int new_size = (int)(old_size * 1.3);
    while (new_size % 2 == 0 || new_size % 3 == 0) new_size++;

    ht->table = (hash_node_t **)calloc(new_size, sizeof(hash_node_t *));
    if (!ht->table) return;

    ht->size = new_size;
    ht->count = 0;
    ht->collisions = 0;

    for (int i = 0; i < old_size; i++) 
    {
        hash_node_t *curr = old_table[i];
        while (curr) 
        {
            hash_node_t *next = curr->next;
            if (!curr->is_deleted) 
            {
                unsigned int new_index = hash_fixed(curr->word, ht->size, saved_hash_func);
                curr->next = ht->table[new_index];
                ht->table[new_index] = curr;
                ht->count++;
            } 
            else 
            {
                free(curr->word);
                free(curr);
            }
            curr = next;
        }
    }

    free(old_table);
    printf("Рехеширование завершено. Новый размер: %d\n", ht->size);
}

hst_open_t *create_hash_table_open(int size) 
{
    hst_open_t *ht = (hst_open_t *)malloc(sizeof(hst_open_t));
    if (!ht) return NULL;

    ht->size = size;
    ht->count = 0;
    ht->collisions = 0;
    ht->total_comparisons = 0;
    ht->searches_count = 0;

    ht->keys = (char**)calloc(size, sizeof(char*));
    ht->counts = (int *)calloc(size, sizeof(int));
    ht->occupied = (bool *)calloc(size, sizeof(bool));
    ht->deleted = (bool *)calloc(size, sizeof(bool));

    if (!ht->keys || !ht->counts || !ht->occupied || !ht->deleted) 
    {
        free(ht->keys);
        free(ht->counts);
        free(ht->occupied);
        free(ht->deleted);
        free(ht);
        return NULL;
    }
    return ht;
}

void free_hash_table_open(hst_open_t *ht) 
{
    if (!ht) return;

    if (ht->keys)
        for (int i = 0; i < ht->size; i++)
            if (ht->keys[i]) free(ht->keys[i]);

    free(ht->keys);
    free(ht->counts);
    free(ht->occupied);
    free(ht->deleted);
    free(ht);
}

void hash_table_insert_open(hst_open_t *ht, const char *word) 
{
    if (!ht || !word) return;

    int saved_hash_func = current_hash_func;

    if ((double)(ht->count + 1) / ht->size > MAX_LOAD_FACTOR) 
    {
        printf("Загруженность превышена. Рехеширование...\n");
        rehash_open(ht);
    }

    int index = hash_fixed(word, ht->size, saved_hash_func);
    int original_index = index;
    int probe_count = 0;

    while (ht->occupied[index] && !ht->deleted[index]) 
    {
        if (ht->keys[index] && strcmp(ht->keys[index], word) == 0) 
        {
            ht->counts[index]++;
            return;
        }
        probe_count++;
        index = (index + 1) % ht->size;
        if (index == original_index) break;
    }

    ht->keys[index] = str_dynamic_copy(word);
    ht->counts[index] = 1;
    ht->occupied[index] = true;
    ht->deleted[index] = false;
    ht->count++;

    if (probe_count > 0) ht->collisions++;
}

int hash_table_search_open(hst_open_t *ht, const char *word, int *comparisons) 
{
    if (!ht) return -1;

    int saved_hash_func = current_hash_func;
    *comparisons = 0;

    int index = hash_fixed(word, ht->size, saved_hash_func);
    int original_index = index;

    while (ht->occupied[index] || ht->deleted[index]) 
    {
        (*comparisons)++;
        ht->total_comparisons++;

        if (ht->occupied[index] && !ht->deleted[index] && 
            ht->keys[index] && strcmp(ht->keys[index], word) == 0) 
            {
            ht->searches_count++;
            return index;
        }

        index = (index + 1) % ht->size;
        if (index == original_index) break;
    }

    if (*comparisons == 0) *comparisons = 1;
    ht->searches_count++;
    return -1;
}

void hash_table_delete_open(hst_open_t *ht, const char *word) 
{
    if (!ht) return;

    int comparisons;
    int index = hash_table_search_open(ht, word, &comparisons);

    if (index != -1) 
    {
        ht->deleted[index] = true;
        ht->occupied[index] = false;
        ht->count--;
    }
}

double calculate_load_factor_open(hst_open_t *ht) 
{
    return (double)ht->count / ht->size;
}

void rehash_open(hst_open_t *ht) 
{
    if (!ht) return;

    int old_size = ht->size;
    char **old_keys = ht->keys;
    int *old_counts = ht->counts;
    bool *old_occupied = ht->occupied;
    bool *old_deleted = ht->deleted;

    int new_size = (int)(old_size * 1.3);
    while (new_size % 2 == 0 || new_size % 3 == 0) new_size++;

    ht->keys = (char **)calloc(new_size, sizeof(char *));
    ht->counts = (int *)calloc(new_size, sizeof(int));
    ht->occupied = (bool *)calloc(new_size, sizeof(bool));
    ht->deleted = (bool *)calloc(new_size, sizeof(bool));
    
    if (!ht->keys || !ht->counts || !ht->occupied || !ht->deleted) return;

    ht->size = new_size;
    ht->count = 0;
    ht->collisions = 0;

    for (int i = 0; i < old_size; i++)
        if (old_occupied[i] && !old_deleted[i] && old_keys[i])
            for (int j = 0; j < old_counts[i]; j++)
                hash_table_insert_open(ht, old_keys[i]);

    free(old_keys);
    free(old_counts);
    free(old_occupied);
    free(old_deleted);
    printf("Рехеширование завершено. Новый размер: %d\n", ht->size);
}

void build_hash_table_from_file_chaining(hst_chaining_t *ht, const char *filename) 
{
    FILE *file = fopen(filename, "r");
    if (!file) 
    {
        printf("Ошибка открытия файла '%s'\n", filename);
        return;
    }

    char buffer[MAX_STRING_LENGTH];
    int words_processed = 0;
    printf("Построение хеш-таблицы с цепочками...\n");

    while (fscanf(file, "%s", buffer) == 1) 
    {
        hash_table_insert_chaining(ht, buffer);
        words_processed++;
    }

    fclose(file);
    printf("Обработано слов: %d\n", words_processed);
}

void build_hash_table_from_file_open(hst_open_t *ht, const char *filename) 
{
    FILE *file = fopen(filename, "r");
    if (!file) 
    {
        printf("Ошибка открытия файла '%s'\n", filename);
        return;
    }

    char buffer[MAX_STRING_LENGTH];
    int words_processed = 0;
    printf("Построение хеш-таблицы с открытой адресацией...\n");

    while (fscanf(file, "%s", buffer) == 1) 
    {
        hash_table_insert_open(ht, buffer);
        words_processed++;
    }

    fclose(file);
    printf("Обработано слов: %d\n", words_processed);
}

double get_average_comparisons_chaining(hst_chaining_t *ht) 
{
    return ht->searches_count > 0 ? (double)ht->total_comparisons / ht->searches_count : 0;
}

double get_average_comparisons_open(hst_open_t *ht) 
{
    return ht->searches_count > 0 ? (double)ht->total_comparisons / ht->searches_count : 0;
}
