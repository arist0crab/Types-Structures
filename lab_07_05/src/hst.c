#define _GNU_SOURCE
#include "../inc/hst.h"
#include <math.h>

static int current_hash_func = 1;

hst_chaining_t *create_hash_table_chaining(int size)
{
    hst_chaining_t *ht = (hst_chaining_t *)malloc(sizeof(hst_chaining_t));
    if (!ht)
        return NULL;

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
    if (!ht)
        return;

    for (int i = 0; i < ht->size; i++)
    {
        hash_node_t *current = ht->table[i];
        while (current)
        {
            hash_node_t *temp = current;
            current = current->next;
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

unsigned int hash_function(const char *word, int table_size)
{
    if (current_hash_func == 1)
        return hash_function1(word, table_size);
    else
        return hash_function2(word, table_size);
}

void hash_table_insert_chaining(hst_chaining_t *ht, const char *word)
{
    if (!ht || !word)
        return;

    if ((double)(ht->count + 1) / ht->size > MAX_LOAD_FACTOR)
    {
        printf("Загруженность таблицы превышена (%.2f > %.2f). Выполняется рехеширование...\n",
               (double)ht->count / ht->size, MAX_LOAD_FACTOR);
        rehash_chaining(ht);
    }

    unsigned int index = hash_function(word, ht->size);
    hash_node_t *current = ht->table[index];
    int chain_length = 0;

    while (current)
    {
        chain_length++;
        if (strcmp(current->word, word) == 0)
        {
            current->count++;
            return;
        }
        current = current->next;
    }

    if (chain_length >= 4)
    {
        printf("Длина цепочки %d превышает лимит. Пробуем другую хэш-функцию...\n", chain_length);
        current_hash_func = (current_hash_func == 1) ? 2 : 1;
        rehash_chaining(ht);
        hash_table_insert_chaining(ht, word);
        return;
    }

    hash_node_t *new_node = malloc(sizeof(hash_node_t));
    if (!new_node)
        return;

    new_node->word = str_dynamic_copy(word);   
    new_node->count = 1;
    new_node->is_deleted = false;

    new_node->next = ht->table[index];
    if (ht->table[index] != NULL)
        ht->collisions++;

    ht->table[index] = new_node;
    ht->count++;

    if ((double)ht->count / ht->size > MAX_LOAD_FACTOR)
    {
        printf("Загруженность превышена после вставки (%.2f > %.2f). Выполняется рехеширование...\n",
               (double)ht->count / ht->size, MAX_LOAD_FACTOR);
        rehash_chaining(ht);
    }
}


hash_node_t *hash_table_search_chaining(hst_chaining_t *ht, const char *word, int *comparisons)
{
    if (!ht)
        return NULL;
    unsigned int index = hash_function(word, ht->size);
    hash_node_t *current = ht->table[index];
    *comparisons = 0;
    while (current)
    {
        (*comparisons)++;
        ht->total_comparisons++;

        if (!current->is_deleted && (strcmp(current->word, word) == 0))
        {
            ht->searches_count++;

            if (*comparisons > 4)
            {
                printf("Для поиска потребовалось %d сравнений (>4). Меняем хэш-функцию...\n", *comparisons);
                if (current_hash_func == 1)
                    current_hash_func = 2;
                else
                    current_hash_func = 1;
                rehash_chaining(ht);
            }

            return current;
        }
        current = current->next;
    }
    ht->searches_count++;
    if (*comparisons == 0)
        *comparisons = 1;
    return NULL;
}

void hash_table_delete_chaining(hst_chaining_t *ht, const char *word)
{
    if (!ht)
        return;

    unsigned int index = hash_function(word, ht->size);
    hash_node_t *current = ht->table[index];
    hash_node_t *prev = NULL;

    if (!current)
    {
        index = (current_hash_func == 1) ? hash_function2(word, ht->size) : hash_function1(word, ht->size);
        current = ht->table[index];
    }

    while (current)
    {
        if (!current->is_deleted && (strcmp(current->word, word) == 0))
        {
            if (prev == NULL)
            {

                ht->table[index] = current->next;
            }
            else
            {

                prev->next = current->next;
            }

            current->is_deleted = true;
            ht->count--;
            return;
        }
        prev = current;
        current = current->next;
    }
}

void display_hash_table_chaining(hst_chaining_t *ht)
{
    if (!ht)
    {
        printf("Хеш-таблица не инициализирована!\n");
        return;
    }

    printf("\n=== Хеш-таблица с цепочками (размер: %d) ===\n", ht->size);
    printf("Индекс | Слова\n");
    printf("-------|--------------------------------------------------\n");

    for (int i = 0; i < ht->size; i++)
    {
        if (ht->table[i] != NULL)
        {
            printf("%6d | ", i);

            hash_node_t *current = ht->table[i];
            bool first = true;
            int chain_length = 0;

            while (current)
            {
                if (!current->is_deleted)
                {
                    if (!first)
                        printf(" -> ");
                    printf("%s (%d)", current->word, current->count);
                    first = false;
                    chain_length++;
                }
                current = current->next;
            }

            if (chain_length > 1)
            {
                printf(" [длина цепочки: %d]", chain_length);
            }

            printf("\n");
        }
    }
    printf("--------------------------------------------------------\n");
}

void hash_table_statistics_chaining(hst_chaining_t *ht)
{
    if (!ht)
        return;

    printf("\n=== Статистика хеш-таблицы с цепочками ===\n");
    printf("Размер таблицы: %d\n", ht->size);
    printf("Количество элементов: %d\n", ht->count);
    printf("Загруженность: %.2f%%\n", (double)ht->count / ht->size * 100);
    printf("Количество коллизий: %d\n", ht->collisions);
    printf("Средняя длина цепочки: %.2f\n", (double)ht->count / ht->size);
    printf("Всего сравнений при поиске: %d\n", ht->total_comparisons);
    printf("Количество поисков: %d\n", ht->searches_count);
    printf("Среднее количество сравнений на поиск: %.2f\n", get_average_comparisons_chaining(ht));

    int empty_cells = 0;
    int max_chain_length = 0;
    for (int i = 0; i < ht->size; i++)
    {
        int chain_length = 0;
        hash_node_t *current = ht->table[i];

        while (current)
        {
            if (!current->is_deleted)
                chain_length++;
            current = current->next;
        }

        if (chain_length == 0)
            empty_cells++;
        if (chain_length > max_chain_length)
            max_chain_length = chain_length;
    }

    printf("Пустых ячеек: %d (%.2f%%)\n", empty_cells, (double)empty_cells / ht->size * 100);
    printf("Максимальная длина цепочки: %d\n", max_chain_length);
    printf("===========================================\n");
}

double calculate_load_factor_chaining(hst_chaining_t *ht)
{
    return (double)ht->count / ht->size;
}

void rehash_chaining(hst_chaining_t *ht)
{
    int old_size = ht->size;
    hash_node_t **old_table = ht->table;

    int new_size = (int)(old_size * 1.3);
    while (new_size % 2 == 0 || new_size % 3 == 0)
    {
        new_size++;
    }

    ht->size = new_size;
    ht->table = (hash_node_t **)calloc(new_size, sizeof(hash_node_t *));
    ht->count = 0;
    ht->collisions = 0;

    for (int i = 0; i < old_size; i++)
    {
        hash_node_t *current = old_table[i];
        while (current)
        {
            if (!current->is_deleted)
                hash_table_insert_chaining(ht, current->word);

            hash_node_t *temp = current;
            current = current->next;
            free(temp);
        }
    }

    free(old_table);
    printf("Рехеширование завершено. Новый размер: %d\n", ht->size);
}

hst_open_t *create_hash_table_open(int size)
{
    hst_open_t *ht = (hst_open_t *)malloc(sizeof(hst_open_t));
    if (!ht)
        return NULL;

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
    if (!ht)
        return;

    if (ht->keys)
    {
        for (int i = 0; i < ht->size; i++)
        {
            if (ht->keys[i])          
                free(ht->keys[i]);
        }
    }

    free(ht->keys);
    free(ht->counts);
    free(ht->occupied);
    free(ht->deleted);
    free(ht);
}


void hash_table_insert_open(hst_open_t *ht, const char *word)
{
    if (!ht)
        return;

    if ((double)ht->count / ht->size > MAX_LOAD_FACTOR)
    {
        printf("Загруженность таблицы превышена (%.2f > %.2f). Выполняется рехеширование...\n",
               (double)ht->count / ht->size, MAX_LOAD_FACTOR);
        rehash_open(ht);
    }

    int index = hash_function(word, ht->size);
    int original_index = index;
    int probe_count = 0;

    while (ht->occupied[index] && !ht->deleted[index])
    {
        if (strcmp(ht->keys[index], word) == 0)
        {
            ht->counts[index]++;
            return;
        }

        probe_count++;

        if (probe_count > 4)
        {
            printf("Для вставки потребовалось %d проб (>4). Меняем хэш-функцию...\n", probe_count);
            if (current_hash_func == 1)
                current_hash_func = 2;
            else
                current_hash_func = 1;
            rehash_open(ht);
            hash_table_insert_open(ht, word);
            return;
        }

        index = (index + 1) % ht->size;
        if (index == original_index)
        {
            printf("Хеш-таблица переполнена!\n");
            rehash_open(ht);
            hash_table_insert_open(ht, word);
            return;
        }
    }

    ht->keys[index] = str_dynamic_copy(word);
    ht->counts[index] = 1;
    ht->occupied[index] = true;
    ht->deleted[index] = false;
    ht->count++;

    if (probe_count > 0)
        ht->collisions++;
}

int hash_table_search_open(hst_open_t *ht, const char *word, int *comparisons)
{
    if (!ht)
        return -1;

    int index = hash_function(word, ht->size);
    int original_index = index;
    *comparisons = 0;

    while (ht->occupied[index] || ht->deleted[index])
    {
        (*comparisons)++;
        ht->total_comparisons++;

        if (ht->occupied[index] && !ht->deleted[index] && (strcmp(ht->keys[index], word) == 0))
        {
            ht->searches_count++;

            if (*comparisons > 4)
            {
                printf("Для поиска потребовалось %d сравнений (>4). Меняем хэш-функцию...\n", *comparisons);
                if (current_hash_func == 1)
                    current_hash_func = 2;
                else
                    current_hash_func = 1;
                rehash_open(ht);
            }

            return index;
        }

        index = (index + 1) % ht->size;
        if (index == original_index)
            break;
    }

    if (*comparisons == 0)
        *comparisons = 1;

    ht->searches_count++;
    return -1;
}

void hash_table_delete_open(hst_open_t *ht, const char *word)
{
    if (!ht)
        return;

    int comparisons;
    int index = hash_table_search_open(ht, word, &comparisons);

    if (index != -1)
    {
        ht->deleted[index] = true;
        ht->occupied[index] = false;
        ht->count--;
    }
}

void display_hash_table_open(hst_open_t *ht)
{
    if (!ht)
    {
        printf("Хеш-таблица не инициализирована!\n");
        return;
    }

    printf("\n=== Хеш-таблица с открытой адресацией (размер: %d) ===\n", ht->size);
    printf("Индекс | Состояние      | Слово          | Количество\n");
    printf("-------|----------------|----------------|------------\n");

    for (int i = 0; i < ht->size; i++)
    {
        printf("%6d | ", i);

        if (!ht->occupied[i] && !ht->deleted[i])
        {
            printf("Свободно       |                | \n");
        }
        else if (ht->deleted[i])
        {
            printf("Удалено        |                | \n");
        }
        else
        {
            printf("Занято         | %-14s | %d\n", ht->keys[i], ht->counts[i]);
        }
    }
    printf("----------------------------------------------------\n");
}

void hash_table_statistics_open(hst_open_t *ht)
{
    if (!ht)
        return;

    printf("\n=== Статистика хеш-таблицы с открытой адресацией ===\n");
    printf("Размер таблицы: %d\n", ht->size);
    printf("Количество элементов: %d\n", ht->count);
    printf("Загруженность: %.2f%%\n", (double)ht->count / ht->size * 100);
    printf("Количество коллизий: %d\n", ht->collisions);
    printf("Всего сравнений при поиске: %d\n", ht->total_comparisons);
    printf("Количество поисков: %d\n", ht->searches_count);
    printf("Среднее количество сравнений на поиск: %.2f\n", get_average_comparisons_open(ht));

    int clusters = 0;
    int max_cluster = 0;
    int current_cluster = 0;

    for (int i = 0; i < ht->size; i++)
    {
        if (ht->occupied[i] && !ht->deleted[i])
        {
            current_cluster++;
        }
        else
        {
            if (current_cluster > 0)
            {
                clusters++;
                if (current_cluster > max_cluster)
                {
                    max_cluster = current_cluster;
                }
                current_cluster = 0;
            }
        }
    }

    if (current_cluster > 0)
    {
        clusters++;
        if (current_cluster > max_cluster)
        {
            max_cluster = current_cluster;
        }
    }

    printf("Количество кластеров: %d\n", clusters);
    printf("Максимальный размер кластера: %d\n", max_cluster);
    printf("Средний размер кластера: %.2f\n", clusters > 0 ? (double)ht->count / clusters : 0);
    printf("====================================================\n");
}

double calculate_load_factor_open(hst_open_t *ht)
{
    return (double)ht->count / ht->size;
}

void rehash_open(hst_open_t *ht)
{
    int old_size = ht->size;
    char **old_keys = ht->keys;
    int *old_counts = ht->counts;
    bool *old_occupied = ht->occupied;

    int new_size = (int)(old_size * 1.3);
    while (new_size % 2 == 0 || new_size % 3 == 0)
    {
        new_size++;
    }

    ht->size = new_size;
    ht->keys = (char **)calloc(new_size, sizeof(char *));
    ht->counts = (int *)calloc(new_size, sizeof(int));
    ht->occupied = (bool *)calloc(new_size, sizeof(bool));
    free(ht->deleted);
    ht->deleted = (bool *)calloc(new_size, sizeof(bool));
    ht->count = 0;
    ht->collisions = 0;

    for (int i = 0; i < old_size; i++)
    {
        if (old_occupied[i] && old_keys[i])
        {
            for (int j = 0; j < old_counts[i]; j++)
            {
                hash_table_insert_open(ht, old_keys[i]);
            }
        }
    }

    free(old_keys);
    free(old_counts);
    free(old_occupied);

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

    while (fscanf(file, "%s", buffer) == 1)   /* лучше "%<N>s" */
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

    while (fscanf(file, "%s", buffer) == 1)   /* можно "%<N>s" */
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