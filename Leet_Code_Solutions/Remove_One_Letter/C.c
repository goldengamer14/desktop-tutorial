#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

bool equalFrequency(char *word)
{
    unsigned char letter_frequency[26] = {0};
    unsigned char freq_freq[9] = {0};

    for (word; *word != '\0'; word++)
        letter_frequency[(*word) - 'a']++;

    for (unsigned char i = 0; i < 26; i++)
        if (letter_frequency[i] > 0)
            freq_freq[letter_frequency[i]]++;

    signed char unequal;
    signed char atleast_once = 0;
    // signed char all_equal_and_1 = 1;
    for (unsigned char i = 0; i < 9; i++)
    {
        if (freq_freq[i] != 0)
        {
            unequal = 0;
            unsigned char compare, j = 0;
            if (freq_freq[i] == 1)
            {
                while (freq_freq[j] == 0 || j == i)
                    j++;
                compare = freq_freq[j];
            }
            else
                compare = freq_freq[i] - 1;
            // printf("compare initialized to %hhu; freq_freq[i=%hhu] = %hhu; "
            //        "freq_freq[j=%hhu] = %hhu\n",
            //        compare, i, freq_freq[i], j, freq_freq[j]);

            for (j; j < 9; j++)
                if (freq_freq[j] != 0 && j != i)
                {
                    atleast_once = 1;

                    // printf("freq_freq[%hhu] = %hhu; freq_freq[%hhu] = %hhu; "
                    //        "unequal = %hhu; atleast_once = %hhu;\n",
                    //        i, freq_freq[i], j, freq_freq[j], unequal,
                    //        atleast_once);
                    if (compare != freq_freq[j])
                    {
                        // if (freq_freq[i] != freq_freq[j] || j > 2 || (j == 2
                        // && freq_freq[2] > 1))
                        //     all_equal_and_1 = 0;
                        unequal = 1;
                        // printf("inequality at: compare = %hhu; "
                        //        "freq_freq[i=%hhu] = %hhu;"
                        //        " freq_freq[j=%hhu] = %hhu;\n",
                        //        compare, i, freq_freq[i], j, freq_freq[j]);
                        break;
                        // i++;
                        // if (i < 9) {
                        //     j = 0;
                        //     // break;
                        // } else {
                        //     for (unsigned char k = 0; k < 9; k++)
                        //         printf("freq_freq[%hhu] = %hhu\n", k,
                        //                freq_freq[k]);
                        //     printf("Returned at line 54 because unequal = "
                        //            "%hhu; i = %hhu; j = %hhu;\n",
                        //            unequal, i, j);
                        //     // if (all_equal_and_1)
                        //     //     return 1;
                        //     return !unequal;
                        // }
                    }
                    // if (all_equal_and_1)
                    //     return 1;
                    if (!unequal)
                    {
                        // for (unsigned char i = 0; i < 9; i++)
                        //     printf("freq_freq[%hhu] = %hhu\n", i, freq_freq[i]);
                        // printf("Returned at line 58 because: compare = %hhu; "
                        //        "freq_freq[i=%hhu] = %hhu; freq_freq[j=%hhu] = "
                        //        "%hhu;\n",
                        //        compare, i, freq_freq[i], j, freq_freq[j]);
                        return !unequal;
                    }
                }
        }
    }

    // for (unsigned char i = 0; i < 9; i++)
        // printf("freq_freq[%hhu] = %hhu\n", i, freq_freq[i]);
    // printf("unequal = %hhu; atleast_once = %hhu\n", unequal, atleast_once);

    // if (atleast_once && all_equal_and_1)
    //     return 1;
    if (atleast_once || freq_freq[1])
    {
        // printf("Returned at line 83 because unequal = %hhu", unequal);
        return !unequal;
    }
    return 0;
}