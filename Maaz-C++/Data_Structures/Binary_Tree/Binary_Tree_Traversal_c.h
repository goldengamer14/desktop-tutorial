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