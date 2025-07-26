#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Double_Ended_Queue_type.h"

DECLARE_DE_Queue(float)

    void display_DE_Queue_float(struct DE_Queue_float *self, unsigned char num_of_digits)
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

    struct DE_Queue_float self = construct_DE_Queue_float(5);
    struct DE_Queue_float self_ini = construct_DE_Queue_float_initialized(9.4356, 5);
    struct DE_Queue_float self_ini_arr = construct_DE_Queue_float_initialized_array(arr, 3, 5);

    printf("****PUSH****\n");
    push_DE_Queue_float(&self, 4.312);
    push_DE_Queue_float(&self, 2.879);
    push_DE_Queue_float(&self, 655.756);

    display_DE_Queue_float(&self, 3);
    printf("\n");

    printf("****POP****\n");
    printf("Popped elements: {%.3f, %.3f}\n", pop_DE_Queue_float(&self), pop_DE_Queue_float(&self));

    display_DE_Queue_float(&self, 3);
    printf("\n");

    printf("****UNSHIFT****\n");
    unshift_DE_Queue_float(&self, 6.537);
    unshift_DE_Queue_float(&self, 8.259);

    display_DE_Queue_float(&self, 3);
    printf("\n");

    printf("****SHIFT****\n");
    printf("Shifted elements: {%.3f, %.3f}\n", shift_DE_Queue_float(&self), shift_DE_Queue_float(&self));

    display_DE_Queue_float(&self, 3);
    printf("\n");

    printf("****PUSH_ARRAY****\n");
    push_DE_Queue_float_array(&self, ((float[]){68.566, 521.432, 123.435}), 3);
    push_DE_Queue_float_array(&self_ini, ((float[]){69.1069, 832.4354, 87.758}), 3);
    push_DE_Queue_float_array(&self_ini_arr, ((float[]){2.5413, 12.4395}), 2);

    display_DE_Queue_float(&self, 3);
    display_DE_Queue_float(&self_ini, 4);
    display_DE_Queue_float(&self_ini_arr, 4);
    printf("\n");

    printf("****POP_ARRAY****\n");
    pop_DE_Queue_float_array(&self, 2, arr);

    printf("Popped elements: {");
    for (unsigned char i = 0; i < 2; i++)
        printf("%.3f%s", arr[i], i < 2 - 1 ? ", " : "}\n");
    display_DE_Queue_float(&self, 3);

    pop_DE_Queue_float_array(&self_ini, 2, arr);

    printf("Popped elements: {");
    for (unsigned char i = 0; i < 2; i++)
        printf("%.3f%s", arr[i], i < 2 - 1 ? ", " : "}\n");
    display_DE_Queue_float(&self_ini, 4);

    pop_DE_Queue_float_array(&self_ini_arr, 3, arr);

    printf("Popped elements: {");
    for (unsigned char i = 0; i < 3; i++)
        printf("%.3f%s", arr[i], i < 3 - 1 ? ", " : "}\n");
    display_DE_Queue_float(&self_ini_arr, 4);

    printf("\n");

    printf("****UNSHIFT_ARRAY****\n");
    unshift_DE_Queue_float_array(&self, ((float[]){68.566, 521.432, 123.435}), 3);
    unshift_DE_Queue_float_array(&self_ini, ((float[]){69.1069, 832.4354, 87.758}), 3);
    unshift_DE_Queue_float_array(&self_ini_arr, ((float[]){2.5413, 12.4395}), 3);

    display_DE_Queue_float(&self, 3);
    display_DE_Queue_float(&self_ini, 4);
    display_DE_Queue_float(&self_ini_arr, 4);
    printf("\n");

    printf("****SHIFT_ARRAY****\n");
    shift_DE_Queue_float_array(&self, 4, arr);

    printf("Shifted elements: {");
    for (unsigned char i = 0; i < 4; i++)
        printf("%.3f%s", arr[i], i < 4 - 1 ? ", " : "}\n");
    display_DE_Queue_float(&self, 3);

    shift_DE_Queue_float_array(&self_ini, 4, arr);

    printf("Shifted elements: {");
    for (unsigned char i = 0; i < 4; i++)
        printf("%.3f%s", arr[i], i < 4 - 1 ? ", " : "}\n");
    display_DE_Queue_float(&self_ini, 4);

    shift_DE_Queue_float_array(&self_ini_arr, 4, arr);

    printf("Shifted elements: {");
    for (unsigned char i = 0; i < 4; i++)
        printf("%.3f%s", arr[i], i < 4 - 1 ? ", " : "}\n");
    display_DE_Queue_float(&self_ini_arr, 4);

    printf("\n");

    destruct_DE_Queue_float(&self);

    return 0;
}