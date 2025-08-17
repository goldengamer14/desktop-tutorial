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

struct Binary_Tree_Node *construct_Binary_Tree()
{
    return (struct Binary_Tree_Node *)calloc(1, sizeof(struct Binary_Tree_Node));
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
        destruct_Binary_Tree(ptr->left);
    if (ptr->right)
        destruct_Binary_Tree(ptr->right);
    if (ptr)
        free(ptr);

    return ptr;
}

#undef type

void print_Binary_Tree_Preorder(struct Binary_Tree_Node *self)
{
    if (!self)
        return;

    printf(" %i [", self->data);
    if (self->left)
        print_Binary_Tree_Preorder(self->left);

    printf("][");

    if (self->right)
        print_Binary_Tree_Preorder(self->right);

    putchar(']');
}

void print_Binary_Tree_Postorder(struct Binary_Tree_Node *self)
{
    if (!self)
        return;

    if (self->left)
        print_Binary_Tree_Postorder(self->left);

    if (self->right)
        print_Binary_Tree_Postorder(self->right);

    printf("%i ", self->data);
}

void print_Binary_Tree_Inorder(struct Binary_Tree_Node *self)
{
    if (!self)
        return;

    if (self->left)
        print_Binary_Tree_Inorder(self->left);

    printf("%i ", self->data);

    if (self->right)
        print_Binary_Tree_Inorder(self->right);
}

int main()
{
    printf("\nHello, World!\n\n");

    struct Binary_Tree_Node *self = construct_Binary_Tree_initialized(45);

    printf("self->left = %p; self->right = %p; self->data = %i\n", self->left, self->right, self->data);
    printf("self->left == NULL: %i; self->right == NULL: %i\n\n", self->left == NULL, self->right == NULL);

    self->left = construct_Binary_Tree_initialized(867);
    self->right = construct_Binary_Tree_initialized(1042);

    self->left->left = construct_Binary_Tree_initialized(2034);
    self->left->right = construct_Binary_Tree_initialized(8756);

    self->right->left = construct_Binary_Tree_initialized(9184);
    self->right->right = construct_Binary_Tree_initialized(289);

    printf("Pre_Order: ");
    print_Binary_Tree_Preorder(self);
    printf("\nPost_Order: ");
    print_Binary_Tree_Postorder(self);
    printf("\nIn_Order: ");
    print_Binary_Tree_Inorder(self);

    puts("\n");
    destruct_Binary_Tree(self);

    self = construct_Binary_Tree_initialized(4);

    self->left = construct_Binary_Tree_initialized(1);
    self->right = construct_Binary_Tree_initialized(6);

    self->left->left = construct_Binary_Tree_initialized(5);
    self->left->right = construct_Binary_Tree_initialized(2);

    printf("Pre_Order: ");
    print_Binary_Tree_Preorder(self);
    printf("\nPost_Order: ");
    print_Binary_Tree_Postorder(self);
    printf("\nIn_Order: ");
    print_Binary_Tree_Inorder(self);

    puts("\n");
    destruct_Binary_Tree(self);

    return 0;
}