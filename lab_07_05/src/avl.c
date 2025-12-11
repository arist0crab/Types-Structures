#include "avl.h"

typedef status_t (*rotation_function_t)(bst_node_t *subroot, bst_node_t **result_subroot);

status_t get_height(bst_node_t *bst_node, size_t *height);
status_t get_balance_factor(bst_node_t *bst_node, int *result_factor);
status_t rotate_left(bst_node_t *bst_node, bst_node_t **result_bst_node);
status_t rotate_right(bst_node_t *bst_node, bst_node_t **result_bst_node);
status_t rebalance_child_and_root(bst_node_t **bst_root, bst_node_t **child_subtree, bool need_double_rotation, rotation_function_t first_rotation, rotation_function_t final_rotation);

status_t balance_tree(bst_node_t **bst_root)
{
    status_t ec = SUCCESS_CODE;
    int left_balance = 0, right_balance = 0;
    bool need_double_rotation = false;
    int balance = 0;

    if (!bst_root) ec = ERR_ARGS;

    if (ec == SUCCESS_CODE && *bst_root)
        ec = balance_tree(&(*bst_root)->left);

    if (ec == SUCCESS_CODE && *bst_root)
        ec = balance_tree(&(*bst_root)->right);

    if (ec == SUCCESS_CODE && *bst_root)
        ec = get_balance_factor(*bst_root, &balance);

    if (ec == SUCCESS_CODE && *bst_root && balance > 1)
    {
        ec = get_balance_factor((*bst_root)->left, &left_balance);
        if (ec == SUCCESS_CODE)
        {
            need_double_rotation = (left_balance < 0);
            ec = rebalance_child_and_root(bst_root, &(*bst_root)->left, need_double_rotation, rotate_left, rotate_right);
        }
    }

    if (ec == SUCCESS_CODE && *bst_root && balance < -1)
    {
        ec = get_balance_factor((*bst_root)->right, &right_balance);
        if (ec == SUCCESS_CODE)
        {
            need_double_rotation = (right_balance > 0);
            ec = rebalance_child_and_root(bst_root, &(*bst_root)->right, need_double_rotation, rotate_right, rotate_left);
        }
    }

    return ec;
}

status_t rebalance_child_and_root(bst_node_t **bst_root, bst_node_t **child_subtree, bool need_double_rotation, rotation_function_t first_rotation, rotation_function_t final_rotation)
{
    status_t ec = SUCCESS_CODE;

    if (!bst_root || !*bst_root || !child_subtree)
        ec = ERR_ARGS;

    if (ec == SUCCESS_CODE && need_double_rotation)
        ec = first_rotation(*child_subtree, child_subtree);

    if (ec == SUCCESS_CODE)
        ec = final_rotation(*bst_root, bst_root);

    return ec;
}

status_t rotate_right(bst_node_t *bst_node, bst_node_t **result_bst_node)
{
    status_t ec = SUCCESS_CODE;
    bst_node_t *new_root = NULL, *moved_subtree = NULL;

    if (!result_bst_node) ec = ERR_ARGS;

    if (ec == SUCCESS_CODE && (bst_node == NULL || bst_node->left == NULL))
        *result_bst_node = bst_node;
    else if (ec == SUCCESS_CODE)
    {
        new_root = bst_node->left;  // новый корень
        moved_subtree = new_root->right;
        new_root->right = bst_node;
        bst_node->left = moved_subtree;
        *result_bst_node = new_root;
    }

    return ec;
}

status_t rotate_left(bst_node_t *bst_node, bst_node_t **result_bst_node)
{
    status_t ec = SUCCESS_CODE;
    bst_node_t *new_root = NULL, *moved_subtree = NULL;

    if (!result_bst_node) ec = ERR_ARGS;

    if (ec == SUCCESS_CODE && (bst_node == NULL || bst_node->right == NULL))
        *result_bst_node = bst_node;
    else if (ec == SUCCESS_CODE)
    {
        new_root = bst_node->right;  // новый корень
        moved_subtree = new_root->left;
        new_root->left = bst_node;
        bst_node->right = moved_subtree;
        *result_bst_node = new_root;
    }

    return ec;
}

status_t get_balance_factor(bst_node_t *bst_node, int *result_factor)
{
    status_t ec = SUCCESS_CODE;
    size_t left_height = 0, right_height = 0;

    if (!result_factor) ec = ERR_ARGS;

    if (ec == SUCCESS_CODE && bst_node == NULL)
        *result_factor = 0;

    if (ec == SUCCESS_CODE && bst_node)
    {
        ec = get_height(bst_node->left, &left_height);
        if (ec == SUCCESS_CODE)
            ec = get_height(bst_node->right, &right_height);
        if (ec == SUCCESS_CODE)
            *result_factor = left_height - right_height;
    }

    return ec;
}

status_t get_height(bst_node_t *bst_node, size_t *height)
{
    status_t ec = SUCCESS_CODE;
    size_t left_height = 0;
    size_t right_height = 0;

    if (!height) ec = ERR_ARGS;

    if (ec == SUCCESS_CODE && bst_node == NULL)
        *height = 0;

    if (ec == SUCCESS_CODE && bst_node)
    {
        ec = get_height(bst_node->left, &left_height);
        if (ec == SUCCESS_CODE)
            ec = get_height(bst_node->right, &right_height);
        if (ec == SUCCESS_CODE)
            *height = 1 + ((left_height > right_height) ? left_height : right_height);
    }

    return ec;
}