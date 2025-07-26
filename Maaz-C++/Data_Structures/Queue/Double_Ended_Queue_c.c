#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define type float

struct DE_Queue
{
    unsigned int max_size;
    type *arr;
    unsigned int front;
    unsigned int back;
};

unsigned int DE_Queue_size(struct DE_Queue *self)
{
    return (self->back >= self->front ? self->back - self->front - 1
                                      : self->max_size - (self->front - self->back) - 1);
}

unsigned int DE_Queue_max_size(struct DE_Queue *self) { return self->max_size; }

struct DE_Queue *push_DE_Queue(struct DE_Queue *self, type data)
{
    if ((self->back + 1) % self->max_size == self->front)
        perror("Queue Overflow!!!");
    else
    {
        self->arr[self->back] = data;
        self->back = (self->back + 1) % self->max_size;
    }

    return self;
}

struct DE_Queue *push_DE_Queue_array(struct DE_Queue *self, type *data, unsigned int arr_size)
{
    if ((self->back + 1) % self->max_size == self->front)
        perror("Queue Overflow!!!");
    else if (DE_Queue_size(self) + arr_size > DE_Queue_max_size(self))
        perror("Queue Input Overflow!!!");
    else
        for (unsigned int i = 0; i < arr_size; i++)
            push_DE_Queue(self, data[i]);

    return self;
}

struct DE_Queue *unshift_DE_Queue(struct DE_Queue *self, type data)
{
    if ((self->back + 1) % self->max_size == self->front)
        perror("Queue Overflow!!!");
    else
    {
        if (self->front == 0)
            self->front = self->max_size - 1;
        else
            self->front--;
        self->arr[self->front] = data;
    }

    return self;
}

struct DE_Queue *unshift_DE_Queue_array(struct DE_Queue *self, type *data, unsigned int arr_size)
{
    if ((self->back + 1) % self->max_size == self->front)
        perror("Queue Overflow!!!");
    else if (DE_Queue_size(self) + arr_size > DE_Queue_max_size(self))
        perror("Queue Input Overflow!!!");
    else
        for (unsigned int i = 0; i < arr_size; i++)
            push_DE_Queue(self, data[i]);

    return self;
}

type pop_DE_Queue(struct DE_Queue *self)
{
    type return_data;

    if (self->front == self->back)
    {
        perror("Queue Underflow!!!");
        memset(&return_data, 0, sizeof(return_data));
    }
    else
    {
        if (self->back == 0)
            self->max_size - 1;
        else
            self->back--;

        return_data = self->arr[self->back];
    }

    return return_data;
}

struct DE_Queue *pop_DE_Queue_array(struct DE_Queue *self, unsigned int pop_size, type *return_arr)
{
    if (self->front == self->back)
        perror("Queue Underflow!!!");
    else
        for (unsigned char i = 0; i < pop_size; i++)
            if (return_arr && return_arr != NULL)
                return_arr[i] = pop_DE_Queue(self);
            else
                pop_DE_Queue(self);

    return self;
}

type shift_DE_Queue(struct DE_Queue *self)
{
    type return_data;

    if (self->front == self->back)
    {
        perror("Queue Underflow!!!");
        memset(&return_data, 0, sizeof(return_data));
    }
    else
    {
        return_data = self->arr[self->front];
        self->front = (self->front + 1) % self->max_size;
    }

    return return_data;
}

struct DE_Queue *shift_DE_Queue_array(struct DE_Queue *self, unsigned int shift_size, type *return_arr)
{
    if (self->front == self->back)
        perror("Queue Underflow!!!");
    else
        for (unsigned char i = 0; i < shift_size; i++)
            if (return_arr && return_arr != NULL)
                return_arr[i] = shift_DE_Queue(self);
            else
                shift_DE_Queue(self);

    return self;
}

struct DE_Queue construct_DE_Queue(unsigned int size)
{
    struct DE_Queue self;

    if (size < 1)
        perror("You can't create a Queue of size 0");
    else
    {
        self.max_size = size + 1;
        self.front = self.back = 0;
        self.arr = (type *)calloc(self.max_size, sizeof(struct DE_Queue));
    }

    return self;
}

struct DE_Queue construct_DE_Queue_initialized(type data, unsigned int size)
{
    struct DE_Queue self = construct_DE_Queue(size);

    self.arr[self.back] = data;
    self.back = (self.back + 1) % self.max_size;

    return self;
}

struct DE_Queue construct_DE_Queue_initialized_array(type *data, unsigned int arr_size, unsigned int size)
{
    struct DE_Queue self = construct_DE_Queue(size);

    memcpy(self.arr, data, arr_size * sizeof(type));
    self.back = arr_size;

    return self;
}

struct DE_Queue *destruct_DE_Queue(struct DE_Queue *self)
{
    memset(self->arr, 0, DE_Queue_size(self));
    if (self->arr && self->arr != NULL)
        free(self->arr);
    self->arr = NULL;

    self->back = self->front = 0;
    self->max_size = 0;

    return self;
}

#undef type

void display_DE_Queue_float(struct DE_Queue *self, unsigned char num_of_digits)
{
    unsigned int i = self->front;

    printf("%s", i == self->back ? "Empty Queue!!!\n" : "{");
    while (i != self->back)
    {
        printf("%.*f%s", num_of_digits, self->arr[i], (i + 1) % self->max_size == self->back ? "}\n" : ", ");
        i = (i + 1) % self->max_size;
    }
}

int main()
{
    printf("\nHello, World!\n\n");

    float arr[4] = {7.7598, 2.3441, 568.7457};

    struct DE_Queue self = construct_DE_Queue(5);
    struct DE_Queue self_ini = construct_DE_Queue_initialized(9.4356, 5);
    struct DE_Queue self_ini_arr = construct_DE_Queue_initialized_array(arr, 3, 5);

    printf("****CONSTRUCT****\n");

    display_DE_Queue_float(&self_ini, 4);
    display_DE_Queue_float(&self_ini_arr, 4);
    printf("\n");

    printf("****PUSH****\n");
    push_DE_Queue(&self, 4.312);
    push_DE_Queue(&self, 2.879);
    push_DE_Queue(&self, 655.756);

    display_DE_Queue_float(&self, 3);
    printf("\n");

    printf("****POP****\n");
    printf("Popped elements: {%.3f, %.3f}\n", pop_DE_Queue(&self), pop_DE_Queue(&self));

    display_DE_Queue_float(&self, 3);
    printf("\n");

    printf("****UNSHIFT****\n");
    unshift_DE_Queue(&self, 6.537);
    unshift_DE_Queue(&self, 8.259);

    display_DE_Queue_float(&self, 3);
    printf("\n");

    printf("****SHIFT****\n");
    printf("Shifted elements: {%.3f, %.3f}\n", shift_DE_Queue(&self), shift_DE_Queue(&self));

    display_DE_Queue_float(&self, 3);
    printf("\n");

    printf("****PUSH_ARRAY****\n");
    push_DE_Queue_array(&self, ((float[]){68.566, 521.432, 123.435}), 3);
    push_DE_Queue_array(&self_ini, ((float[]){69.1069, 832.4354, 87.758}), 3);
    push_DE_Queue_array(&self_ini_arr, ((float[]){2.5413, 12.4395}), 2);

    display_DE_Queue_float(&self, 3);
    display_DE_Queue_float(&self_ini, 4);
    display_DE_Queue_float(&self_ini_arr, 4);
    printf("\n");

    printf("****POP_ARRAY****\n");
    pop_DE_Queue_array(&self, 2, arr);

    printf("Popped elements: {");
    for (unsigned char i = 0; i < 2; i++)
        printf("%.3f%s", arr[i], i < 2 - 1 ? ", " : "}\n");
    display_DE_Queue_float(&self, 3);

    pop_DE_Queue_array(&self_ini, 2, arr);

    printf("Popped elements: {");
    for (unsigned char i = 0; i < 2; i++)
        printf("%.3f%s", arr[i], i < 2 - 1 ? ", " : "}\n");
    display_DE_Queue_float(&self_ini, 4);

    pop_DE_Queue_array(&self_ini_arr, 3, arr);

    printf("Popped elements: {");
    for (unsigned char i = 0; i < 3; i++)
        printf("%.3f%s", arr[i], i < 3 - 1 ? ", " : "}\n");
    display_DE_Queue_float(&self_ini_arr, 4);

    printf("\n");

    printf("****UNSHIFT_ARRAY****\n");
    unshift_DE_Queue_array(&self, ((float[]){68.566, 521.432, 123.435}), 3);
    unshift_DE_Queue_array(&self_ini, ((float[]){69.1069, 832.4354, 87.758}), 3);
    unshift_DE_Queue_array(&self_ini_arr, ((float[]){2.5413, 12.4395}), 3);

    display_DE_Queue_float(&self, 3);
    display_DE_Queue_float(&self_ini, 4);
    display_DE_Queue_float(&self_ini_arr, 4);
    printf("\n");

    printf("****SHIFT_ARRAY****\n");
    shift_DE_Queue_array(&self, 4, arr);

    printf("Shifted elements: {");
    for (unsigned char i = 0; i < 4; i++)
        printf("%.3f%s", arr[i], i < 4 - 1 ? ", " : "}\n");
    display_DE_Queue_float(&self, 3);

    shift_DE_Queue_array(&self_ini, 4, arr);

    printf("Shifted elements: {");
    for (unsigned char i = 0; i < 4; i++)
        printf("%.3f%s", arr[i], i < 4 - 1 ? ", " : "}\n");
    display_DE_Queue_float(&self_ini, 4);

    shift_DE_Queue_array(&self_ini_arr, 4, arr);

    printf("Shifted elements: {");
    for (unsigned char i = 0; i < 4; i++)
        printf("%.3f%s", arr[i], i < 4 - 1 ? ", " : "}\n");
    display_DE_Queue_float(&self_ini_arr, 4);

    printf("\n");

    destruct_DE_Queue(&self);

    return 0;
}