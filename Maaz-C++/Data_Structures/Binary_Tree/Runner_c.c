#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Linked_Binary_Tree_c.h"
#include "Binary_Tree_Traversal_c.h"

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