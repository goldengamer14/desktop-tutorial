#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "..\Bubble_Sort\Bubble_Sort.h"
#include "Merge_Sort.h"

Bubble_Sort_ascend_c(int)
    Merge_Sort_ascend(int)

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

    printf("arr_1: ");
    for (unsigned char i = 0; i < n_1; i++)
        printf("%i%s", arr_1[i], i < n_1 - 1 ? ", " : "}\n");

    printf("arr_2: ");
    for (unsigned char i = 0; i < n_2; i++)
        printf("%i%s", arr_2[i], i < n_2 - 1 ? ", " : "}\n");

    Merge_Sort_ascend_int(arr_1, arr_2, n_1, n_2, arr_3);

    printf("arr_3: ");
    for (unsigned char i = 0; i < n_1 + n_2; i++)
        printf("%i%s", arr_3[i], i < n_1 + n_2 - 1 ? ", " : "}\n");

    putc('\n', stdout);

    return 0;
}