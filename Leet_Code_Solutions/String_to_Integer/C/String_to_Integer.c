#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

// Runtime -> 0ms, Beats 100.00%
// Memory -> 8.60MB, Beats 42.57%


int myAtoi(char *s)
{
    int64_t integer = 0;
    char *ptr = s;
    signed char sign = 1;

    if (*ptr == '-')
    {
        sign = -1;
        ptr++;
    }
    else if (*ptr == '+')
        ptr++;
    else
        while (*ptr == ' ')
        {
            ptr++;
            if (*ptr == '-')
            {
                sign = -1;
                ptr++;
                break;
            }
            else if (*ptr == '+')
            {
                ptr++;
                break;
            }
        }

    for (ptr; *ptr >= '0' && *ptr <= '9'; ptr++)
    {
        integer = integer * 10 + ((*ptr) - '0');
        if (integer >= pow(2, 31))
            if (sign < 0)
                return pow(2, 31) * -1;
            else
                return pow(2, 31) - 1;
    }

    integer *= sign;
    return integer;
}

int main()
{
    printf("\nHello, World!\n\n");

    char s[200];

    strcpy(s, "42");
    printf("myAtoi(\"%s\") = %i\n", s, myAtoi(s));

    strcpy(s, " -042");
    printf("myAtoi(\"%s\") = %i\n", s, myAtoi(s));

    strcpy(s, "1337c0d3");
    printf("myAtoi(\"%s\") = %i\n", s, myAtoi(s));

    strcpy(s, "0-1");
    printf("myAtoi(\"%s\") = %i\n", s, myAtoi(s));

    sprintf(s, "-%lli", (long long int)(pow(2, 31)));
    printf("myAtoi(\"%s\") = %i\n", s, myAtoi(s));

    sprintf(s, "%lli", (long long int)pow(2, 31));
    printf("myAtoi(\"%s\") = %i\n", s, myAtoi(s));

    sprintf(s, "-%lli", (long long int)pow(2, 31));
    printf("myAtoi(\"%s\") = %i\n", s, myAtoi(s));

    sprintf(s, "%lli", (long long int)pow(2, 32));
    printf("myAtoi(\"%s\") = %i\n", s, myAtoi(s));

    sprintf(s, "-%lli", (long long int)pow(2, 32));
    printf("myAtoi(\"%s\") = %i\n", s, myAtoi(s));

    strcpy(s, "+-12");
    printf("myAtoi(\"%s\") = %i\n", s, myAtoi(s));

    strcpy(s, "   +0 123");
    printf("myAtoi(\"%s\") = %i\n", s, myAtoi(s));

    strcpy(s, "words and 987");
    printf("myAtoi(\"%s\") = %i\n", s, myAtoi(s));

    strcpy(s, "20000000000000000000");
    printf("myAtoi(\"%s\") = %i\n", s, myAtoi(s));

    return 0;
}