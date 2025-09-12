#define type int

signed char is_Binary_Search_Subtree(struct Binary_Tree_Node *parent)
{
    if (parent->left)
        if (parent->left->data > parent->data)
            return 0;
        else if (!is_Binary_Search_Subtree(parent->left))
            return 0;

    if (parent->right)
        if (parent->right->data < parent->data)
            return 0;
        else if (!is_Binary_Search_Subtree(parent->right))
            return 0;

    return 1;
}

signed char is_Binary_Search_Tree(struct Binary_Tree_Node *root)
{
    if (!root)
    {
        perror("The Tree is Empty");
        return -1;
    }

    return is_Binary_Search_Subtree(root);
}

struct Binary_Tree_Node *recursive_search_Binary_Search_Tree(struct Binary_Tree_Node *parent, type search_data)
{
    if (search_data < parent->data)
    {
        if (parent->left)
            return (parent->left ? recursive_search_Binary_Search_Tree(parent->left, search_data) : NULL);
    }
    else if (search_data > parent->data)
        return (parent->right ? recursive_search_Binary_Search_Tree(parent->right, search_data) : NULL);

    return parent;
}

struct Binary_Tree_Node *search_Binary_Search_Tree(struct Binary_Tree_Node *root, type search_data)
{
    if (!is_Binary_Search_Tree(root))
    {
        perror("This isn't even a Binary_Search_Tree!!!");
        return NULL;
    }

    if (search_data < root->data)
        return recursive_search_Binary_Search_Tree(root->left, search_data);
    else if (search_data > root->data)
        return recursive_search_Binary_Search_Tree(root->right, search_data);

    return root;
}

#undef type