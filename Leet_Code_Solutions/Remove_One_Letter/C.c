#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

bool equalFrequency(char *word)
{
    unsigned char letter_frequency[26] = {0};
    for (word; *word != '\0'; word++)
        letter_frequency[(int)(*word - 'a')]++;

    // for (unsigned char i = 0; i < 26; i++)
    //     printf("letter_frequency[i=%hhu] = %hhu\n", i, letter_frequency[i]);

    signed char unequal = 0;
    unsigned char i = 0;
    // for (unsigned char i = 0; i < 26; i++)
    while (i < 26)
    {
        if (i >= 26)
            break;
        while (i < 26 && letter_frequency[i] == 0)
        {
            i++;
            continue;
        }
        if (i >= 26)
            break;

        unequal = 0;
        unsigned char compare;
        unsigned char j = 0;
        if (letter_frequency[i] == 1)
        {
            while (j < 26 && (j == i || letter_frequency[j] == 0))
                j++;

            if (j >= 26 || j == i)
            {
                // printf("j exceeded 26 or is equal to i as %hhu\n", j);
                // compare = letter_frequency[i];
                return 1;
            }
            else
                compare = letter_frequency[j];
        }
        else
            compare = letter_frequency[i] - 1;

        // printf("Outer Loop 1:\n\tcompare = %hhu; letter_frequency[i=%hhu] = %hhu; letter_frequency[j=%hhu] = "
        //        "%hhu; unequal = %hhi\n",
        //        compare, i, letter_frequency[i], j, j < 26 ? letter_frequency[j] : 0, unequal);

        // printf("\nInner Loop:\n");

        j = 0;
        while (j < 26)
        {
            if (j >= 26)
                break;
            while (j < 26 && letter_frequency[j] == 0)
            {
                j++;
                continue;
            }
            if (j >= 26)
                break;

            unequal = 0;

            if (j != i && compare != letter_frequency[j])
            {
                unequal = 1;
                break;
            }
            // printf("\tcompare = %hhu; letter_frequency[j=%hhu] = %hhu;"
            //        "unequal "
            //        "= %hhi\n",
            //        compare, j, j < 26 ? letter_frequency[j] : 0, unequal);
            j++;
        }

        // printf("\nOuter Loop 2:\n\tcompare = %hhu; letter_frequency[j=%hhu] = "
        //        "%hhu; unequal = %hhi\n",
        //        compare, j, j < 26 ? letter_frequency[j] : 0, unequal);

        if (!unequal)
        {
            // printf("\n\nReturned inside the loop\n");
            return 1;
        }

        i++;
    }

    // printf("\n\nReturned at the end\n");
    return 0;
}

int main()
{
    char str[200];
    printf("\n\n");
    fflush(stdout);

    strcpy(str, "abcc");
    printf("equalFrequency(%s) = %hhi\n\n", str, equalFrequency(str));

    strcpy(str, "aazz");
    printf("equalFrequency(%s) = %hhi\n\n", str, equalFrequency(str));

    strcpy(str, "bac");
    printf("equalFrequency(%s) = %hhi\n\n", str, equalFrequency(str));

    strcpy(str, "abbcc");
    printf("equalFrequency(%s) = %hhi\n\n", str, equalFrequency(str));

    strcpy(str, "ddaccb");
    printf("equalFrequency(%s) = %hhi\n\n", str, equalFrequency(str));

    strcpy(str, "aca");
    printf("equalFrequency(%s) = %hhi\n\n", str, equalFrequency(str));

    strcpy(str, "acbda");
    printf("equalFrequency(%s) = %hhi\n\n", str, equalFrequency(str));

    strcpy(str, "cbccca");
    printf("equalFrequency(%s) = %hhi\n\n", str, equalFrequency(str));

    strcpy(str, "cccd");
    printf("equalFrequency(%s) = %hhi\n\n", str, equalFrequency(str));

    strcpy(str, "xxvbv");
    printf("equalFrequency(%s) = %hhi\n\n", str, equalFrequency(str));

    printf("\n");

    return 0;
}