#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

typedef unsigned short int hu;
typedef signed char hhi;

int main()
{
    printf("\nHello, World!\n\n");

    char int_24[sizeof(hu) + sizeof(hhi)];
    *(hu *)int_24 = ~(hu)0;
    *(hhi *)(int_24 + sizeof(hu)) = ~(hhi)0 - (hhi)0x80;

    printf("unsigned short int = %hu; signed char = %hhi\n", *(hu *)int_24, *(hhi *)(int_24 + sizeof(hu)));
    printf("unsigned short int = 0x%hx; signed char = 0x%hhx\n", *(hu *)int_24, *(hhi *)(int_24 + sizeof(hu)));

    putchar('\n');

    printf("unsigned short int = %hu; signed char = %hhi\n", USHRT_MAX, SCHAR_MAX);
    printf("unsigned short int = 0x%hx; signed char = 0x%hhx\n", USHRT_MAX, SCHAR_MAX);

    putchar('\n');

    printf("int_24 = %i\n", (int)(USHRT_MAX + (pow(2, 8 * sizeof(hu)) * SCHAR_MAX)));
    printf("int_24 = 0x%x\n", (int)(USHRT_MAX + (pow(2, 8 * sizeof(hu)) * SCHAR_MAX)));

    putchar('\n');

    printf("int_24 = %i\n", (*(hu *)int_24) | ((*(hhi *)(int_24 + sizeof(hu))) << 16));
    printf("int_24 = 0x%x\n", (*(hu *)int_24) | ((*(hhi *)(int_24 + sizeof(hu))) << 16));

    putchar('\n');

    return 0;
}