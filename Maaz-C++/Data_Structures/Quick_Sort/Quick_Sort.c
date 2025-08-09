#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DECLARE_Quick_Sort_ascend(type)                                            \
    void swap_##type(type *a, type *b)                                             \
    {                                                                              \
        type temp = *a;                                                            \
        *a = *b;                                                                   \
        *b = temp;                                                                 \
    }                                                                              \
                                                                                   \
    unsigned int partition_##type(type *arr, unsigned int low, unsigned int high)  \
    {                                                                              \
        type pivot = arr[low];                                                     \
        unsigned int i = low + 1, j = high;                                        \
                                                                                   \
        do                                                                         \
        {                                                                          \
            while (arr[i] < pivot)                                                 \
                i++;                                                               \
            while (arr[j] > pivot)                                                 \
                j--;                                                               \
            if (i < j)                                                             \
                swap_##type(arr + i, arr + j);                                     \
        } while (i < j);                                                           \
                                                                                   \
        swap_##type(arr + low, arr + j);                                           \
        return j;                                                                  \
    }                                                                              \
                                                                                   \
    type *Quick_Sort_ascend_##type(type *arr, unsigned int low, unsigned int high) \
    {                                                                              \
        if (low < high)                                                            \
        {                                                                          \
            unsigned int partition_index = partition_##type(arr, low, high);       \
            Quick_Sort_ascend_##type(arr, low, partition_index - 1);               \
            Quick_Sort_ascend_##type(arr, partition_index + 1, high);              \
        }                                                                          \
        return arr;                                                                \
    }

DECLARE_Quick_Sort_ascend(int)
    DECLARE_Quick_Sort_ascend(char)
        DECLARE_Quick_Sort_ascend(float)

            int main()
{
    printf("\nHello, World!\n\n");

    int arr[] = {3, 6, 8, 10, 1, 2, 1};
    unsigned char n = sizeof(arr) / sizeof(arr[0]);

    printf("Unsorted array: {");
    for (unsigned char i = 0; i < n; i++)
        printf("%i%s", arr[i], (i < n - 1) ? ", " : "}");
    printf("\n");

    clock_t start = clock();
    Quick_Sort_ascend_int(arr, 0, n);
    clock_t end = clock();

    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken to sort int array of size %i: %lf seconds\n", n, time_spent);

    printf("Sorted array: {");
    for (unsigned char i = 0; i < n; i++)
        printf("%i%s", arr[i], (i < n - 1) ? ", " : "}");
    printf("\n");

    printf("\n");

    char arr_char[] = "quick sort example";
    unsigned char n_char = strlen(arr_char);

    printf("Unsorted char array: {");
    for (unsigned char i = 0; i < n_char; i++)
        printf("%c%s", arr_char[i], i < n_char - 1 ? ", " : "}");
    printf("\n");

    start = clock();
    Quick_Sort_ascend_char(arr_char, 0, n_char);
    end = clock();

    time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken to sort char array of size %i: %lf seconds\n", n_char, time_spent);

    printf("Sorted char array: {");
    for (unsigned char i = 0; i < n_char; i++)
        printf("%c%s", arr_char[i], i < n_char - 1 ? ", " : "}\n\t\t   {");
    for (unsigned char i = 0; i < n_char; i++)
        printf("%i%s", arr_char[i], i < n_char - 1 ? ", " : "}");
    printf("\n");

    printf("\n");

    float arr_float[] = {3.1, 6.2, 8.3, 10.4, 1.5, 2.6, 1.7};
    unsigned char n_float = sizeof(arr_float) / sizeof(arr_float[0]);

    printf("Unsorted float array: {");
    for (unsigned char i = 0; i < n_float; i++)
        printf("%.1f%s", arr_float[i], (i < n_float - 1) ? ", " : "}");
    printf("\n");

    start = clock();
    Quick_Sort_ascend_float(arr_float, 0, n_float);
    end = clock();

    time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken to sort float array of size %i: %lf seconds\n", n_float, time_spent);

    printf("Sorted float array: {");
    for (unsigned char i = 0; i < n_float; i++)
        printf("%.1f%s", arr_float[i], (i < n_float - 1) ? ", " : "}");
    printf("\n");

    printf("\n");

    return 0;
}