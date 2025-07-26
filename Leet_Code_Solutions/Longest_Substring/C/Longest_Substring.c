#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int lengthOfLongestSubstring(char *s)
{
    unsigned int length = strlen(s) + 1;
    unsigned int max_length = 0;

    unsigned int truth_value;
    unsigned short int str_len;

    char subs[length];
    strcpy(subs, "\0");

    if (length <= 1)
        return max_length;

    unsigned int ind = 0;
    while (ind < length)
    {
        for (unsigned int i = ind; i < length; i++)
        {
            if (s[i] == '\0')
                continue;

            truth_value = (strchr(subs, (*(s + i))) != NULL);
            // printf("%c at i = %u %s present in %s from ind = %u\n", s[i], i, (truth_value ? "was " : "wasn't "), subs, ind);

            if (truth_value)
            {
                str_len = strlen(subs);
                if (str_len > max_length)
                    max_length = str_len;
                strcpy(subs, "\0");
                break;
            }
            else
                strncat(subs, s + i, 1);
        }
        if (truth_value)
            ind++;
    }

    // printf("\n%s", s);
    return max_length;
}

int main()
{
    printf("\nHello, World!\n\n");

    unsigned int size;
    char exp[(int)5e4];

    strcpy(exp, "abcabcbb");
    size = lengthOfLongestSubstring(exp);
    printf("\nsize = %u\n\n", size);

    strcpy(exp, "bbbbb");
    size = lengthOfLongestSubstring(exp);
    printf("\nsize = %u\n\n", size);

    strcpy(exp, "pwwkew");
    size = lengthOfLongestSubstring(exp);
    printf("\nsize = %u\n\n", size);

    strcpy(exp, " ");
    size = lengthOfLongestSubstring(exp);
    printf("\nsize = %u\n\n", size);

    strcpy(exp, "dvdf");
    size = lengthOfLongestSubstring(exp);
    printf("\nsize = %u\n\n", size);

    strcpy(exp, "");
    size = lengthOfLongestSubstring(exp);
    printf("\nsize = %u\n\n", size);

    strcpy(exp, "tmmzuxt");
    size = lengthOfLongestSubstring(exp);
    printf("\nsize = %u\n\n", size);

    return 0;
}