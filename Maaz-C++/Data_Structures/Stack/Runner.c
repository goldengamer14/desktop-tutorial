#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Stack_list.c"

typedef struct
{
    size_t Number;
    char id;
} STACK_ELEMENT;

DECLARE_Stack(char)
    DECLARE_Stack(int)
        DECLARE_Stack(STACK_ELEMENT)

            void display_Stack_char(struct Stack_char *self)
{
    for (size_t i = 0; i < self->arr_size; i++)
        printf("%c", self->arr[i]);
    printf(" -> %i\n", self->arr_size);
}

void display_Stack_int(struct Stack_int *self)
{
    printf("{");
    for (size_t i = 0; i < self->arr_size; i++)
        printf("%i%s", self->arr[i], i < self->arr_size - 1 ? ", " : "\0");
    printf("} -> %i\n", self->arr_size);
}

void display_Stack_STACK_ELEMENT(struct Stack_STACK_ELEMENT *self)
{
    printf("{");
    for (size_t i = 0; i < self->arr_size; i++)
        printf("{.Number = %zu, .id = '%c'}%s", self->arr[i].Number, self->arr[i].id, i < self->arr_size - 1 ? ", " : "\0");
    printf("} -> %i\n", self->arr_size);
}

int main()
{
    printf("\n\nHello, World!\n\n");

    /*                      ****CONSTRUCT_STACK****                 */
    char exp[] = "dattebayo!";
    struct Stack_char stack = construct_Stack_char_initialized_array(exp, strlen(exp), 20);
    struct Stack_int stack_int = construct_Stack_int_initialized(45, 5);
    struct Stack_STACK_ELEMENT stack_ele = construct_Stack_STACK_ELEMENT_initialized((STACK_ELEMENT){.Number = 98238, .id = 'c'}, 5);

    printf("Stack: ");
    display_Stack_char(&stack);
    printf("Stack_int: ");
    display_Stack_int(&stack_int);
    printf("Stack_STACK_ELEMENT: ");
    display_Stack_STACK_ELEMENT(&stack_ele);

    printf("\n");

    /*                      ****PUSH && PUSH_ARRAY****                 */
    push_Stack_char(&stack, ' ');
    push_Stack_int(&stack_int, 12);
    push_Stack_STACK_ELEMENT(&stack_ele, (STACK_ELEMENT){.Number = 63421, .id = 'x'});

    strcpy(exp, "Naruto");
    push_Stack_char_array(&stack, exp, strlen(exp));
    int exp_int[] = {99, 88, 56};
    push_Stack_int_array(&stack_int, exp_int, 3);
    STACK_ELEMENT exp_ele[] = {{.Number = 67438, .id = 'n'}, {.Number = 13290, .id = 'a'}, {.Number = 72034, .id = 'j'}};
    push_Stack_STACK_ELEMENT_array(&stack_ele, exp_ele, 3);

    printf("Stack after ' ' push and \"Naruto\" push_array: ");
    display_Stack_char(&stack);
    printf("Stack_int after 12 push and {99, 88, 56} push_array: ");
    display_Stack_int(&stack_int);
    printf("Stack_STACK_ELEMENT after {.Number = 63421, .id = 'x'} push and {{.Number = 67438, .id = 'n'}, %s",
           "{.Number = 13290, .id = 'a'}, {.Number = 72034, .id = 'j'}} push_array:\n\t");
    display_Stack_STACK_ELEMENT(&stack_ele);

    printf("\n");

    /*                      ****POP && POP_ARRAY****                 */
    pop_Stack_char(&stack);
    pop_Stack_int(&stack_int);
    pop_Stack_STACK_ELEMENT(&stack_ele);

    pop_Stack_char_array(&stack, strlen(" Narut"));
    pop_Stack_int_array(&stack_int, 2);
    pop_Stack_STACK_ELEMENT_array(&stack_ele, 2);

    printf("Stack after 'o' pop and \" Narut\" pop_array: ");
    display_Stack_char(&stack);
    printf("Stack_int after '56' pop and {88, 99} pop_array: ");
    display_Stack_int(&stack_int);
    printf("Stack_STACK_ELEMENT after {.Number = 72034, .id = 'j'} pop and {{.Number = 13290, .id = 'a'}, %s",
           "{.Number = 67438, .id = 'n'}} pop_array:\n\t");
    display_Stack_STACK_ELEMENT(&stack_ele);

    printf("\n");

    /*                      ****PEEK****                 */
    printf("Peeking Stack: ");
    for (int i = 0; i < stack.arr_size; i++)
        printf("%c", peek_Stack_char(&stack, i));
    printf("\n");

    printf("Peeking Stack_int: {");
    for (int i = 0; i < stack_int.arr_size; i++)
        printf("%i%s", peek_Stack_int(&stack_int, i), i < stack_int.arr_size - 1 ? ", " : "\0");
    printf("}\n");

    printf("Peeking Stack_STACK_ELEMENT: {");
    for (size_t i = 0; i < stack_ele.arr_size; i++)
        printf("{.Number = %zu, .id = '%c'}%s", stack_ele.arr[i].Number, stack_ele.arr[i].id, i < stack_ele.arr_size - 1 ? ", " : "\0");
    printf("}\n");

    destruct_Stack_char(&stack);
    destruct_Stack_int(&stack_int);
    destruct_Stack_STACK_ELEMENT(&stack_ele);
    printf("\n");

    return 0;
}