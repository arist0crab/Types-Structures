#include "../inc/avl.h"

typedef result_t (*rotation_function_t)(avl_node_t *root, avl_node_t **result_root);

int get_height(avl_node_t *node)
{
    if (!node)
        return 0;
    return node->height;
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}

void update_height(avl_node_t *node)
{
    if (node)
        node->height = 1 + max(get_height(node->left), get_height(node->right));
}

result_t node_balance_factor(avl_node_t *root, int *balance_factor)
{
    result_t exit_code = OK_CODE;

    if (!balance_factor)
        exit_code = INVALID_PTR_CODE;
    else
        *balance_factor = get_height(root->left) - get_height(root->right);

    return exit_code;
}

result_t rotate_tree_right(avl_node_t *root, avl_node_t **result_root)
{
    result_t exit_code = OK_CODE;

    avl_node_t *hanging_root = NULL;
    avl_node_t *new_root = NULL;

    if (!result_root)
        exit_code = INVALID_PTR_CODE;
    else if (!root->left)
        *result_root = root;
    else
    {
        new_root = root->left;
        hanging_root = new_root->right;
        new_root->right = root;
        root->left = hanging_root;

        update_height(root);
        update_height(new_root);

        *result_root = new_root;
    }

    return exit_code;
}

result_t rotate_tree_left(avl_node_t *root, avl_node_t **result_root)
{
    result_t exit_code = OK_CODE;

    avl_node_t *hanging_root = NULL;
    avl_node_t *new_root = NULL;

    if (!result_root)
        exit_code = INVALID_PTR_CODE;
    else if (!root->right)
        *result_root = root;
    else
    {
        new_root = root->right;
        hanging_root = new_root->left;
        new_root->left = root;
        root->right = hanging_root;

        update_height(root);
        update_height(new_root);

        *result_root = new_root;
    }

    return exit_code;
}

result_t rotate_tree_twice(avl_node_t **root, avl_node_t **subroot, rotation_function_t first_rotation, rotation_function_t second_rotation)
{
    result_t exit_code = OK_CODE;

    if (!root || !subroot)
        exit_code = INVALID_PTR_CODE;
    else
    {
        exit_code = first_rotation(*subroot, subroot);
        if (exit_code == OK_CODE)
            exit_code = second_rotation(*root, root);
    }

    return exit_code;
}

result_t balance_node(avl_node_t **root)
{
    result_t exit_code = OK_CODE;
    int balance_factor = 0, left_balance_factor = 0, right_balance_factor = 0;

    if (!root)
        exit_code = INVALID_PTR_CODE;
    else if (*root)
    {
        exit_code = node_balance_factor(*root, &balance_factor);
        
        if ((exit_code == OK_CODE) && (balance_factor > 1))
        {
            exit_code = node_balance_factor((*root)->left, &left_balance_factor);
            if ((exit_code == OK_CODE) && (left_balance_factor < 0))
                exit_code = rotate_tree_twice(root, &(*root)->left, rotate_tree_left, rotate_tree_right);
            else if (exit_code == OK_CODE)
                exit_code = rotate_tree_right(*root, root);
        }
        if ((exit_code == OK_CODE) && (balance_factor < - 1))
        {
            exit_code = node_balance_factor((*root)->right, &right_balance_factor);
            if ((exit_code == OK_CODE) && (right_balance_factor > 0))
                exit_code = rotate_tree_twice(root, &(*root)->right, rotate_tree_right, rotate_tree_left);
            else if (exit_code == OK_CODE)
                exit_code = rotate_tree_left(*root, root);
        }

        if (exit_code == OK_CODE)
            update_height(*root);
    }

    return exit_code;
}

result_t create_avl_node(avl_node_t **new_tree_node, const char *word)
{
    result_t exit_code = OK_CODE;

    if (!new_tree_node)
        exit_code = INVALID_PTR_CODE;
    else
    {
        *new_tree_node = NULL;
        *new_tree_node = malloc(sizeof(avl_node_t));
        if (!(*new_tree_node))
            exit_code = MEMORY_ERR_CODE;
    }

    if (exit_code == OK_CODE)
    {
        (*new_tree_node)->word = str_dynamic_copy(word);
        (*new_tree_node)->count = 1;
        (*new_tree_node)->left = NULL;
        (*new_tree_node)->right = NULL;
        (*new_tree_node)->height = 1;
    }

    return exit_code;
}

result_t insert_avl_node(avl_node_t **root, const char *word)
{
    result_t exit_code = OK_CODE;

    if (!root)
        exit_code = INVALID_PTR_CODE;
    else if (!(*root))
        exit_code = create_avl_node(root, word);
    else if (strcmp(word, (*root)->word) < 0)
        exit_code = insert_avl_node(&((*root)->left), word);
    else if (strcmp(word, (*root)->word) > 0)
        exit_code = insert_avl_node(&((*root)->right), word);
    else
        (*root)->count++;

    if (exit_code == OK_CODE)
    {
        update_height(*root);
        exit_code = balance_node(root);
    }


    return exit_code;
}

result_t make_avl_from_file(avl_node_t **root, const char *filename)
{
    result_t exit_code = OK_CODE;
    char buffer[MAX_STRING_LENGTH];

    if (!root || !filename)
        exit_code = INVALID_PTR_CODE;
    else
        {
        if (*root)
        {
            free_avl(*root);
            *root = NULL;
        }

        FILE *file = NULL;
        if (safe_open_file(filename, &file) == OK_CODE)
        {
            while ((exit_code == OK_CODE) && (fscanf(file, "%s", buffer) == 1))
                exit_code = insert_avl_node(root, buffer);
            fclose(file);
        }
    }

    return exit_code;
}

void free_avl(avl_node_t *root)
{
    if (!root)
        return;

    free_avl(root->left);
    free_avl(root->right);

    free(root->word);
    free(root);
}

size_t avl_height(avl_node_t *root)
{
     if (root == NULL)
        return 0;

    size_t left_h  = avl_height(root->left);
    size_t  right_h = avl_height(root->right);

    return (left_h > right_h ? left_h : right_h) + 1;
}

void print_avl_branch(avl_node_t *node, char* prefix, int is_tail, char *color) 
{
    char new_prefix[MAX_PREFIX_SIZE];

    if (node)
    {
        printf("%s%s%s%s%s (%d)\n", prefix, color, (is_tail ? "└── " : "├── "), RESET, node->word, node->count);
        snprintf(new_prefix, sizeof(new_prefix), "%s%s", prefix, (is_tail ? "    " : "│   "));

        if (node->right)
            print_avl_branch(node->right, new_prefix, node->left == NULL, BLUE);

        if (node->left)
            print_avl_branch(node->left, new_prefix, 1, GREEN);
    }
}

result_t print_avl_rec(avl_node_t *root)
{
    result_t exit_code = OK_CODE;

    if (!root)
        exit_code = EMPTY_TREE_CODE;
    
    if (exit_code == OK_CODE) 
        printf("* %s (%d)\n", root->word, root->count);
    
    if (exit_code == OK_CODE && root->right)
        print_avl_branch(root->right, "", root->left == NULL, BLUE);

    if (exit_code == OK_CODE && root->left)
        print_avl_branch(root->left, "", 1, GREEN);

    return exit_code;
}

void print_avl(avl_node_t *root)
{
    if (print_avl_rec(root) == EMPTY_TREE_CODE)
        printf("Дерево пустое.\n");
}

result_t delete_avl_node(avl_node_t **root, const char *word, bool *to_del_found)
{
    result_t exit_code = OK_CODE;
    avl_node_t *temp_root = NULL;
    avl_node_t *min_parent = NULL;

    if (!root)
        exit_code = INVALID_PTR_CODE;
    else if (!(*root))
        exit_code = NOTHING_TO_DELETE_CODE;
    else
    {
        if (strcmp(word, (*root)->word) < 0)
            delete_avl_node(&((*root)->left), word, to_del_found);
        else if (strcmp(word, (*root)->word) > 0)
            delete_avl_node(&((*root)->right), word, to_del_found);
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
        if (exit_code == OK_CODE && *root)
        {
            update_height(*root);
            exit_code = balance_node(root);
        }

    }
    return exit_code;
}

result_t find_word_in_avl(avl_node_t **root, const char *word, bool flag, int *comparisons)
{
    result_t exit_code = OK_CODE;
    avl_node_t *target_node = NULL;

    if (comparisons)
        *comparisons = 0;

    if (!root)
        exit_code = INVALID_PTR_CODE;
    else
    {
        avl_node_t *current = *root;
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


size_t count_nodes_avl(avl_node_t *root)
{
    if (root == NULL)
        return 0;

    return 1 + count_nodes_avl(root->left) + count_nodes_avl(root->right);
}