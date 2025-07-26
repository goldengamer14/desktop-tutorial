#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define inc_index(index, max) index = (index + 1) % max

#define type char

struct Queue
{
    size_t front_index;
    size_t back_index;
    size_t max_size;
    type *arr;
};

size_t Queue_size(struct Queue *self)
{
    return (self->back_index >= self->front_index ? self->back_index - self->front_index
                                                  : self->max_size - (self->front_index - self->back_index));
}

size_t Queue_max_size(struct Queue *self)
{
    return self->max_size;
}

struct Queue *destruct_Queue(struct Queue *self)
{
    if (self->arr && self->arr != NULL)
        free(self->arr);

    self->arr = NULL;
    self->max_size = self->front_index = self->back_index = 0;

    return self;
}

struct Queue construct_Queue(size_t size)
{
    struct Queue self;

    self.max_size = size;
    self.arr = (type *)malloc(sizeof(type) * self.max_size);

    self.back_index = self.front_index = 0;
    // memset((char *)self.arr + self.max_size, 0, 1);

    return self;
}

struct Queue construct_Queue_initialized(type data, size_t size)
{
    struct Queue self;
    if (size < 1)
    {
        printf("Input to Queue Constructor Overflow!!!\n");
        self.arr = NULL;
        self.max_size = self.front_index = self.back_index = 0;
    }
    else
    {
        self = construct_Queue(size);
        self.arr[self.back_index] = data;
        inc_index(self.back_index, self.max_size);
    }
    return self;
}

struct Queue construct_Queue_initialized_array(type *data, size_t data_size, size_t size)
{
    struct Queue self;
    if (data_size > size)
    {
        printf("Input to Queue Constructor Overflow!!!\n");
        self.arr = NULL;
        self.max_size = self.front_index = self.back_index = 0;
    }
    else
    {
        self = construct_Queue(size);

        // For some unknown reason, data[self.back_index++] causes memory corruption
        while (self.back_index < data_size)
        {
            self.arr[self.back_index] = data[self.back_index];
            inc_index(self.back_index, self.max_size);
        }
    }
    return self;
}

struct Queue *enqueue_Queue(struct Queue *self, type data)
{
    // In case of failure: (self->back_index + 1) % self->max_size == self->front_index
    if ((self->back_index + 1) % self->max_size == self->front_index || Queue_size(self) >= self->max_size) // - 1)
        printf("Queue Overflow!!! with Queue_size = %zu, max_size = %zu\n", Queue_size(self), self->max_size);
    else
    {
        self->arr[self->back_index] = data;
        inc_index(self->back_index, self->max_size);
    }

    return self;
}

struct Queue *enqueue_Queue_array(struct Queue *self, type *data, size_t data_size)
{
    if ((self->back_index + 1) % self->max_size == self->front_index)
        printf("Queue Overflow!!! with back = %zu, front = %zu, Queue_size = %zu, max_size = %zu\n", self->back_index, self->front_index, Queue_size(self), self->max_size);
    else if (Queue_size(self) + data_size > self->max_size) // - 1)
        printf("Queue Input Overflow!!! with back = %zu, front = %zu, Queue_size = %zu, data_size = %zu, max_size = %zu\n", self->back_index, self->front_index, Queue_size(self), data_size, self->max_size);
    else
        for (size_t i = 0; i < data_size; i++)
            enqueue_Queue(self, data[i]);

    return self;
}

type dequeue_Queue(struct Queue *self)
{
    type returning;
    if (Queue_size(self) < 1 || self->back_index == self->front_index)
    {
        printf("Queue Underflow!!! for Queue_size = %zu, max_size = %zu\n", Queue_size(self), self->max_size);
        memset(&returning, 0, sizeof(returning));
    }
    else
    {
        returning = self->arr[self->front_index];
        memset((char *)self->arr + self->front_index, 0, 1);
        inc_index(self->front_index, self->max_size);
    }

    return returning;
}

struct Queue *dequeue_Queue_array(struct Queue *self, size_t dequeue_size, type *holder)
{
    if (Queue_size(self) < 1 || self->back_index == self->front_index)
        printf("Queue Underflow!!! for Queue_size = %zu, dequeue_size = %zu, max_size = %zu\n", Queue_size(self), dequeue_size, self->max_size);
    else if (dequeue_size > Queue_size(self))
        printf("Queue Input Underflow!!! for back = %zu, front = %zu, Queue_size = %zu, dequeue_size = %zu, max_size = %zu\n", self->back_index, self->front_index, Queue_size(self), dequeue_size, self->max_size);
    else
    {
        // while (dequeue_size-- > 0)
        for (size_t i = 0; i < dequeue_size; i++)
            if (holder && holder != NULL)
                holder[i] = dequeue_Queue(self);
            else
                dequeue_Queue(self);
        // memcpy(holder, self->arr, dequeue_size);
    }

    return self;
}

#undef type

void display_Queue_char(struct Queue *self)
{
    size_t size = Queue_size(self);
    size_t i = self->front_index;

    if (size > 0)
        while (size-- > 0 && i != self->back_index)
        {
            printf("{%c, %i}", self->arr[i], i);
            inc_index(i, Queue_max_size(self));
        }
    else
        printf("Empty Queue!!!");

    printf("\tback = %zu, front = %zu, Queue_size = %zu, max_size = %zu\n", self->back_index, self->front_index, Queue_size(self), self->max_size);
}

#undef inc_index

int main()
{
    printf("\nHello, World!\n\n");

    // ****CONSTRUCT_QUEUE****

    printf("\t\t\t\t****CONSTRUCT_QUEUE****\n");

    char exp[10] = "cat";
    struct Queue que = construct_Queue(6);
    struct Queue que_ini = construct_Queue_initialized('c', 6);
    struct Queue que_ini_arr = construct_Queue_initialized_array(exp, strlen(exp), 6);

    printf("Queue que: ");
    display_Queue_char(&que);
    printf("Queue que_ini: ");
    display_Queue_char(&que_ini);
    printf("Queue que_ini_arr: ");
    display_Queue_char(&que_ini_arr);

    printf("\n");

    // ****ENQUEUE****

    printf("\t\t\t\t****ENQUEUE****\n");

    enqueue_Queue(&que, 'b');
    enqueue_Queue(&que_ini, 'a');
    enqueue_Queue(&que_ini_arr, 'e');

    printf("Queue que after 'b' enqueue: ");
    display_Queue_char(&que);
    printf("Queue que_ini after 'a' enqueue: ");
    display_Queue_char(&que_ini);
    printf("Queue que_ini_arr after 'e' enqueue: ");
    display_Queue_char(&que_ini_arr);

    printf("\n");

    // ****DEQUEUE****

    printf("\t\t\t\t****DEQUEUE****\n");

    printf("Queue que after '%c' dequeue: ", dequeue_Queue(&que));
    display_Queue_char(&que);
    printf("Queue que_ini after '%c' dequeue: ", dequeue_Queue(&que_ini));
    display_Queue_char(&que_ini);
    printf("Queue que_ini_arr after '%c' dequeue: ", dequeue_Queue(&que_ini_arr));
    display_Queue_char(&que_ini_arr);

    printf("\n");

    // ****ENQUEUE_ARRAY****

    printf("\t\t\t\t****ENQUEUE_ARRAY****\n");

    strcpy(exp, "andst");
    enqueue_Queue_array(&que, exp, strlen(exp));
    strcpy(exp, "ndye");
    enqueue_Queue_array(&que_ini, exp, strlen(exp));
    strcpy(exp, "sa");
    enqueue_Queue_array(&que_ini_arr, exp, strlen(exp));

    printf("Queue que after \"ands\" enqueue_array: ");
    display_Queue_char(&que);
    printf("Queue que_ini after \"ndy\" enqueue_array: ");
    display_Queue_char(&que_ini);
    printf("Queue que_ini_arr after \"s\" enqueue_array: ");
    display_Queue_char(&que_ini_arr);

    printf("\n");

    // ****DEQUEUE_ARRAY****

    printf("\t\t\t\t****DEQUEUE_ARRAY****\n");

    dequeue_Queue_array(&que, 4, exp);
    printf("Queue que after \"%.4s\" dequeue_array: ", exp); // Can pass `NULL` and it'll work
    display_Queue_char(&que);

    dequeue_Queue_array(&que_ini, 3, exp);
    printf("Queue que_ini after \"%.3s\" dequeue_array: ", exp);
    display_Queue_char(&que_ini);

    dequeue_Queue_array(&que_ini_arr, 2, exp);
    printf("Queue que_ini_arr after \"%.2s\" dequeue_array: ", exp);
    display_Queue_char(&que_ini_arr);

    printf("\n");

    // // ****INDEX CHECK****

    printf("\t\t\t\t****INDEX_CHECKING****\n");

    strcpy(exp, "arte");
    enqueue_Queue_array(&que, exp, strlen(exp));
    printf("Queue que after \"%s\" enqueue_array: ", exp);
    display_Queue_char(&que);

    strcpy(exp, "ork");
    enqueue_Queue_array(&que_ini, exp, strlen(exp));
    printf("Queue que_ini after \"%s\" enqueue_array: ", exp);
    display_Queue_char(&que_ini);

    strcpy(exp, "te");
    enqueue_Queue_array(&que_ini_arr, exp, strlen(exp));
    printf("Queue que_ini_arr after \"%s\" enqueue_array: ", exp);
    display_Queue_char(&que_ini_arr);

    printf("\n");

    dequeue_Queue_array(&que, 4, exp);
    printf("Queue que after \"%.4s\" dequeue_array: ", exp);
    display_Queue_char(&que);

    dequeue_Queue_array(&que_ini, 4, exp);
    printf("Queue que_ini after \"%.4s\" dequeue_array: ", exp);
    display_Queue_char(&que_ini);

    dequeue_Queue_array(&que_ini_arr, 4, exp);
    printf("Queue que_ini_arr after \"%.4s\" dequeue_array: ", exp);
    display_Queue_char(&que_ini_arr);

    // ****DESTRUCT_QUEUE****

    printf("\n\t\t\t\t****DESTRUCT_QUEUE****\n");

    destruct_Queue(&que);
    destruct_Queue(&que_ini);
    destruct_Queue(&que_ini_arr);

    return 0;
}