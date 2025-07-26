#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Queue_type.c"

DECLARE_Queue(char)
#define integer int
    DECLARE_Queue(integer)
#undef integer

        void display_Queue_char(struct Queue_char *self)
{
    if (Queue_char_size(self) < 1)
        printf("Empty Queue!!!");
    else
    {
        size_t i = self->front_index;
        size_t size = Queue_char_size(self);
        while (size-- > 0)
        {
            printf("%c", self->arr[i]);
            if (i == self->max_size - 1)
                i = 0;
            else
                i++;
        }
    }
    // printf("\n");
    printf("\n\twith back = %d, front = %d, Queue_size = %d, max_size = %d, rotate = %d\n", self->back_index, self->front_index, Queue_char_size(self), self->max_size, self->rotate);
}

void display_Queue_integer(struct Queue_integer *self)
{
    if (Queue_integer_size(self) < 1)
        printf("Empty Queue!!!");
    else
    {
        size_t i = self->front_index;
        size_t size = Queue_integer_size(self);
        printf("{");
        while (size > 0)
        {
            printf("%i%s", self->arr[i], size-- > 1 ? ", " : "}");
            if (i == self->max_size - 1)
                i = 0;
            else
                i++;
        }
    }
    printf("\n\twith back = %d, front = %d, Queue_size = %d, max_size = %d, rotate = %d\n", self->back_index, self->front_index, Queue_integer_size(self), self->max_size, self->rotate);
}

int main()
{
    printf("\n\nHello, World!\n\n");

    // ****CONSTRUCT_QUEUE****

    char exp[6] = "cat";
    struct Queue_char que = construct_Queue_char(5);
    struct Queue_char que_ini = construct_Queue_char_initialized('c', 5);
    struct Queue_char que_ini_arr = construct_Queue_char_initialized_array(exp, strlen(exp), 5);

    struct Queue_integer que_int = construct_Queue_integer_initialized(45, 7);

    printf("Queue que: ");
    display_Queue_char(&que);
    printf("Queue que_ini: ");
    display_Queue_char(&que_ini);
    printf("Queue que_ini_arr: ");
    display_Queue_char(&que_ini_arr);

    printf("\nQueue que_int: ");
    display_Queue_integer(&que_int);

    printf("\n");

    // ****ENQUEUE****

    enqueue_Queue_char(&que, 'b');
    enqueue_Queue_char(&que_ini, 'a');
    enqueue_Queue_char(&que_ini_arr, 'e');

    enqueue_Queue_integer(&que_int, 12);

    printf("Queue que after 'b' enqueue: ");
    display_Queue_char(&que);
    printf("Queue que_ini after 'a' enqueue: ");
    display_Queue_char(&que_ini);
    printf("Queue que_ini_arr after 'e' enqueue: ");
    display_Queue_char(&que_ini_arr);

    printf("\nQueue que_int after 12 enqueue: ");
    display_Queue_integer(&que_int);

    printf("\n");

    // ****ENQUEUE_ARRAY****

    strcpy(exp, "ands");
    printf("%d ", strlen(exp));
    enqueue_Queue_char_array(&que, exp, strlen(exp));
    strcpy(exp, "ndy");
    printf("%d ", strlen(exp));
    enqueue_Queue_char_array(&que_ini, exp, strlen(exp));
    strcpy(exp, "s");
    printf("%d ", strlen(exp));
    enqueue_Queue_char_array(&que_ini_arr, exp, strlen(exp));

    int arr_int[3] = {4, 31, 423};
    enqueue_Queue_integer_array(&que_int, arr_int, 3);

    printf("Queue que after \"ands\" enqueue_array: ");
    display_Queue_char(&que);
    printf("Queue que_ini after \"ndy\" enqueue_array: ");
    display_Queue_char(&que_ini);
    printf("Queue que_ini_arr after \"s\" enqueue_array: ");
    display_Queue_char(&que_ini_arr);

    printf("\nQueue que_int after {4, 31, 423} enqueue_array: ");
    display_Queue_integer(&que_int);

    printf("\n");

    // ****DEQUEUE****

    dequeue_Queue_char(&que);
    dequeue_Queue_char(&que_ini);
    dequeue_Queue_char(&que_ini_arr);

    dequeue_Queue_integer(&que_int);

    printf("Queue que after 'b' dequeue: ");
    display_Queue_char(&que);
    printf("Queue que_ini after 'c' dequeue: ");
    display_Queue_char(&que_ini);
    printf("Queue que_ini_arr after 'c' dequeue: ");
    display_Queue_char(&que_ini_arr);

    printf("\nQueue que_int after 45 dequeue: ");
    display_Queue_integer(&que_int);

    printf("\n");

    // ****DEQUEUE_ARRAY****

    dequeue_Queue_char_array(&que, 4);
    dequeue_Queue_char_array(&que_ini, 3);
    dequeue_Queue_char_array(&que_ini_arr, 2);

    dequeue_Queue_integer_array(&que_int, 4);

    printf("\n");

    printf("Queue que after \"ands\" dequeue_array: ");
    display_Queue_char(&que);
    printf("Queue que_ini after \"and\" dequeue_array: ");
    display_Queue_char(&que_ini);
    printf("Queue que_ini_arr after \"at\" dequeue_array: ");
    display_Queue_char(&que_ini_arr);

    printf("\nQueue que_int after {12, 4, 31, 423} dequeue_array: ");
    display_Queue_integer(&que_int);

    printf("\n");

    // ****INDEXING CHECK****

    strcpy(exp, "large");
    enqueue_Queue_char_array(&que, exp, strlen(exp));
    strcpy(exp, "orky");
    enqueue_Queue_char_array(&que_ini, exp, strlen(exp));
    strcpy(exp, "ket");
    enqueue_Queue_char_array(&que_ini_arr, exp, strlen(exp));

    arr_int[0] = 89;
    arr_int[1] = 65;
    arr_int[2] = 290;
    enqueue_Queue_integer_array(&que_int, arr_int, 3);

    printf("Queue que: ");
    display_Queue_char(&que);
    printf("Queue que_ini: ");
    display_Queue_char(&que_ini);
    printf("Queue que_ini_arr: ");
    display_Queue_char(&que_ini_arr);

    printf("\nQueue que_int after {89, 65, 290} enqueue_array: ");
    display_Queue_integer(&que_int);

    printf("\n");

    // ****DESTRUCT_QUEUE****

    destruct_Queue_char(&que);
    destruct_Queue_char(&que_ini);
    destruct_Queue_char(&que_ini_arr);

    destruct_Queue_integer(&que_int);

    return 0;
}