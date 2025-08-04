#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Bubble_Sort_ascend_c(num_type)                                     \
    num_type *Bubble_Sort_ascend_##num_type(num_type *arr, unsigned int n) \
    {                                                                      \
        if (n > 1)                                                         \
        {                                                                  \
            signed char swapped;                                           \
            do                                                             \
            {                                                              \
                swapped = 0;                                               \
                for (unsigned int j = 0; j < n - 1; j++)                   \
                    if (arr[j] > arr[j + 1])                               \
                    {                                                      \
                        arr[j + 1] += arr[j];                              \
                        arr[j] = arr[j + 1] - arr[j];                      \
                        arr[j + 1] -= arr[j];                              \
                        swapped = 1;                                       \
                    }                                                      \
            } while (--n > 1 && swapped);                                  \
        }                                                                  \
        return arr;                                                        \
    }

Bubble_Sort_ascend_c(double)
    Bubble_Sort_ascend_c(int)

        int main()
{
    printf("\nHello, World!\n\n");

    int arr_int[] = {64, 34, 25, 12, -59, 22, 11, 90};
    int n = sizeof(arr_int) / sizeof(arr_int[0]);
    Bubble_Sort_ascend_int(arr_int, n);

    printf("Sorted integer array: \n{");
    for (int i = 0; i < n; i++)
        printf("%i%s", arr_int[i], (i < n - 1) ? ", " : "}\n");
    printf("\n");

    double arr_double[] = {64.5, 34.2, 25.1, 12.3, 22.4, 11.0, 90.6, -43.2};
    n = sizeof(arr_double) / sizeof(arr_double[0]);
    Bubble_Sort_ascend_double(arr_double, n);

    printf("Sorted double array: \n{");
    for (int i = 0; i < n; i++)
        printf("%.1f%s", arr_double[i], (i < n - 1) ? ", " : "}\n");

    printf("\n");

    return 0;
}