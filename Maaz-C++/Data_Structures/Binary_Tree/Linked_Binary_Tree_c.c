#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define type int

struct Binary_Tree_Node
{
    type data;
    struct Binary_Tree_Node *right;
    struct Binary_Tree_Node *left;
};

struct Binary_Tree_Node *
construct_Binary_Tree()
{
    struct Binary_Tree_Node *self = (struct Binary_Tree_Node *)malloc(sizeof(struct Binary_Tree_Node));

    memset(&self->data, 0, sizeof(type));
    self->left = self->right = NULL;

    return self;
}

struct Binary_Tree_Node *construct_Binary_Tree_initialized(type data)
{
    struct Binary_Tree_Node *self = construct_Binary_Tree();
    self->data = data;

    return self;
}

struct Binary_Tree_Node *destruct_Binary_Tree(struct Binary_Tree_Node *ptr)
{
    if (ptr->left)
    {
        destruct_Binary_Tree(ptr->left);
        free(ptr->left);
    }
    if (ptr->right)
    {
        destruct_Binary_Tree(ptr->right);
        free(ptr->right);
    }
    if (ptr)
        free(ptr);

    return ptr;
}

#undef type

int main()
{
    printf("\nHello, World!\n\n");

    return 0;
}