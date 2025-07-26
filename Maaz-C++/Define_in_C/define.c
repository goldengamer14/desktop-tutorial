#include <stdio.h>
#include <stdlib.h>

#define max_value(data)                                      \
    {                                                        \
        typeof(data) largest_number = 0;                     \
        typeof(data) bit_size = 1;                           \
                                                             \
        for (int i = 0; i < sizeof(data) * 8 - 1; i++)       \
        {                                                    \
            bit_size *= 2;                                   \
            largest_number += bit_size;                      \
        }                                                    \
                                                             \
        data = largest_number;                               \
        printf("The number of bits integer holds is: %llu\n", data); \
    }

int main()
{
    printf("\n\nHello, World!\n\n");

    int a;
    max_value(a);

    printf("The maximum holding capability of integer is: %u\n\n", a);

    return 0;
}