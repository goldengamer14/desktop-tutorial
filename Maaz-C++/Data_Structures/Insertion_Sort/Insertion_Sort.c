#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Insert_Ascend(type)                                          \
    type *Insert_ascend_##type(type *arr, type data, unsigned int n) \
    {                                                                \
        while (n > 0 && arr[n - 1] > data)                           \
        {                                                            \
            arr[n] = arr[n - 1];                                     \
            n--;                                                     \
        }                                                            \
        arr[n] = data;                                               \
        return arr;                                                  \
    }

Insert_Ascend(int)
    Insert_Ascend(float)

    // int *Insert_ascend_int(int *arr, int data, unsigned int n)
    // {
    //     while (n > 0 && arr[n - 1] > data)
    //     {
    //         arr[n] = arr[n - 1];
    //         n--;
    //     }
    //     arr[n] = data;
    //     return arr;
    // }

    // float *Insert_ascend_float(float *arr, float data, unsigned int n)
    // {
    //     while (n > 0 && arr[n - 1] > data)
    //     {
    //         arr[n] = arr[n - 1];
    //         n--;
    //     }
    //     arr[n] = data;
    //     return arr;
    // }

    int main()
{
    printf("\nHello, World!\n\n");

    int arr_int[12];
    int insertion_sequence[12];
    for (unsigned int i = 0; i < 12; i++)
    {
        int insert_num = (int)(rand()) % 100;
        insertion_sequence[i] = insert_num;
        Insert_ascend_int(arr_int, insert_num, i);
    }

    printf("arr_int: {");
    for (unsigned int i = 0; i < 12; i++)
        printf("%i%s", arr_int[i], i < 11 ? ", " : "}\n");

    printf("insertion_sequence: {");
    for (unsigned int i = 0; i < 12; i++)
        printf("%i%s", insertion_sequence[i], i < 11 ? ", " : "}\n\n");

    float arr_float[12];
    float insertion_sequence_float[12];

    for (unsigned int i = 0; i < 12; i++)
    {
        int rand_num = (int)(rand());
        float insert_num = rand_num % 100 + rand_num / (float)(RAND_MAX);
        insertion_sequence_float[i] = insert_num;
        Insert_ascend_float(arr_float, insert_num, i);
    }

    printf("arr_float: {");
    for (unsigned int i = 0; i < 12; i++)
        printf("%f%s", arr_float[i], i < 11 ? ", " : "}\n");
    printf("insertion_sequence_float: {");

    for (unsigned int i = 0; i < 12; i++)
        printf("%f%s", insertion_sequence_float[i], i < 11 ? ", " : "}\n\n");

    return 0;
}