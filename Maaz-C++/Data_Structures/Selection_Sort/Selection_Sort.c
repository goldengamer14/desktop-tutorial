#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DECLARE_Selection_Sort_ascend(type)                                                \
    void swap_##type(type *a, type *b)                                                     \
    {                                                                                      \
        type temp = *a;                                                                    \
        *a = *b;                                                                           \
        *b = temp;                                                                         \
    }                                                                                      \
                                                                                           \
    type *Selection_Sort_ascend_##type(type *arr, unsigned int n)                          \
    {                                                                                      \
        if (n < 2)                                                                         \
        {                                                                                  \
            perror("Don't pass an empty or single array");                                 \
            return arr;                                                                    \
        }                                                                                  \
                                                                                           \
        unsigned int min_index;                                                            \
        unsigned int swap_index = 0;                                                       \
                                                                                           \
        do                                                                                 \
        {                                                                                  \
            min_index = swap_index + 1;                                                    \
            for (unsigned int i = min_index + 1; i < n; i++)                               \
                min_index = (arr[min_index] > arr[i] ? i : min_index);                     \
                                                                                           \
            if (min_index != swap_index && arr[swap_index] > arr[min_index])               \
                swap_##type(arr + swap_index, arr + min_index);                            \
                                                                                           \
            /* printf("After swapping index %u with index %u: {", swap_index, min_index);  \
            for (unsigned int i = 0; i < n; i++)                                           \
                printf("%i%s", arr[i], i < n - 1 ? ", " : "}");                            \
            printf("\n");                                                               */ \
            swap_index++;                                                                  \
        } while (swap_index < n - 1);                                                      \
                                                                                           \
        return arr;                                                                        \
    }

DECLARE_Selection_Sort_ascend(int)
    DECLARE_Selection_Sort_ascend(char)

        int main()
{
    printf("\nHello, World!\n\n");

    int arr[] = {64, 25, 12, 22, 11};
    unsigned char n = sizeof(arr) / sizeof(arr[0]);

    printf("Unsorted array:{");
    for (unsigned char i = 0; i < n; i++)
        printf("%i%s", arr[i], i < n - 1 ? ", " : "}");

    printf("\n");

    clock_t start = clock();
    Selection_Sort_ascend_int(arr, n);
    clock_t end = clock();

    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken to sort int array of size %u: %.6f seconds\n", n, time_spent);

    printf("Sorted array:{");
    for (unsigned char i = 0; i < n; i++)
        printf("%i%s", arr[i], i < n - 1 ? ", " : "}");
    printf("\n");

    printf("\n");

    char arr_char[] = {'d', 'a', 'c', 'b'};
    unsigned char n_char = sizeof(arr_char) / sizeof(arr_char[0]);

    printf("Unsorted char array:{");
    for (unsigned char i = 0; i < n_char; i++)
        printf("%c%s", arr_char[i], i < n_char - 1 ? ", " : "}");
    printf("\n");

    start = clock();
    Selection_Sort_ascend_char(arr_char, n_char);
    end = clock();

    time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken to sort char array of size %u: %.6f seconds\n", n_char, time_spent);

    printf("Sorted char array:{");
    for (unsigned char i = 0; i < n_char; i++)
        printf("%c%s", arr_char[i], i < n_char - 1 ? ", " : "}");
    printf("\n");

    printf("\n");

    int arr_empty[] = {};
    Selection_Sort_ascend_int(arr_empty, 0);

    int arr_single[] = {42};
    Selection_Sort_ascend_int(arr_single, 1);

    printf("\n");

    return 0;
}