#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct My_Struct
{
    short int a;
    char b;
};

struct My_Struct_2
{
    short int a;
    char b;
    unsigned short int c;
};

int main()
{
    printf("\nHello, World!\n\n");

    char memory[64];
    memset(memory, 0x00, 64);

    unsigned short int *ui = (unsigned short int *)memory;
    *ui = 74;

    printf("unsigned int: %hu of %hu\n", *(unsigned short int *)memory, ~(unsigned short int)0);

    struct My_Struct *str = (struct My_Struct *)(memory + sizeof(ui));
    *str = (struct My_Struct){.a = 12, .b = '@'};

    // printf("My_Struct: {%hi, %c} of {%hi, %c}\n\n", ((struct My_Struct *)(memory + sizeof(ui)))->a, ((struct My_Struct *)(memory + sizeof(ui)))->b, (short int)0x7fff, (char)0x7e);
    printf("My_Struct: {%hi, %c} of {%hi, %c}\n", str->a, ((struct My_Struct *)(memory + sizeof(ui)))->b, (short int)0x7fff, (char)0x7e);

    struct My_Struct_2 *str_2 = (struct My_Struct_2 *)memory;
    *str_2 = (struct My_Struct_2){.a = -1843, .b = 'I', .c = 1947};

    printf("My_Struct_2: {%hi, %c, %hu} of {+-%hi, %c, %hu}\n\n", str_2->a, str_2->b, ((struct My_Struct_2 *)memory)->c, (short int)0x7fff, (char)0x7e, (unsigned short int)0xffff);

    size_t used_memory = sizeof(ui) + sizeof(str) + sizeof(str_2);
    char rest_memory[64 - used_memory];

    memcpy(rest_memory, memory + used_memory, 64 - used_memory);
    printf("Rest of the memory: %s\n\n", rest_memory);

    printf("Compare the first 3 bytes of str & str_2: %i\n", memcmp(str, str_2, 3));
    printf("Compare the first 3 bytes of str_2 & str: %i\n", memcmp(str_2, str, 3));
    printf("Compare the 3rd byte of str & str_2: %i\n", memcmp(&(str->b), &(str_2->b), 1));
    printf("Compare the 3rd byte of str_2 & str: %i\n", memcmp(&(str_2->b), &(str->b), 1));
    printf("Compare the first 2 bytes of str & ui: %i\n\n", memcmp(str, ui, 2));

    return 0;
}