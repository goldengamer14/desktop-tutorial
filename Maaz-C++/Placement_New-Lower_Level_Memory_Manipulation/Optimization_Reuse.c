#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define hu unsigned short int
#define hi short int
#define cu unsigned char

typedef struct
{
    hu a;
    char b;
} My_Struct;

typedef struct
{
    double a;
    hi b;
} My_Struct_2;

int main()
{
    printf("\nHello, World!\n\n");

    printf("%d + %d = %d; %d + %d = %d\n\n", sizeof(hu), sizeof(char), sizeof(My_Struct), sizeof(double), sizeof(hi), sizeof(My_Struct_2));

    My_Struct str;
    My_Struct_2 str_2;
    char used_str[2];
    memset(used_str, 0x00, 2);

    printf("Type hu = ");
    scanf(" %hu", &str.a);
    printf("; c = ");
    scanf(" %c", &str.b);
    // scanf("Type hu = %hu; c = %c", &str.a, &str.b);

    printf("\nType lf = ");
    scanf(" %lf", &(str_2.a));
    printf("; hi = ");
    scanf(" %hi", &(str_2.b));
    // scanf("Type lf = %lf; hi = %hi", &str_2.a, &str_2.b);

    printf("\n\nThis is the str: a = %hu; b = %c", str.a, str.b);
    printf("\nThis is the str_2: a = %lf; b = %hi\n\n", str_2.a, str_2.b);

    printf("Now type a string[2]: ");
    scanf(" %2s", (char *)&str);
    printf("\nNow type an hu: ");
    scanf(" %hu", (hu *)(&str_2));
    // scanf("Now type a string[2]: %.2s", (char *)(&str + 2));
    // scanf("Now type an hu: %hu", (hu *)(&str_2));
    memset(used_str, 0x02, 1);
    memset(used_str + 1, 0x02, 1);

    printf("\n\n This is the str: %.2s", (char *)&str);
    printf("\nThis is the str_2: %hu\n", *(hu *)(&str_2));

    printf("\nNow type a char: ");
    scanf(" %c", (char *)((char *)&str + *(cu *)used_str));
    printf("\nNow type an int: ");
    scanf(" %i", (int *)((char *)&str_2 + *(cu *)(used_str + 1)));
    // scanf("\nNow type a string[2]: %.2s", (char *)(&str + *(hu *)used_str - *(hu *)(used_str + 1)));
    // scanf("\nNow type an int: %i", (int *)(&str_2 + *(hu *)(used_str + 1)));
    memset((char *)&str + *(cu *)used_str + 1, 0x00, 1);
    (*(cu *)used_str) += 2;
    (*(cu *)(used_str + 1))++;

    printf("\n\nThe Final Result:\n");
    printf("\tMy_Struct = %.4s\n", (char *)(&str));
    printf("\tMy_Struct_2 = {hu = %hu, int = %i}\n", *(hu *)&str_2, *(int *)((char *)&str_2 + sizeof(hu)));
    printf("\t used_str = %d; used_str_2 = %d\n\n", *(cu *)used_str, *(cu *)(used_str + 1));

    return 0;
}

#undef hu
#undef hi
#undef cu