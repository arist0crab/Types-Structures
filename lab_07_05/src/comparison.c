#define _POSIX_C_SOURCE 199309L
#include "../inc/comparison.h"
#include <sys/time.h>
#include <time.h>

long long get_time_ns()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)ts.tv_sec * 1000000000LL + ts.tv_nsec;
}

size_t calculate_bst_memory(bst_node_t *root) {
    if (!root) return 0;
    
    size_t total = sizeof(bst_node_t);
    total += strlen(root->word) + 1;
    total += calculate_bst_memory(root->left);
    total += calculate_bst_memory(root->right);
    
    return total;
}

size_t calculate_avl_memory(avl_node_t *root) {
    if (!root) return 0;
    
    size_t total = sizeof(avl_node_t);
    total += strlen(root->word) + 1;
    total += calculate_avl_memory(root->left);
    total += calculate_avl_memory(root->right);
    
    return total;
}

size_t calculate_hash_chaining_memory(hst_chaining_t *ht)
{
    if (!ht) return 0;
    
    size_t memory = sizeof(hst_chaining_t);         
    memory += ht->size * sizeof(hash_node_t*);     
    
    for (int i = 0; i < ht->size; i++) {
        hash_node_t *current = ht->table[i];
        while (current) {
            if (!current->is_deleted) {
                memory += sizeof(hash_node_t);
                memory += strlen(current->word) + 1;  
            }
            current = current->next;
        }
    }
    
    return memory;
}


size_t calculate_hash_open_memory(hst_open_t *ht)
{
    if (!ht) return 0;
    
    size_t memory = sizeof(hst_open_t);
    
    memory += ht->size * sizeof(char*);   
    memory += ht->size * sizeof(int);     
    memory += ht->size * sizeof(bool);    
    memory += ht->size * sizeof(bool);   
    
    for (int i = 0; i < ht->size; i++)
        if (ht->keys[i])
            memory += strlen(ht->keys[i]) + 1;
    
    return memory;
}



void collect_words(bst_node_t *root, char **words, int *idx)
{
    if (root == NULL)
        return;

    collect_words(root->left, words, idx);
    words[*idx] = root->word;
    (*idx)++;
    collect_words(root->right, words, idx);
}

void collect_counts(bst_node_t *root, int *counts, int *idx)
{
    if (root == NULL)
        return;

    collect_counts(root->left, counts, idx);
    counts[*idx] = root->count;
    (*idx)++;
    collect_counts(root->right, counts, idx);
}

comparison_results_t compare_structures(bst_node_t *bst_root, avl_node_t *avl_root, hst_chaining_t *hash_chaining,
                                     hst_open_t *hash_open)
{
    comparison_results_t results = {0};

    if (!bst_root || !avl_root || !hash_chaining || !hash_open)
    {
        printf("Ошибка: не все структуры инициализированы!\n");
        return results;
    }

    int numbers_count = count_nodes_bst(bst_root);
    char **test_words = (char **)malloc(numbers_count * sizeof(char *));
    int index = 0;

    collect_words(bst_root, test_words, &index);

    int total_bst_comparisons = 0;
    int total_avl_comparisons = 0;
    int total_hash_chaining_comparisons = 0;
    int total_hash_open_comparisons = 0;
    int comp = 0;

    long long bst_start_time = get_time_ns();

    for (int i = 0; i < TESTS_QUANTITY; i++)
    {
        comp = 0;
        find_word_in_bst(&bst_root, test_words[i % numbers_count], false, &comp);
        total_bst_comparisons += comp;
    }
    long long bst_end_time = get_time_ns();

    long long avl_start_time = get_time_ns();
    for (int i = 0; i < TESTS_QUANTITY; i++)
    {
        comp = 0;
        find_word_in_avl(&avl_root, test_words[i % numbers_count], false, &comp);
        total_avl_comparisons += comp;
    }
    long long avl_end_time = get_time_ns();

    long long hash_chaining_start_time = get_time_ns();
    for (int i = 0; i < TESTS_QUANTITY; i++)
    {
        int comparisons;
        hash_table_search_chaining(hash_chaining, test_words[i % numbers_count], &comparisons);
        total_hash_chaining_comparisons += comparisons;
    }
    long long hash_chaining_end_time = get_time_ns();

    long long hash_open_start_time = get_time_ns();
    for (int i = 0; i < TESTS_QUANTITY; i++)
    {
        int comparisons;
        hash_table_search_open(hash_open, test_words[i % numbers_count], &comparisons);
        total_hash_open_comparisons += comparisons;
    }
    long long hash_open_end_time = get_time_ns();

    results.bst_search_time = (double)(bst_end_time - bst_start_time) / TESTS_QUANTITY / 1000.0;
    results.avl_search_time = (double)(avl_end_time - avl_start_time) / TESTS_QUANTITY / 1000.0;
    results.hash_chaining_search_time = (double)(hash_chaining_end_time - hash_chaining_start_time) / TESTS_QUANTITY / 1000.0;
    results.hash_open_search_time = (double)(hash_open_end_time - hash_open_start_time) / TESTS_QUANTITY / 1000.0;
    
    results.bst_comparisons = numbers_count > 0 ? total_bst_comparisons / TESTS_QUANTITY : 0;
    results.avl_comparisons = numbers_count > 0 ? total_avl_comparisons / TESTS_QUANTITY : 0;
    results.hash_chaining_comparisons = numbers_count > 0 ? total_hash_chaining_comparisons / TESTS_QUANTITY : 0;
    results.hash_open_comparisons = numbers_count > 0 ? total_hash_open_comparisons / TESTS_QUANTITY : 0;

    results.bst_memory = calculate_bst_memory(bst_root);
    results.avl_memory = calculate_avl_memory(avl_root);
    results.hash_chaining_memory = calculate_hash_chaining_memory(hash_chaining);
    results.hash_open_memory = calculate_hash_open_memory(hash_open);

    results.bst_height = tree_height(bst_root);
    results.avl_height = avl_height(avl_root);
    results.hash_chaining_load_factor = calculate_load_factor_chaining(hash_chaining);
    results.hash_open_load_factor = calculate_load_factor_open(hash_open);

    free(test_words);

    return results;
}

void print_comparison_results(comparison_results_t *results)
{
    printf("%s", BLUE);
    printf("╔══════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                     СРАВНЕНИЕ СТРУКТУР ДАННЫХ                            ║\n");
    printf("╠══════════════════════════════╦══════════╦══════════╦══════════╦══════════╣\n");
    printf("║           Параметр           ║   BST    ║   AVL    ║ Hash(Цеп)║Hash(Откр)║\n");
    printf("╠══════════════════════════════╬══════════╬══════════╬══════════╬══════════╣\n");

    printf("║ Время поиска (nsc)           ║ %8.4f ║ %8.4f ║ %8.4f ║ %8.4f ║\n", results->bst_search_time,
           results->avl_search_time, results->hash_chaining_search_time, results->hash_open_search_time);

    printf("║ Ср. сравнений на поиск       ║ %8d ║ %8d ║ %8d ║ %8d ║\n", results->bst_comparisons,
           results->avl_comparisons, results->hash_chaining_comparisons, results->hash_open_comparisons);

    printf("║ Исп. память (байт)           ║ %8zu ║ %8zu ║ %8zu ║ %8zu ║\n", results->bst_memory, results->avl_memory,
           results->hash_chaining_memory, results->hash_open_memory);

    printf("║ Высота/Загруженность         ║ %8d ║ %8d ║ %7.1f%% ║ %7.1f%% ║\n", results->bst_height,
           results->avl_height, results->hash_chaining_load_factor * 100, results->hash_open_load_factor * 100);

    printf("╚══════════════════════════════╩══════════╩══════════╩══════════╩══════════╝\n");
    printf("%s", RESET);
}

void benchmark_structures(const char *filename, int hash_table_size)
{
    printf("\n=== БЕНЧМАРК СТРУКТУР ДАННЫХ ===\n");
    printf("Файл: %s\n", filename);
    printf("Размер хеш-таблицы: %d\n\n", hash_table_size);

    printf("1. Построение BST дерева...\n");
    bst_node_t *bst_root = NULL;
    make_bst_from_file(&bst_root, filename);

    printf("2. Построение AVL дерева...\n");
    avl_node_t *avl_root = NULL;
    make_avl_from_file(&avl_root, filename);

    printf("3. Построение хеш-таблицы с цепочками...\n");
    hst_chaining_t *hash_chaining = create_hash_table_chaining(hash_table_size);
    build_hash_table_from_file_chaining(hash_chaining, filename);

    printf("4. Построение хеш-таблицы с открытой адресацией...\n");
    hst_open_t *hash_open = create_hash_table_open(hash_table_size);
    build_hash_table_from_file_open(hash_open, filename);

    comparison_results_t results = compare_structures(bst_root, avl_root, hash_chaining, hash_open);

    print_comparison_results(&results);

    free_bst(bst_root);
    free_avl(avl_root);
    free_hash_table_chaining(hash_chaining);
    free_hash_table_open(hash_open);
}
