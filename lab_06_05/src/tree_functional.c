#include "tree_functional.h"

status_t create_tree_node(tree_node_t **new_tree_node, const char *word);
status_t print_branch(tree_node_t *node, char* prefix, int is_tail);

status_t create_tree_node(tree_node_t **new_tree_node, const char *word)
{
    status_t ec = SUCCESS_CODE;

    if (!new_tree_node || !word)
        ec = ERR_ARGS;

    if (ec == SUCCESS_CODE)
    {
        *new_tree_node = NULL;
        *new_tree_node = malloc(sizeof(tree_node_t));
        if (*new_tree_node == NULL)
            ec = ERR_MEM;
    }

    if (ec == SUCCESS_CODE)
    {
        (*new_tree_node)->word = str_dynamic_copy(word);
        (*new_tree_node)->counted = 0;
        (*new_tree_node)->left = NULL;
        (*new_tree_node)->right = NULL;
    }

    return ec;
}

status_t insert_tree_node(tree_node_t **root, const char *word)
{
    status_t ec = SUCCESS_CODE;
    int cmp = 0;

    if (!root || !word)
        ec = ERR_ARGS;

    // если вершина дерева пуста
    if (ec == SUCCESS_CODE && !(*root))
        ec = create_tree_node(root, word);

    // если вершина дерева не пуста
    if (ec == SUCCESS_CODE && *root)
    {
        cmp = strcmp(word, (*root)->word);

        if (cmp == 0)
            (*root)->counted++;
        else if (cmp < 0)
            ec = insert_tree_node(&((*root)->left), word);
        else
            ec = insert_tree_node(&((*root)->right), word);
    }

    return ec;
}

status_t delete_tree_node(tree_node_t **root, const char *word)
{
    status_t ec = SUCCESS_CODE;
    tree_node_t *min_parent = NULL;
    tree_node_t *tmp = NULL;
    int cmp = 0;

    if (!root || !word)
        ec = ERR_ARGS;
    else if (!(*root))
        ec = ERR_NOT_FOUND;

    if (ec == SUCCESS_CODE)
    {
        cmp = strcmp(word, (*root)->word);

        if (cmp < 0)
            ec = delete_tree_node(&((*root)->left), word);
        else if (cmp > 0)
            ec = delete_tree_node(&((*root)->right), word);
        else  // нашли узел для удаления
        {
            if ((*root)->left == NULL) 
            {
                tmp = (*root)->right;  
                free((*root)->word);
                free(*root);
                *root = tmp;
            }
            else if ((*root)->right == NULL)
            {
                tmp = (*root)->left;
                free((*root)->word);
                free(*root);
                *root = tmp;
            }
            else
            {
                min_parent = *root;
                tmp = (*root)->right;
                while (tmp->left != NULL)
                {
                    min_parent = tmp;
                    tmp = tmp->left;
                }

                free((*root)->word);
                (*root)->word = str_dynamic_copy(tmp->word);
                (*root)->counted = tmp->counted;

                if (min_parent == *root)
                    min_parent->right = tmp->right;
                else
                    min_parent->left = tmp->right;

                free(tmp->word);
                free(tmp);
            }
        }
    }
    return ec;
}

status_t find_word_in_tree(tree_node_t *root, tree_node_t **target_root, const char *word)
{
    status_t ec = SUCCESS_CODE;
    int cmp = 0;

    if (!root || !target_root || !word || word[0] == '\0')
        ec = ERR_ARGS;

    if (ec == SUCCESS_CODE)
        *target_root = NULL;

    while (ec == SUCCESS_CODE && root && !(*target_root))
    {
        cmp = strcmp(word, root->word);
        if (cmp == 0) (*target_root) = root;
        else root = (cmp < 0) ? root->left : root->right;
    }

    return ec;
}

status_t print_pretty_tree(tree_node_t *root) 
{
    status_t ec = SUCCESS_CODE;

    if (!root)
        ec = ERR_EMPTY_TREE;
    
    if (ec == SUCCESS_CODE)  // печатаем корень
        printf("* %s (%zu)\n", root->word, root->counted);
    
    if (ec == SUCCESS_CODE && root->right)  // печатаем ребенка
        print_branch(root->right, "", root->left == NULL);

    if (ec == SUCCESS_CODE && root->left)  // печатаем ребенка
        print_branch(root->left, "", 1);

    return ec;
}

status_t print_branch(tree_node_t *node, char* prefix, int is_tail) 
{
    char new_prefix[MAX_PREFIX_SIZE];

    if (node)
    {
        // печатаем что есть и формируем новый префикс
        printf("%s%s%s (%zu)\n", prefix, (is_tail ? "└── " : "├── "), node->word, node->counted);
        snprintf(new_prefix, sizeof(new_prefix), "%s%s", prefix, (is_tail ? "    " : "│   "));

        if (node->right)
            print_branch(node->right, new_prefix, node->left == NULL);

        if (node->left)
            print_branch(node->left, new_prefix, 1);
    }

    return SUCCESS_CODE;
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