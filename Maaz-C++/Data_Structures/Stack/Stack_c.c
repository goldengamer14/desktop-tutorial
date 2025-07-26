#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef type
#define type char
#endif

struct Stack
{
    type *arr;
    size_t arr_size;
    size_t max_size;
};

struct Stack construct_Stack_type(size_t max_size)
{
    struct Stack self;
    self.arr = (type *)malloc(sizeof(type) * max_size);
    self.arr_size = 0;
    self.max_size = max_size;

    return self;
}

struct Stack construct_Stack_type_initialized(type data, size_t max_size)
{
    struct Stack self;
    self.arr = (type *)malloc(sizeof(type) * max_size);
    self.arr[0] = data;
    self.arr_size = 1;
    self.max_size = max_size;

    return self;
}

struct Stack construct_Stack_type_initialized_array(type *data, size_t arr_size, size_t max_size)
{
    struct Stack self;
    self.arr = (type *)malloc(sizeof(type) * max_size);
    for (size_t i = 0; i < arr_size; i++)
        self.arr[i] = data[i];
    self.arr_size = arr_size;
    self.max_size = max_size;

    return self;
}

type peek_Stack_type(struct Stack *self, size_t index)
{
    if (index > self->max_size - 1)
    {
        printf("Stack Underflow!\n");
        return self->arr[index];
    }
    else if (index > self->arr_size - 1)
    {
        printf("Stack Unused!\n");
        return self->arr[index];
    }
    return self->arr[index];
}

struct Stack *push_Stack_type(struct Stack *self, type data)
{
    if (self->arr_size >= self->max_size)
        printf("Stack Overflow!!!\n");
    else
        self->arr[self->arr_size++] = data;

    return self;
}

struct Stack *push_Stack_type_array(struct Stack *self, type *data, size_t data_size)
{
    if (self->arr_size >= self->max_size)
        printf("Stack Overflow!!!\n");
    else if (self->arr_size + data_size > self->max_size)
        printf("Push Overflow!!!\n");
    else
        for (size_t i = 0; i < data_size; i++)
            self->arr[self->arr_size++] = data[i];

    return self;
}

struct Stack *pop_Stack_type(struct Stack *self)
{
    if (self->arr_size < 1)
        printf("Stack Underflow!!!");
    else
        self->arr_size--;

    return self;
}

struct Stack *pop_Stack_type_array(struct Stack *self, size_t pop_size)
{
    if (self->arr_size < 1)
        printf("Stack Underflow!!!\n");
    else if (self->arr_size - pop_size < 1)
        printf("Pop Underflow!!!\n");
    else
        while (pop_size-- > 0)
            self->arr_size--;

    return self;
}

void destruct_Stack_type(struct Stack *self)
{
    if (self->arr != NULL)
    {
        free(self->arr);
        self->arr = NULL;
    }
    self->arr_size = self->max_size = 0;
}

void display_Stack(struct Stack *self)
{
    for (size_t i = 0; i < self->arr_size; i++)
        printf("%c", self->arr[i]);
    printf(" -> %i\n", self->arr_size);
}

#undef type

int main()
{
    printf("\n\nHello, World!\n\n");

    /*                      ****CONSTRUCT_STACK****                 */
    char exp[] = "dattebayo!";
    struct Stack stack = construct_Stack_type_initialized_array(exp, strlen(exp), 20);

    printf("Stack: ");
    display_Stack(&stack);

    /*                      ****PUSH && PUSH_ARRAY****                 */
    push_Stack_type(&stack, ' ');
    strcpy(exp, "Naruto");
    push_Stack_type_array(&stack, exp, strlen(exp));

    printf("Stack after ' ' push and \"Naruto\" push_array: ");
    display_Stack(&stack);

    /*                      ****POP && POP_ARRAY****                 */
    pop_Stack_type(&stack);
    pop_Stack_type_array(&stack, strlen(" Narut"));

    printf("Stack after 'o' pop and \" Narut\" pop_array: ");
    display_Stack(&stack);

    /*                      ****PEEK****                 */
    printf("Peeking Stack: ");
    for (int i = 0; i < stack.arr_size ; i++)
        printf("%c", peek_Stack_type(&stack, i));
    printf("\n");

    destruct_Stack_type(&stack);
    printf("\n");

    return 0;
}