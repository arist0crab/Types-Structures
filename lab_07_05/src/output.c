#include "../inc/output.h"

void print_menu(void)
{
    printf("%s", BLUE);
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║                       МЕНЮ                       ║\n");
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║ 0  - Выйти из программы                          ║\n");
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║                ДЕРЕВЬЯ И ТАБЛИЦЫ                 ║\n");
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║ 1  - Считать дерево из файла (bst и avl)         ║\n");
    printf("║ 2  - Распечатать bst дерево в виде дерева        ║\n");
    printf("║ 3  - Распечатать avl дерево в виде дерева        ║\n");
    printf("║ 4  - Распечатать bst дерево в виде картинки      ║\n");
    printf("║ 5  - Распечатать avl дерево в виде картинки      ║\n");
    printf("║ 6  - Добавить слово в дерево                     ║\n");
    printf("║ 7  - Удалить слово из дерева                     ║\n");
    printf("║ 8  - Найти слово в дереве                        ║\n");
    printf("║ 9  - Считать хэш-таблицу из файла                ║\n");
    printf("║      (с цепочками и открытой адресацией)         ║\n");
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║                      ВЫВОД                       ║\n");
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║ 10  - Распечатать chaining таблицу               ║\n");
    printf("║ 11  - Распечатать таблицу с открытой адресацией  ║\n");
    printf("║ 12  - Показать картинку chaining таблицы         ║\n");
    printf("║ 13  - Показать картинку open таблицы             ║\n");
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║                РАБОТА СО СЛОВАМИ                 ║\n");
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║ 14  - Добавить слово в таблицы                   ║\n");
    printf("║ 15  - Удалить слово из таблиц                    ║\n");
    printf("║ 16  - Найти слово в таблицах                     ║\n");
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║                   ЭФФЕКТИВНОСТЬ                  ║\n");
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║ 17 - Провести оценку эффективности поиска        ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");    
    printf("%s", RESET);
}

void print_result(const status_t exit_code)
{
    switch (exit_code)
    {
        case SUCCESS_CODE:
            printf(GREEN_BOLD "Успех!\n" RESET);
            break;

        case INPUT_ERR_CODE:
            printf(RED_BOLD "Ошибка ввода!\n" RESET);
            break;

        case INVALID_PTR_CODE:
            printf(RED_BOLD "Ошибка при работе с указателем!\n" RESET);
            break;

        case MEMORY_ERR_CODE:
            printf(RED_BOLD "Ошибка при работе с памятью!\n" RESET);
            break;

        case FILE_OPEN_ERR_CODE:
            printf(RED_BOLD "Ошибка при открытии файла!\n" RESET);
            break;
        
        case NOTHING_TO_DELETE_CODE:
            printf(RED_BOLD "Нечего удалять, дерево пусто!\n" RESET);
            break;
        
        default:
            break;
    }
}

void print_compare_table(double average_tree_time_ns, double average_file_time_ns)
{
    printf("%s", BLUE);
    printf("╔═════════════════════════════════════════════╗\n");
    printf("║             РЕЗУЛЬТАТЫ СРАВНЕНИЯ            ║\n");
    printf("╠══════════════════════╦══════════════════════╣\n");
    printf("║     дерево, nsec     ║      файл, nsec      ║\n");
    printf("╠══════════════════════╬══════════════════════╣\n");
    printf("║  %18.2lf  ║  %18.2lf  ║\n", average_tree_time_ns, average_file_time_ns);
    printf("╚══════════════════════╩══════════════════════╝\n");
    printf("%s", RESET);  
}

void print_compare_trees_table(double average_balanced_time_ns, double average_degenerate_time_ns, double average_random_time_ns)
{
    printf("%s", BLUE);
    printf("╔══════════════════════════════════════════════════════════════════════╗\n");
    printf("║                          РЕЗУЛЬТАТЫ СРАВНЕНИЯ                        ║\n");
    printf("╠════════════════════════╦═══════════════════════╦═════════════════════╣\n");
    printf("║ сбалансированное, nsec ║   вырожденное, nsec   ║   рандомное, nsec   ║\n");
    printf("╠════════════════════════╬═══════════════════════╬═════════════════════╣\n");
    printf("║  %20.2lf  ║  %19.2lf  ║ %19.2lf ║\n", average_balanced_time_ns, average_degenerate_time_ns, average_random_time_ns);
    printf("╚════════════════════════╩═══════════════════════╩═════════════════════╝\n"); 
    printf("%s", RESET); 
}

void print_compare_trees_table_2(double average_balanced_time_ns, double average_degenerate_time_ns, double average_random_time_ns)
{
    printf("+======================================================================+\n");
    printf("|                          РЕЗУЛЬТАТЫ СРАВНЕНИЯ                        |\n");
    printf("+======================================================================+\n");
    printf("| сбалансированное, nsec |   вырожденное, nsec   |   рандомное, nsec   |\n");
    printf("|======================================================================|\n");
    printf("|  %20.2lf  |  %19.2lf  | %19.2lf |\n", average_balanced_time_ns, average_degenerate_time_ns, average_random_time_ns);
    printf("+======================================================================+\n");
}

status_t print_chaining_hash_table(const hst_chaining_t *table)
{
    status_t exit_code = SUCCESS_CODE;
    hash_node_t *current = NULL;
    bool first_in_chain = true;

    if (!table)
        exit_code = INVALID_PTR_CODE;
    else if (table->count == 0)
        printf("Таблица пуста!\n");  
    else if (exit_code == SUCCESS_CODE)
    {
        printf("%s", BLUE);
        printf("╔═════════════════════════════════════════════════════════════╗\n");
        printf("║                   Хэш-таблица (цепочки)                     ║\n");
        printf("╠════════════════╦═══════════════════╦════════════════════════╣\n");
        printf("║ Размер: %6d ║ Элементов: %6d ║ Загруженность: %3.1f%%   ║\n", table->size, table->count, (double)table->count / table->size * 100);
        printf("╠════════════════╬═══════════════════╬════════════════════════╣\n");

        for (int i = 0; exit_code == SUCCESS_CODE && i < table->size; i++)
        {
            if (table->table[i])
            {                
                current = table->table[i];
                first_in_chain = true;
                
                printf("║ %10d ║ ", i);
                while (current)
                {
                    if (!first_in_chain)
                        printf(" -> ");
                    
                    printf("%s (%d)", current->word, current->count);
                    current = current->next;
                    first_in_chain = false;
                }
                printf("\n");
            }
        }
        printf("╚═════════════════════════════════════════════════════════════╝\n");
        printf("%s", RESET);
    }

    return exit_code;
}

status_t print_open_hash_table(const hst_open_t *table)
{
    status_t exit_code = SUCCESS_CODE;

    if (!table)
        exit_code = INVALID_PTR_CODE;
    else if (table->count == 0)
        printf("Таблица пуста!\n");
    else if (exit_code == SUCCESS_CODE)
    {
        printf("%s", BLUE);
        printf("╔═════════════════════════════════════════════════════════════╗\n");
        printf("║            Хэш-таблица (открытая адресация)                 ║\n");
        printf("╠════════════════╦═══════════════════╦════════════════════════╣\n");
        printf("║ Размер: %6d ║ Элементов: %6d ║ Загруженность: %3.1f%%   ║\n",
               table->size, table->count,
               (double)table->count / table->size * 100);
        printf("╠════════════════╬═══════════════════╬════════════════════════╣\n");

        for (int i = 0; exit_code == SUCCESS_CODE && i < table->size; i++)
        {
            printf("| %10d | ", i);

            if (table->occupied[i] && !table->deleted[i])
                printf("%s (%d)", table->keys[i], table->counts[i]);
            else if (table->deleted[i])
                printf("<deleted>");
            else
                printf("<empty>");

            printf("\n");
        }

        printf("╚═════════════════════════════════════════════════════════════╝\n");
        printf("%s", RESET);
    }

    return exit_code;
}
