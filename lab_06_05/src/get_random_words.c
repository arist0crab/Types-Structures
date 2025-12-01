#include "get_random_words.h"

status_t get_node_by_index(tree_node_t *root, size_t *k, tree_node_t **target_root);

// TODO искать каждое слово в файле, ищем среднее время
// TODO ищем каждое слово в дереве по одному разу и делим время на кол-во вершин

status_t get_random_word_from_file(const char *filename, char **out_word)
{
    status_t ec = SUCCESS_CODE;
    FILE *f = NULL;
    char buf[MAX_STRING_LENGTH];
    char *winner = NULL, *tmp = NULL;
    size_t words_read_qiantity = 0;

    if (!filename || !out_word)
        ec = ERR_ARGS;

    if (ec == SUCCESS_CODE)
    {
        *out_word = NULL;
        ec = safe_open_file((char *)filename, &f);
    }

    for (; ec == SUCCESS_CODE && fscanf(f, "%s", buf) == 1;)
    {
        if (rand() % (++words_read_qiantity) == 0)
        {
            tmp = str_dynamic_copy(buf);
            if (tmp == NULL)
                ec = ERR_MEM;
            else
            {
                free(winner);
                winner = tmp;
            }
        }
    }

    fclose(f);

    if (ec == SUCCESS_CODE && words_read_qiantity == 0)
        ec = ERR_FILE;

    if (ec == SUCCESS_CODE)
        *out_word = winner;
    else
        free(winner);

    return ec;
}

status_t get_random_word_from_tree(tree_node_t *root, char **out_word, size_t words_in_tree_quantity)
{
    status_t ec = SUCCESS_CODE;
    tree_node_t *node = NULL;
    size_t idx;

    if (!root || !out_word || words_in_tree_quantity == 0)
        ec = ERR_ARGS;

    if (ec == SUCCESS_CODE)
        *out_word = NULL;

    if (ec == SUCCESS_CODE)
    {
        idx = (size_t)(rand() % words_in_tree_quantity);
        ec = get_node_by_index(root, &idx, &node);
        if (ec == SUCCESS_CODE && !node)
            ec = ERR_RANGE;
    }

    if (ec == SUCCESS_CODE)
    {
        *out_word = str_dynamic_copy(node->word);
        if (!(*out_word))
            ec = ERR_MEM;
    }

    return ec;
}

// поиск k-го узла в inorder-порядке
status_t get_node_by_index(tree_node_t *root, size_t *k, tree_node_t **target_root)
{
    status_t ec = SUCCESS_CODE;

    if (!root || !k || !target_root)
        ec = ERR_ARGS;
    
    if (ec == SUCCESS_CODE)
        if (*target_root == NULL && root->left)
            ec = get_node_by_index(root->left, k, target_root);

    if (ec == SUCCESS_CODE && *target_root == NULL)
    {
        if (*k == 0)
            *target_root = root;
        else
            (*k)--;
    }

    if (ec == SUCCESS_CODE && *target_root == NULL && root->right)
        ec = get_node_by_index(root->right, k, target_root);

    return ec;
}

status_t count_nodes(const tree_node_t *root, size_t *cnt)
{
    status_t ec = SUCCESS_CODE;

    if (!root || !cnt)
        ec = ERR_ARGS;
    else
    {
        (*cnt)++;
        count_nodes(root->left, cnt);
        count_nodes(root->right, cnt);
    }

    return ec;
}
