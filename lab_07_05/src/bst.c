#include "../inc/bst.h"

typedef status_t (*rotation_function_t)(bst_node_t *root, bst_node_t **result_root);

status_t create_bst_node(bst_node_t **new_tree_node, const char *word)
{
    status_t exit_code = SUCCESS_CODE;

    if (!new_tree_node)
        exit_code = INVALID_PTR_CODE;
    else
    {
        *new_tree_node = NULL;
        *new_tree_node = malloc(sizeof(bst_node_t));
        if (!(*new_tree_node))
            exit_code = MEMORY_ERR_CODE;
    }

    if (exit_code == SUCCESS_CODE)
    {
        (*new_tree_node)->word = str_dynamic_copy(word);
        (*new_tree_node)->count = 1;
        (*new_tree_node)->left = NULL;
        (*new_tree_node)->right = NULL;
    }

    return exit_code;
}

status_t insert_bst_node(bst_node_t **root, const char *word)
{
    status_t exit_code = SUCCESS_CODE;

    if (!root)
        exit_code = INVALID_PTR_CODE;
    else if (!(*root))
        exit_code = create_bst_node(root, word);
    else if (strcmp(word, (*root)->word) < 0)
        exit_code = insert_bst_node(&((*root)->left), word);
    else if (strcmp(word, (*root)->word) > 0)
        exit_code = insert_bst_node(&((*root)->right), word);
    else
        (*root)->count++;

    return exit_code;
}

status_t make_bst_from_file(bst_node_t **root, const char *filename)
{
    status_t exit_code = SUCCESS_CODE;
    char buffer[MAX_STRING_LENGTH];

    if (!root || !filename)
        exit_code = INVALID_PTR_CODE;
    else
        {
        if (*root)
        {
            free_bst(*root);
            *root = NULL;
        }

        FILE *file = NULL;
        exit_code = safe_open_file(filename, &file);
        if (exit_code == SUCCESS_CODE)
        {
            while ((exit_code == SUCCESS_CODE) && (fscanf(file, "%s", buffer) == 1))
                exit_code = insert_bst_node(root, buffer);
            fclose(file);
        }
    }

    return exit_code;
}

status_t delete_bst_node(bst_node_t **root, const char *word, bool *to_del_found)
{
    status_t exit_code = SUCCESS_CODE;
    bst_node_t *temp_root = NULL;
    bst_node_t *min_parent = NULL;

    if (!root)
        exit_code = INVALID_PTR_CODE;
    else if (!(*root))
        exit_code = NOTHING_TO_DELETE_CODE;
    else
    {
        if (strcmp(word, (*root)->word) < 0)
            delete_bst_node(&((*root)->left), word, to_del_found);
        else if (strcmp(word, (*root)->word) > 0)
            delete_bst_node(&((*root)->right), word, to_del_found);
        else
        {
            *to_del_found = true;
            if (!(*root)->left || !(*root)->right)
            {
                temp_root = (!(*root)->left) ? (*root)->right : (*root)->left;
                free(*root);
                *root = temp_root;
            }
            else
            {
                min_parent = *root;
                temp_root = (*root)->right;
                while (temp_root->left)
                {
                    min_parent = temp_root;
                    temp_root = temp_root->left;
                }

                (*root)->word = str_dynamic_copy(temp_root->word);
                if (min_parent == *root)
                    min_parent->right = temp_root->right;
                else
                    min_parent->left = temp_root->right;
                free(temp_root);
            }
        }
    }
    return exit_code;
}

status_t find_word_in_bst(bst_node_t **root, const char *word, bool flag, int *comparisons)
{
    status_t exit_code = SUCCESS_CODE;
    bst_node_t *target_node = NULL;

    if (comparisons)
        *comparisons = 0;

    if (!root)
        exit_code = INVALID_PTR_CODE;
    else
    {
        bst_node_t *current = *root;
        while (current && !target_node)
        {
            if (comparisons)
                (*comparisons)++;

            if (strcmp(current->word, word) == 0)
                target_node = current;
            else
                current = (strcmp(current->word, word) > 0) ? current->left : current->right;
        }

        if (flag)
        {
            if (target_node)
                printf("Слово было найдено!\n");
            else
                printf("Слово не было найдено!\n");
        }
    }

    return exit_code;
}


void count_nodes_on_levels(bst_node_t *root, size_t level, size_t *levels)
{
    if (!root)
        return;
    levels[level]++;
    count_nodes_on_levels(root->left,  level + 1, levels);
    count_nodes_on_levels(root->right, level + 1, levels);
}

size_t tree_height(bst_node_t *root)
{
     if (root == NULL)
        return 0;

    size_t left_h  = tree_height(root->left);
    size_t  right_h = tree_height(root->right);

    return (left_h > right_h ? left_h : right_h) + 1;
}

status_t define_nodes_quantity_on_each_level(bst_node_t **root)
{
    status_t exit_code = SUCCESS_CODE;

    if (!root)
        exit_code = INVALID_PTR_CODE;
    else
    {
        size_t height = tree_height(*root);
        size_t *levels = calloc(height + 1, sizeof(size_t));
        count_nodes_on_levels(*root, 0, levels);

        for (size_t i = 0; i < height; i++)
            printf("Уровень %zu: %zu узлов\n", i, levels[i]);
        
        free(levels);
    }

    return exit_code;
}

void infix(bst_node_t *root)
{
    if (!root)
        return;

    infix(root->left);             
    infix(root->right);             
}

void free_bst(bst_node_t *root)
{
    if (!root)
        return;
    free_bst(root->left);
    free_bst(root->right);
    free(root);
}

void print_bst_branch(bst_node_t *node, char* prefix, int is_tail, char *color) 
{
    char new_prefix[MAX_PREFIX_SIZE];

    if (node)
    {
        printf("%s%s%s%s%s (%d)\n", prefix, color, (is_tail ? "└── " : "├── "), RESET, node->word, node->count);
        snprintf(new_prefix, sizeof(new_prefix), "%s%s", prefix, (is_tail ? "    " : "│   "));

        if (node->right)
            print_bst_branch(node->right, new_prefix, node->left == NULL, BLUE);

        if (node->left)
            print_bst_branch(node->left, new_prefix, 1, GREEN);
    }
}

status_t print_bst_rec(bst_node_t *root)
{
    status_t exit_code = SUCCESS_CODE;

    if (!root)
        exit_code = EMPTY_TREE_CODE;
    
    if (exit_code == SUCCESS_CODE) 
        printf("* %s (%d)\n", root->word, root->count);
    
    if (exit_code == SUCCESS_CODE && root->right)
        print_bst_branch(root->right, "", root->left == NULL, BLUE);

    if (exit_code == SUCCESS_CODE && root->left)
        print_bst_branch(root->left, "", 1, GREEN);

    return exit_code;
}

void print_bst(bst_node_t *root)
{
    if (print_bst_rec(root) == EMPTY_TREE_CODE)
        printf("Дерево пустое.\n");
}

size_t count_nodes_bst(bst_node_t *root)
{
    if (root == NULL)
        return 0;

    return 1 + count_nodes_bst(root->left) + count_nodes_bst(root->right);
}