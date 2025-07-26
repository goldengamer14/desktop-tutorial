#include <stdio.h>
#include <stdlib.h>

#define type int

struct Queue_Node
{
    type data;
    struct Queue_Node *next;
};

struct Queue_Linked_List
{
    unsigned short int size;
    struct Queue_Node *front;
    struct Queue_Node *back;
};

struct Queue_Linked_List *enqueue_Queue_Linked_List(struct Queue_Linked_List *self, type data)
{

    if (self->front == NULL)
        self->front = self->back = (struct Queue_Node *)malloc(sizeof(struct Queue_Node));
    else
        self->back = (self->back->next = (struct Queue_Node *)malloc(sizeof(struct Queue_Node)));

    self->back->data = data;
    self->back->next = NULL;
    self->size++;

    return self;
}

type dequeue_Queue_Linked_List(struct Queue_Linked_List *self)
{
    if (self->front == NULL)
    {
        perror("\nQueue Underflow!!!\n");
        return (type)0x00;
    }

    struct Queue_Node *delete_node = self->front;
    self->front = self->front->next;
    self->size--;

    type return_data = delete_node->data;
    free(delete_node);

    if (self->front == NULL)
        self->back = NULL;

    return return_data;
}

struct Queue_Linked_List construct_Queue_Linked_List()
{
    struct Queue_Linked_List self;
    self.back = self.front = NULL;
    self.size = 0;

    return self;
}

struct Queue_Node *destruct_Queue_Linked_List_Node_chain(struct Queue_Node *ptr)
{
    if (ptr->next && ptr->next != NULL)
        destruct_Queue_Linked_List_Node_chain(ptr->next);
    free(ptr);

    return ptr;
}

struct Queue_Linked_List *destruct_Queue_Linked_List(struct Queue_Linked_List *self)
{
    if (self->front && self->front != NULL)
        destruct_Queue_Linked_List_Node_chain(self->front);

    self->front = self->back = NULL;
    self->size = 0;

    return self;
}

#undef type

void display_Queue_Linked_List_int(struct Queue_Linked_List *self)
{
    struct Queue_Node *ptr = self->front;

    printf("%s", ptr != NULL ? "{" : "Empty Queue!!!\n");
    while (ptr != NULL)
    {
        printf("%i%s", ptr->data, ptr->next != NULL ? ", " : "}\n");
        ptr = ptr->next;
    }
}

int main()
{
    printf("\nHello, World!\n\n");

    struct Queue_Linked_List self = construct_Queue_Linked_List();

    enqueue_Queue_Linked_List(&self, 45);
    enqueue_Queue_Linked_List(&self, 12);
    enqueue_Queue_Linked_List(&self, 67);

    printf("self after {45, 12, 67} enqueue: ");
    display_Queue_Linked_List_int(&self);

    printf("\nself after {%i, %i, %i} dequeue: ", dequeue_Queue_Linked_List(&self),
           dequeue_Queue_Linked_List(&self), dequeue_Queue_Linked_List(&self));
    display_Queue_Linked_List_int(&self);

    dequeue_Queue_Linked_List(&self);

    destruct_Queue_Linked_List(&self);
    printf("\n");

    return 0;
}