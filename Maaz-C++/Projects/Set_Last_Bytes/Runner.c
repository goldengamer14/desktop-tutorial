#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Set_Last_Bytes.h"

int main()
{
    printf("\nHello, World!\n\n");

    printf("size of long int: %d\nMaximum long int: %li\n\n", sizeof(long int), set_last_bytes(0x7f, long int, 8));
    printf("size of unsigned long int: %d\nMaximum unsigned long int: %lu\n\n", sizeof(unsigned long int), set_last_bytes(0x7f, unsigned long int, 8));

    printf("char: %c = (0x40)%c\n", set_last_bytes(0x41, char, 8), 0x41);

    return 0;
}
