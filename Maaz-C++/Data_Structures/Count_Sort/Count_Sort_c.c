#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *Count_Sort_ascend(int *arr, unsigned int n)
{
    int max = (0x80 << (sizeof(int) - 1) * 8);

    for (unsigned int i = 0; i < n; i++)
        if (max < arr[i])
            max = arr[i];

    int *frequency_arr = (int *)calloc(max + 1, sizeof(int));
    for (unsigned int i = 0; i < n; i++)
        frequency_arr[(unsigned int)arr[i]]++;

    unsigned int i = 0, j = 0;
    while (j <= max)
        if (frequency_arr[j] > 0)
        {
            arr[i] = j;
            frequency_arr[j]--;
            i++;
        }
        else
            j++;

    free(frequency_arr);
    return arr;
}

int main()
{
    printf("\nHello, World!\n\n");

    int arr[] = {227, 76, 70, 400, 340, 25, 433, 557, 578, 198, 28, 98};
    unsigned short int n = sizeof(arr) / sizeof(int);

    printf("Unsorted arr: {");
    for (unsigned char i = 0; i < n; i++)
        printf("%i%s", arr[i], (i < n - 1 ? ", " : "}\n"));

    Count_Sort_ascend(arr, n);

    printf("Sorted arr: {");
    for (unsigned char i = 0; i < n; i++)
        printf("%i%s", arr[i], (i < n - 1 ? ", " : "}\n"));

    putchar('\n');

    return 0;
}