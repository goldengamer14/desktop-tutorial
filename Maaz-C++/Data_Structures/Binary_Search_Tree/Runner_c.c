#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "..\Binary_Tree\Linked_Binary_Tree_c.h"
#include "..\Binary_Tree\Binary_Tree_Traversal_c.h"
#include "Binary_Search_Tree_Authentication.h"

int main()
{
    printf("\nHello, World!\n\n");

    struct Binary_Tree_Node *self = construct_Binary_Tree_initialized(45);

    self->left = construct_Binary_Tree_initialized(867);
    self->right = construct_Binary_Tree_initialized(1042);

    self->left->left = construct_Binary_Tree_initialized(2034);
    self->left->right = construct_Binary_Tree_initialized(8756);

    self->right->left = construct_Binary_Tree_initialized(9184);
    self->right->right = construct_Binary_Tree_initialized(289);

    printf("In_Order: ");
    print_Binary_Tree_Inorder(self);

    printf("\nIs this a Binary_Search_Tree? -> %hhi\n", is_Binary_Search_Tree(self));

    puts("\n");
    destruct_Binary_Tree(self);

    self = construct_Binary_Tree_initialized(4);

    self->left = construct_Binary_Tree_initialized(1);
    self->right = construct_Binary_Tree_initialized(6);

    self->left->left = construct_Binary_Tree_initialized(5);
    self->left->right = construct_Binary_Tree_initialized(2);

    printf("In_Order: ");
    print_Binary_Tree_Inorder(self);

    printf("\nIs this a Binary_Search_Tree? -> %hhi\n", is_Binary_Search_Tree(self));

    puts("\n");
    destruct_Binary_Tree(self);

    self = construct_Binary_Tree_initialized(5);

    self->left = construct_Binary_Tree_initialized(3);
    self->right = construct_Binary_Tree_initialized(6);

    self->left->left = construct_Binary_Tree_initialized(1);
    self->left->right = construct_Binary_Tree_initialized(4);

    printf("In_Order: ");
    print_Binary_Tree_Inorder(self);

    printf("\nIs this a Binary_Search_Tree? -> %hhi\n", is_Binary_Search_Tree(self));

    struct Binary_Tree_Node *find = search_Binary_Search_Tree(self, 1);
    int max = (~(int)0 - (0x80 << (sizeof(int) - 1) * 8));

    printf("The number 1 in the Node: %p; Look: data = %i\n", find, find ? find->data : max);
    find = search_Binary_Search_Tree(self, 3);
    printf("The number 3 in the Node: %p; Look: data = %i\n", find, find ? find->data : max);
    find = search_Binary_Search_Tree(self, 4);
    printf("The number 4 in the Node: %p; Look: data = %i\n", find, find ? find->data : max);
    find = search_Binary_Search_Tree(self, 5);
    printf("The number 5 in the Node: %p; Look: data = %i\n", find, find ? find->data : max);
    find = search_Binary_Search_Tree(self, 6);
    printf("The number 6 in the Node: %p; Look: data = %i\n", find, find ? find->data : max);
    find = search_Binary_Search_Tree(self, 2);
    printf("The number 2 in the Node: %p; Look: data = %i\n", find, find ? find->data : max);

    putchar('\n');
    destruct_Binary_Tree(self);

    return 0;
}