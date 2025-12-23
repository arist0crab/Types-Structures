#include "../inc/visualization.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#define OPEN_COMMAND "start"
#elif __APPLE__
#define OPEN_COMMAND "open"
#else
#define OPEN_COMMAND "xdg-open"
#endif


void generate_dot_for_bst(bst_node_t *root, FILE *dot_file, int *node_id)
{
    if (root == NULL)
        return;

    int current_id = (*node_id)++;

    fprintf(dot_file, "  node%d [label=\"%s (%d)\", shape=circle, style=filled, fillcolor=lightblue];\n",
            current_id, root->word, root->count);

    if (root->left != NULL)
    {
        int left_id = *node_id;
        generate_dot_for_bst(root->left, dot_file, node_id);
        fprintf(dot_file, "  node%d -> node%d [color=blue];\n", current_id, left_id);
    }
    else
    {
        int null_id = (*node_id)++;
        fprintf(dot_file, "  null%d [label=\"\", shape=point];\n", null_id);
        fprintf(dot_file, "  node%d -> null%d [color=gray, style=dashed];\n", current_id, null_id);
    }

    if (root->right != NULL)
    {
        int right_id = *node_id;
        generate_dot_for_bst(root->right, dot_file, node_id);
        fprintf(dot_file, "  node%d -> node%d [color=red];\n", current_id, right_id);
    }
    else
    {
        int null_id = (*node_id)++;
        fprintf(dot_file, "  null%d [label=\"\", shape=point];\n", null_id);
        fprintf(dot_file, "  node%d -> null%d [color=gray, style=dashed];\n", current_id, null_id);
    }
}

void visualize_bst_tree(bst_node_t *root, const char *output_filename)
{
    if (root == NULL)
    {
        printf("BST дерево пусто!\n");
        return;
    }

    char dot_filename[256];
    char png_filename[256];

    if (output_filename == NULL)
    {
        snprintf(dot_filename, sizeof(dot_filename), "bst_tree.dot");
        snprintf(png_filename, sizeof(png_filename), "bst_tree.png");
    }
    else
    {
        snprintf(dot_filename, sizeof(dot_filename), "%s.dot", output_filename);
        snprintf(png_filename, sizeof(png_filename), "%s.png", output_filename);
    }

    FILE *dot_file = fopen(dot_filename, "w");
    if (!dot_file)
    {
        printf("Ошибка создания файла %s\n", dot_filename);
        return;
    }

    fprintf(dot_file, "digraph BST {\n");
    fprintf(dot_file, "  graph [ranksep=0.5, nodesep=0.5];\n");
    fprintf(dot_file, "  node [fontname=\"Arial\", fontsize=10];\n");
    fprintf(dot_file, "  edge [fontname=\"Arial\", fontsize=9];\n");

    int node_id = 0;
    generate_dot_for_bst(root, dot_file, &node_id);

    fprintf(dot_file, "  labelloc=\"t\";\n");
    fprintf(dot_file, "  label=\"Binary Search Tree\\nВысота: %zu\";\n",
            tree_height(root));
    fprintf(dot_file, "}\n");

    fclose(dot_file);

    char command[1024];
    snprintf(command, sizeof(command), "dot -Tpng -Gdpi=150 \"%s\" -o \"%s\"", dot_filename, png_filename);

    printf("Генерация изображения BST дерева...\n");
    printf("Команда: %s\n", command);

    int result = system(command);

    if (result == 0)
    {
        printf("Изображение создано: %s\n", png_filename);

        snprintf(command, sizeof(command), "%s \"%s\"", OPEN_COMMAND, png_filename);
        system(command);
    }
    else
    {
        printf("Ошибка генерации изображения. Убедитесь, что Graphviz установлен.\n");
        printf("Установите Graphviz: brew install graphviz (macOS) или apt-get install graphviz (Linux)\n");
    }
}

void generate_dot_for_avl(avl_node_t *root, FILE *dot_file, int *node_id)
{
    if (root == NULL)
        return;

    int current_id = (*node_id)++;

    const char *fillcolor;
    int balance = root->height;
    if (balance > 2)
    {
        fillcolor = "lightcoral";
    }
    else if (balance == 1)
    {
        fillcolor = "lightgreen";
    }
    else
    {
        fillcolor = "lightblue";
    }
    // fprintf(stderr, "dot node: %d h=%d\n", root->number, root->height);


    fprintf(dot_file, "  node%d [label=\"%s (%d)\\nh: %d\", shape=circle, style=filled, fillcolor=%s];\n",
            current_id, root->word, root->count, root->height, fillcolor);

    if (root->left != NULL)
    {
        int left_id = *node_id;
        generate_dot_for_avl(root->left, dot_file, node_id);
        fprintf(dot_file, "  node%d -> node%d [color=darkgreen];\n", current_id, left_id);
    }
    else
    {
        int null_id = (*node_id)++;
        fprintf(dot_file, "  null%d [label=\"\", shape=point];\n", null_id);
        fprintf(dot_file, "  node%d -> null%d [color=gray, style=dashed];\n", current_id, null_id);
    }

    if (root->right != NULL)
    {
        int right_id = *node_id;
        generate_dot_for_avl(root->right, dot_file, node_id);
        fprintf(dot_file, "  node%d -> node%d [color=darkred];\n", current_id, right_id);
    }
    else
    {
        int null_id = (*node_id)++;
        fprintf(dot_file, "  null%d [label=\"\", shape=point];\n", null_id);
        fprintf(dot_file, "  node%d -> null%d [color=gray, style=dashed];\n", current_id, null_id);
    }
}

void visualize_avl_tree(avl_node_t *root, const char *output_filename)
{
    if (root == NULL)
    {
        printf("AVL дерево пусто!\n");
        return;
    }

    char dot_filename[256];
    char png_filename[256];

    if (output_filename == NULL)
    {
        snprintf(dot_filename, sizeof(dot_filename), "avl_tree.dot");
        snprintf(png_filename, sizeof(png_filename), "avl_tree.png");
    }
    else
    {
        snprintf(dot_filename, sizeof(dot_filename), "%s.dot", output_filename);
        snprintf(png_filename, sizeof(png_filename), "%s.png", output_filename);
    }

    FILE *dot_file = fopen(dot_filename, "w");
    if (!dot_file)
    {
        printf("Ошибка создания файла %s\n", dot_filename);
        return;
    }

    fprintf(dot_file, "digraph AVL {\n");
    fprintf(dot_file, "  graph [ranksep=0.5, nodesep=0.5];\n");
    fprintf(dot_file, "  node [fontname=\"Arial\", fontsize=10];\n");
    fprintf(dot_file, "  edge [fontname=\"Arial\", fontsize=9];\n");

    int node_id = 0;
    generate_dot_for_avl(root, dot_file, &node_id);

    fprintf(dot_file, "  labelloc=\"t\";\n");
    fprintf(dot_file, "  label=\"AVL Tree (Balanced Binary Tree)\\ Высота: %zu\";\n", avl_height(root));
    fprintf(dot_file, "}\n");

    fclose(dot_file);

    char command[1024];
    snprintf(command, sizeof(command), "dot -Tpng -Gdpi=150 \"%s\" -o \"%s\"", dot_filename, png_filename);

    printf("Генерация изображения AVL дерева...\n");
    int result = system(command);

    if (result == 0)
    {
        printf("Изображение создано: %s\n", png_filename);

        snprintf(command, sizeof(command), "%s \"%s\"", OPEN_COMMAND, png_filename);
        system(command);
    }
    else
    {
        printf("Ошибка генерации изображения. Убедитесь, что Graphviz установлен.\n");
    }
}

void visualize_hash_table_chaining(hst_chaining_t *ht, const char *output_filename)
{
    if (!ht)
    {
        printf("Хеш-таблица с цепочками не инициализирована!\n");
        return;
    }

    char dot_filename[256];
    char png_filename[256];

    if (output_filename == NULL)
    {
        snprintf(dot_filename, sizeof(dot_filename), "hash_html_table.dot");
        snprintf(png_filename, sizeof(png_filename), "hash_html_table.png");
    }
    else
    {
        snprintf(dot_filename, sizeof(dot_filename), "%s.dot", output_filename);
        snprintf(png_filename, sizeof(png_filename), "%s.png", output_filename);
    }

    FILE *dot_file = fopen(dot_filename, "w");
    if (!dot_file)
    {
        printf("Ошибка создания файла %s\n", dot_filename);
        return;
    }

    printf("Создание HTML-таблицы для хеш-таблицы (%d ячеек)...\n", ht->size);

    fprintf(dot_file, "digraph HashTableHTML {\n");
    fprintf(dot_file, "  node [shape=plaintext];\n");
    fprintf(dot_file, "  rankdir=LR;\n");

    fprintf(dot_file, "  hash_table [label=<\n");
    fprintf(dot_file, "    <TABLE BORDER=\"1\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPADDING=\"3\">\n");

    fprintf(dot_file, "    <TR>\n");
    fprintf(dot_file, "      <TD COLSPAN=\"10\" BGCOLOR=\"lightblue\" ALIGN=\"CENTER\">\n");
    fprintf(dot_file, "        <B>ХЕШ-ТАБЛИЦА С ЦЕПОЧКАМИ</B><BR/>\n");
    fprintf(dot_file, "        Размер: %d | Элементов: %d | Загруженность: %.1f%%\n", ht->size, ht->count,
            (double)ht->count / ht->size * 100);
    fprintf(dot_file, "      </TD>\n");
    fprintf(dot_file, "    </TR>\n");

    int cols_per_row = 10;
    int rows = (ht->size + cols_per_row - 1) / cols_per_row;

    for (int row = 0; row < rows; row++)
    {
        fprintf(dot_file, "    <TR>\n");

        for (int col = 0; col < cols_per_row; col++)
        {
            int idx = row * cols_per_row + col;

            if (idx < ht->size)
            {

                int chain_length = 0;
                hash_node_t *current = ht->table[idx];
                // size_t chain_length = 0;

                // while (current) {
                //     chain_length++;
                //     current = current->next;
                // }
                while (current)
                {
                    if (!current->is_deleted)
                        chain_length++;
                    current = current->next;
                }

                const char *bgcolor;
                if (chain_length == 0)
                {
                    bgcolor = "white";
                }
                else if (chain_length == 1)
                {
                    bgcolor = "lightgreen";
                }
                else if (chain_length <= 3)
                {
                    bgcolor = "gold";
                }
                else if (chain_length <= 5)
                {
                    bgcolor = "orange";
                }
                else
                {
                    bgcolor = "lightcoral";
                }

                fprintf(dot_file, "      <TD BGCOLOR=\"%s\" VALIGN=\"TOP\">\n", bgcolor);
                fprintf(dot_file, "        <TABLE BORDER=\"0\" CELLBORDER=\"0\" CELLSPACING=\"0\">\n");

                fprintf(dot_file, "          <TR><TD ALIGN=\"CENTER\"><B>[%d]</B></TD></TR>\n", idx);

                if (chain_length > 0)
                {

                    fprintf(dot_file, "          <TR><TD ALIGN=\"CENTER\">Цепочка: %d</TD></TR>\n", chain_length);

                    fprintf(dot_file, "          <TR><TD>\n");
                    fprintf(dot_file,
                            "            <TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPADDING=\"1\">\n");

                    current = ht->table[idx];
                    int element_count = 0;
                    int max_elements_to_show = 10;

                    while (current && element_count < max_elements_to_show)
                    {
                        if (!current->is_deleted)
                        {
                            fprintf(dot_file, "              <TR>\n");
                            fprintf(dot_file, "                <TD BGCOLOR=\"%s\" ALIGN=\"CENTER\">\n",
                                    (element_count % 2 == 0) ? "aliceblue" : "lavender");
                            fprintf(dot_file, "                  <FONT POINT-SIZE=\"8\">%s (%d)</FONT><BR/>\n",
                                    current->word, current->count);
                            // FIXME: ???
                            // fprintf(dot_file, "                  <FONT POINT-SIZE=\"7\">%zu</FONT>\n", current->tail_length);
                            fprintf(dot_file, "                </TD>\n");
                            fprintf(dot_file, "              </TR>\n");
                            element_count++;
                        }
                        current = current->next;
                    }

                    if (current != NULL)
                    {
                        int remaining = chain_length - max_elements_to_show;
                        fprintf(dot_file, "              <TR>\n");
                        fprintf(dot_file, "                <TD BGCOLOR=\"lightgray\" ALIGN=\"CENTER\">\n");
                        fprintf(dot_file, "                  <FONT POINT-SIZE=\"6\">... еще %d</FONT>\n", remaining);
                        fprintf(dot_file, "                </TD>\n");
                        fprintf(dot_file, "              </TR>\n");
                    }

                    fprintf(dot_file, "            </TABLE>\n");
                    fprintf(dot_file, "          </TD></TR>\n");
                }
                else
                {

                    fprintf(dot_file, "          <TR><TD ALIGN=\"CENTER\">--- ПУСТО ---</TD></TR>\n");
                }

                fprintf(dot_file, "        </TABLE>\n");
                fprintf(dot_file, "      </TD>\n");
            }
            else
            {

                fprintf(dot_file, "      <TD BGCOLOR=\"gray\"></TD>\n");
            }
        }

        fprintf(dot_file, "    </TR>\n");
    }

    fprintf(dot_file, "    <TR>\n");
    fprintf(dot_file, "      <TD COLSPAN=\"%d\" BGCOLOR=\"lightyellow\">\n", cols_per_row);
    fprintf(dot_file, "        <TABLE BORDER=\"0\" CELLBORDER=\"0\" WIDTH=\"100%%\">\n");
    fprintf(dot_file, "          <TR>\n");
    fprintf(dot_file, "            <TD WIDTH=\"25%%\" ALIGN=\"CENTER\">\n");
    fprintf(dot_file, "              <TABLE BORDER=\"1\" CELLBORDER=\"1\"><TR><TD BGCOLOR=\"white\">Пустая "
                      "ячейка</TD></TR></TABLE>\n");
    fprintf(dot_file, "            </TD>\n");
    fprintf(dot_file, "            <TD WIDTH=\"25%%\" ALIGN=\"CENTER\">\n");
    fprintf(dot_file, "              <TABLE BORDER=\"1\" CELLBORDER=\"1\"><TR><TD BGCOLOR=\"lightgreen\">1 "
                      "элемент</TD></TR></TABLE>\n");
    fprintf(dot_file, "            </TD>\n");
    fprintf(dot_file, "            <TD WIDTH=\"25%%\" ALIGN=\"CENTER\">\n");
    fprintf(
        dot_file,
        "              <TABLE BORDER=\"1\" CELLBORDER=\"1\"><TR><TD BGCOLOR=\"gold\">2-3 элемента</TD></TR></TABLE>\n");
    fprintf(dot_file, "            </TD>\n");
    fprintf(dot_file, "            <TD WIDTH=\"25%%\" ALIGN=\"CENTER\">\n");
    fprintf(dot_file, "              <TABLE BORDER=\"1\" CELLBORDER=\"1\"><TR><TD BGCOLOR=\"lightcoral\">4+ "
                      "элемента</TD></TR></TABLE>\n");
    fprintf(dot_file, "            </TD>\n");
    fprintf(dot_file, "          </TR>\n");
    fprintf(dot_file, "        </TABLE>\n");
    fprintf(dot_file, "      </TD>\n");
    fprintf(dot_file, "    </TR>\n");

    fprintf(dot_file, "    </TABLE>\n");
    fprintf(dot_file, "  >];\n");

    fprintf(dot_file, "}\n");
    fclose(dot_file);

    printf("HTML таблица создана: %s\n", dot_filename);

    char command[1024];

    int image_width = 1800;
    int image_height = 200 + (rows * 200);

    snprintf(command, sizeof(command), "dot -Tpng -Gsize=%d,%d! -Gdpi=100 \"%s\" -o \"%s\"", image_width, image_height,
             dot_filename, png_filename);

    printf("Генерация изображения %dx%d...\n", image_width, image_height);
    int result = system(command);

    if (result == 0)
    {
        printf("✓ Изображение создано: %s\n", png_filename);

        snprintf(command, sizeof(command), "%s \"%s\"", OPEN_COMMAND, png_filename);
        system(command);
    }
    else
    {
        printf("✗ Ошибка генерации. Попробуйте:\n");
        printf("  dot -Tpng \"%s\" -o \"%s\"\n", dot_filename, png_filename);
    }
}

void visualize_hash_table_open(hst_open_t *ht, const char *output_filename)
{
    if (!ht)
    {
        printf("Хеш-таблица с открытой адресацией не инициализирована!\n");
        return;
    }

    char dot_filename[256];
    char png_filename[256];

    if (output_filename == NULL)
    {
        snprintf(dot_filename, sizeof(dot_filename), "hash_open.dot");
        snprintf(png_filename, sizeof(png_filename), "hash_open.png");
    }
    else
    {
        snprintf(dot_filename, sizeof(dot_filename), "%s.dot", output_filename);
        snprintf(png_filename, sizeof(png_filename), "%s.png", output_filename);
    }

    FILE *dot_file = fopen(dot_filename, "w");
    if (!dot_file)
    {
        printf("Ошибка создания файла %s\n", dot_filename);
        return;
    }

    fprintf(dot_file, "digraph hst_open_t {\n");
    fprintf(dot_file, "  rankdir=TB;\n");
    fprintf(dot_file, "  node [shape=record, fontname=\"Arial\", fontsize=9];\n");

    int cols = 8;
    if (ht->size < 20)
        cols = 5;
    if (ht->size < 10)
        cols = 3;

    int rows = (ht->size + cols - 1) / cols;

    for (int i = 0; i < rows; i++)
    {
        fprintf(dot_file, "  row%d [label=\"", i);

        for (int j = 0; j < cols; j++)
        {
            int idx = i * cols + j;
            if (idx < ht->size)
            {
                if (ht->occupied[idx] && !ht->deleted[idx])
                {

                    char *display_key;
                    display_key = str_dynamic_copy(ht->keys[idx]);

                    fprintf(dot_file, "[%d] %s (%d)", idx, display_key, ht->counts[idx]);
                }
                else if (ht->deleted[idx])
                {
                    fprintf(dot_file, "[%d] DELETED", idx);
                }
                else
                {
                    fprintf(dot_file, "[%d] EMPTY", idx);
                }

                if (j < cols - 1 && idx + 1 < ht->size)
                {
                    fprintf(dot_file, " | ");
                }
            }
        }

        fprintf(dot_file, "\"");

        if (i % 2 == 0)
        {
            fprintf(dot_file, ", style=filled, fillcolor=lightgray");
        }

        fprintf(dot_file, "];\n");
    }

    for (int i = 0; i < rows - 1; i++)
    {
        fprintf(dot_file, "  row%d -> row%d [style=invis];\n", i, i + 1);
    }

    fprintf(dot_file, "  labelloc=\"t\";\n");
    fprintf(dot_file,
            "  label=\"Hash Table with Open Addressing\\nРазмер: %d, Элементов: %d, Коллизий: %d\\nЗагруженность: "
            "%.1f%%\";\n",
            ht->size, ht->count, ht->collisions, (double)ht->count / ht->size * 100);
    fprintf(dot_file, "}\n");

    fclose(dot_file);

    char command[1024];
    snprintf(command, sizeof(command), "dot -Tpng -Gdpi=150 \"%s\" -o \"%s\"", dot_filename, png_filename);

    printf("Генерация изображения хеш-таблицы с открытой адресацией...\n");
    int result = system(command);

    if (result == 0)
    {
        printf("Изображение создано: %s\n", png_filename);

        snprintf(command, sizeof(command), "%s \"%s\"", OPEN_COMMAND, png_filename);
        system(command);
    }
    else
    {
        printf("Ошибка генерации изображения. Убедитесь, что Graphviz установлен.\n");
    }
}

void display_bst_tree_png(bst_node_t *root)
{
    if (root == NULL)
    {
        printf("BST дерево пусто!\n");
        return;
    }

    char filename[256];
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    snprintf(filename, sizeof(filename), "bst_%04d%02d%02d_%02d%02d%02d", tm->tm_year + 1900, tm->tm_mon + 1,
             tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);

    visualize_bst_tree(root, filename);
}

void display_avl_tree_png(avl_node_t *root)
{
    if (root == NULL)
    {
        printf("AVL дерево пусто!\n");
        return;
    }

    char filename[256];
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    snprintf(filename, sizeof(filename), "avl_%04d%02d%02d_%02d%02d%02d", tm->tm_year + 1900, tm->tm_mon + 1,
             tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);

    visualize_avl_tree(root, filename);
}

void display_hash_table_chaining_png(hst_chaining_t *ht)
{
    if (!ht)
    {
        printf("Хеш-таблица с цепочками не инициализирована!\n");
        return;
    }

    char filename[256];
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    snprintf(filename, sizeof(filename), "hash_chain_%04d%02d%02d_%02d%02d%02d", tm->tm_year + 1900, tm->tm_mon + 1,
             tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);

    visualize_hash_table_chaining(ht, filename);
}

void display_hash_table_open_png(hst_open_t *ht)
{
    if (!ht)
    {
        printf("Хеш-таблица с открытой адресацией не инициализирована!\n");
        return;
    }

    char filename[256];
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    snprintf(filename, sizeof(filename), "hash_open_%04d%02d%02d_%02d%02d%02d", tm->tm_year + 1900, tm->tm_mon + 1,
             tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);

    visualize_hash_table_open(ht, filename);
}