#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "..\Bubble_Sort\Bubble_Sort.h"
#include "Merge_Sort.h"

Bubble_Sort_ascend_c(int)
    Merge_Sort_ascend(int)
        Bubble_Sort_ascend_c(char)
            Merge_Sort_ascend(char)

                int main()
{
    printf("\nHello, World!\n\n");

    int arr_1[] = {832, 34, 66, 26, 74, 23, 412, 24, 52, 79, 72, 136};
    int arr_2[] = {180, 36, 52, 274, 41, 401, 101, 26, 386, 112, 46, 174};
    unsigned int n_1 = sizeof(arr_1) / sizeof(int);
    unsigned int n_2 = sizeof(arr_2) / sizeof(int);
    int arr_3[n_1 + n_2];

    Bubble_Sort_ascend_int(arr_1, n_1);
    Bubble_Sort_ascend_int(arr_2, n_2);

    printf("arr_1: {");
    for (unsigned char i = 0; i < n_1; i++)
        printf("%i%s", arr_1[i], i < n_1 - 1 ? ", " : "}\n");

    printf("arr_2: {");
    for (unsigned char i = 0; i < n_2; i++)
        printf("%i%s", arr_2[i], i < n_2 - 1 ? ", " : "}\n");

    Merge_Sort_ascend_int(arr_1, arr_2, n_1, n_2, arr_3);

    printf("arr_3: {");
    for (unsigned char i = 0; i < n_1 + n_2; i++)
        printf("%i%s", arr_3[i], i < n_1 + n_2 - 1 ? ", " : "}\n");

    putc('\n', stdout);

    char arr_1_char[] = "Merge Sort Algorithm example";
    char arr_2_char[] = " Bubble Sort Algorithm used too!";
    unsigned int n_char_1 = sizeof(arr_1_char);
    unsigned int n_char_2 = sizeof(arr_2_char);
    char arr_3_char[n_char_1 + n_char_2 + 1];

    Bubble_Sort_ascend_char(arr_1_char, n_char_1);
    Bubble_Sort_ascend_char(arr_2_char, n_char_2);

    printf("arr_1_char: {");
    for (unsigned char i = 0; i < n_char_1; i++)
        printf("%c%s", arr_1_char[i], i < n_char_1 - 1 ? ", " : "}\n");

    printf("arr_2_char: {");
    for (unsigned char i = 0; i < n_char_2; i++)
        printf("%c%s", arr_2_char[i], i < n_char_2 - 1 ? ", " : "}\n");

    Merge_Sort_ascend_char(arr_1_char, arr_2_char, n_char_1, n_char_2, arr_3_char);

    printf("arr_3_char: {");
    for (unsigned char i = 0; i < n_char_1 + n_char_2; i++)
        printf("%c%s", arr_3_char[i], (i < n_char_1 + n_char_2 - 1 ? ", " : "}\n"));
    arr_3_char[n_char_1 + n_char_2] = '\0';

    unsigned char print_index = 0;
    while ((int)arr_3_char[print_index] < 33)
        print_index++;
    printf("print_ptr: {");
    for (unsigned char i = print_index; i < n_char_1 + n_char_2; i++)
        printf("%c%s", arr_3_char[i], (i < n_char_1 + n_char_2 - 1 ? ", " : "}\n"));
    printf("%s", arr_3_char + print_index);

    putc('\n', stdout);

    return 0;
}