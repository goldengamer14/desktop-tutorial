#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef type
#define type char
#endif

struct Node
{
    type data;
    struct Node *next;
};

struct Linked_List
{
    struct Node *head;
    long int size;
};

struct Linked_List construct_Linked_List()
{
    struct Linked_List self;
    self.head = NULL;
    self.size = 0;

    return self;
}

struct Linked_List construct_Linked_List_initialized(type data)
{
    struct Linked_List self;
    self.head = (struct Node *)malloc(sizeof(struct Node));
    self.head->data = data;
    self.head->next = NULL;
    self.size = 1;

    return self;
}

struct Node *peek(struct Linked_List *self, long int index)
{
    if (index < 0 || index >= self->size)
    {
        printf("Linked List Underflow!!!\n");
        return self->head;
    }

    struct Node *peek_element = self->head;
    while (index-- > 0 && peek_element->next != NULL)
        peek_element = peek_element->next;
    return peek_element;
}

struct Linked_List *push_Linked_List(struct Linked_List *self, type data)
{
    struct Node *ptr = self->head;

    if (ptr == NULL)
    {
        ptr = (struct Node *)malloc(sizeof(struct Node));
        ptr->data = data;
        ptr->next = NULL;

        self->size++;
        return self;
    }

    while (ptr->next != NULL)
        ptr = ptr->next;

    ptr->next = (struct Node *)malloc(sizeof(struct Node));
    ptr->next->data = data;
    ptr->next->next = NULL;

    self->size++;

    return self;
}

struct Linked_List *push_Linked_List_string(struct Linked_List *self, type *data)
{
    struct Node *ptr = self->head;

    if (ptr == NULL)
    {
        ptr = (struct Node *)malloc(sizeof(struct Node));
        ptr->data = (type)data[0];
        ptr->next = NULL;

        for (long int i = 1; i < strlen(data); i++)
        {
            ptr->next = (struct Node *)malloc(sizeof(struct Node));
            ptr->next->data = data[i];
            ptr->next->next = NULL;

            self->size++;
        }

        return self;
    }

    while (ptr->next != NULL)
        ptr = ptr->next;

    for (long int i = 0; i < strlen(data); i++)
    {
        ptr->next = (struct Node *)malloc(sizeof(struct Node));
        ptr->next->data = data[i];
        ptr->next->next = NULL;

        ptr = ptr->next;
        self->size++;
    }

    return self;
}

struct Linked_List *unshift_Linked_List(struct Linked_List *self, type data)
{
    if (self->head == NULL)
    {
        self->head = (struct Node *)malloc(sizeof(struct Node));
        self->head->data = data;
        self->head->next = NULL;

        self->size++;
        return self;
    }

    struct Node *new_head = (struct Node *)malloc(sizeof(struct Node));
    new_head->data = data;
    new_head->next = self->head;
    self->head = new_head;

    self->size++;
    return self;
}

struct Linked_List *unshift_Linked_List_string(struct Linked_List *self, type *data)
{
    if (self->head == NULL)
    {
        self->head = (struct Node *)malloc(sizeof(struct Node));
        self->head->data = data[0];
        self->head->next = NULL;

        for (long int i = 1; i < strlen(data); i++)
        {
            struct Node *new_head = (struct Node *)malloc(sizeof(struct Node));
            new_head->data = data[i];
            new_head->next = self->head;
            self->head = new_head;

            self->size++;
        }

        return self;
    }

    for (long int i = 0; i < strlen(data); i++)
    {
        struct Node *new_head = (struct Node *)malloc(sizeof(struct Node));
        new_head->data = data[i];
        new_head->next = self->head;
        self->head = new_head;

        self->size++;
    }

    return self;
}

struct Linked_List *insert_Linked_List(struct Linked_List *self, type data, long int index)
{
    if (index < 0 || index >= self->size)
    {
        printf("Linked List Underflow!!!\n");
        return self;
    }
    else if (index == 0)
        return unshift_Linked_List(self, data);
    else if (index == self->size - 1)
        return push_Linked_List(self, data);

    struct Node *ptr = self->head;
    while (index-- > 0 && ptr->next != NULL)
        ptr = ptr->next;

    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    new_node->data = data;
    new_node->next = ptr->next;
    ptr->next = new_node;

    self->size++;
    return self;
}

struct Linked_List *insert_Linked_List_string(struct Linked_List *self, type *data, long int index)
{
    if (index < 0 || index >= self->size)
    {
        printf("Linked List Underflow!!!\n");
        return self;
    }
    else if (index == 0)
        return unshift_Linked_List_string(self, data);
    else if (index == self->size - 1)
        return push_Linked_List_string(self, data);

    struct Node *ptr = self->head;
    while (index-- > 0 && ptr->next != NULL)
        ptr = ptr->next;

    for (long int i = 0; i < strlen(data); i++)
    {
        struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
        new_node->data = data[i];
        new_node->next = ptr->next;
        ptr->next = new_node;

        self->size++;
        ptr = ptr->next;
    }

    return self;
}

struct Linked_List *pop_Linked_List(struct Linked_List *self)
{
    if (self->size < 1)
    {
        printf("Linked List Underflow!!!\n");
        return self;
    }

    struct Node *ptr = self->head;
    while (ptr->next != NULL && ptr->next->next != NULL)
        ptr = ptr->next;

    free(ptr->next);
    ptr->next = NULL;
    self->size--;

    return self;
}

struct Linked_List *pop_Linked_List_string(struct Linked_List *self, long int index)
{
    if (self->size < 1)
    {
        printf("Linked List Underflow!!!\n");
        return self;
    }
    else if (index < 0 || index >= self->size - 1)
    {
        printf("Invalid Index!!!\n");
        return self;
    }

    struct Node *ptr = self->head;
    while (index-- > 0)
        ptr = ptr->next;

    while (ptr->next != NULL)
    {
        struct Node *delete_node = ptr->next;
        ptr->next = ptr->next->next;
        free(delete_node);

        self->size--;
    }

    return self;
}

struct Linked_List *shift_Linked_List(struct Linked_List *self)
{
    if (self->size < 1)
    {
        printf("Linked List Underflow!!!\n");
        return self;
    }

    struct Node *new_head = self->head->next;
    free(self->head);
    self->head = new_head;

    self->size--;
    return self;
}

struct Linked_List *shift_Linked_List_string(struct Linked_List *self, long int index)
{
    if (self->size < 1)
    {
        printf("Linked List Underflow!!!\n");
        return self;
    }
    else if (index < 0 || index > self->size - 1)
    {
        printf("Invalid Index!!!\n");
        return self;
    }

    while (index-- > 0)
    {
        struct Node *new_head = self->head->next;
        free(self->head);
        self->head = new_head;

        self->size--;
    }

    return self;
}

struct Linked_List *remove_Linked_List(struct Linked_List *self, long int index)
{
    if (index < 0 || index >= self->size)
    {
        printf("Linked List Underflow!!!\n");
        return self;
    }
    else if (index == 0)
        return shift_Linked_List(self);
    else if (index == self->size - 1)
        return pop_Linked_List(self);

    struct Node *ptr = self->head;
    while (index-- > 0 && ptr->next != NULL)
        ptr = ptr->next;

    struct Node *delete_node = ptr->next;
    ptr->next = ptr->next->next;
    free(delete_node);

    self->size--;
    return self;
}

struct Linked_List *remove_Linked_List_string(struct Linked_List *self, long int start_index, long int end_index)
{
    if (start_index >= end_index || start_index < 0 || start_index >= self->size || end_index < 0 || end_index >= self->size)
    {
        printf("Invalid Indices!!!\n");
        return self;
    }
    else if (start_index == 0)
        return shift_Linked_List_string(self, end_index);
    else if (end_index == self->size - 1)
        return pop_Linked_List_string(self, start_index);

    struct Node *ptr = self->head;
    for (long int i = 0; i < start_index && ptr->next != NULL; i++)
        ptr = ptr->next;

    for (long int i = 0; i <= end_index && ptr->next != NULL; i++)
    {
        struct Node *delete_node = ptr->next;
        ptr->next = ptr->next->next;
        free(delete_node);

        self->size--;
    }

    return self;
}

void destruct_Linked_List_Node(struct Node *ptr)
{
    if (ptr->next != NULL)
        destruct_Linked_List_Node(ptr->next);
    free(ptr);
}

void destruct_Linked_List(struct Linked_List *self)
{
    if (self->head != NULL)
        destruct_Linked_List_Node(self->head);
}

#undef type