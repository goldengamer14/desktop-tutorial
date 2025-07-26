#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int lengthOfLongestSubstring(char *s)
{
    if (memchr(s, '\0', 2))
        return strlen(s);

    char *front = s, *back = s, *found;
    unsigned int max_length = 0;
    unsigned int current_length;

    while (*(++back) != '\0')
    {
        if ((found = (char *)memchr(front, *back, current_length = (unsigned int)(back - front))) == NULL)
            continue;
        // printf("max_length = %u, current_length = %u match not found\n", max_length, current_length);
        else
        {
            max_length = (max_length < current_length ? current_length : max_length);
            front = found + 1;

            // printf("max_length = %u, current_length = %u match found\n", max_length, current_length);
        }
    }
    current_length = (unsigned int)(back - front);
    return (max_length < current_length ? current_length : max_length);
}

int main()
{
    printf("\nHello, World!\n\n");

    unsigned int size;
    char exp[20];

    strcpy(exp, "abcabcbb");
    size = lengthOfLongestSubstring(exp);
    printf("\n%s size = %u\n\n", exp, size);

    strcpy(exp, "bbbbb");
    size = lengthOfLongestSubstring(exp);
    printf("\n%s size = %u\n\n", exp, size);

    strcpy(exp, "pwwkew");
    size = lengthOfLongestSubstring(exp);
    printf("\n%s size = %u\n\n", exp, size);

    strcpy(exp, " ");
    size = lengthOfLongestSubstring(exp);
    printf("\n%s size = %u\n\n", exp, size);

    strcpy(exp, "dvdf");
    size = lengthOfLongestSubstring(exp);
    printf("\n%s size = %u\n\n", exp, size);

    strcpy(exp, "");
    size = lengthOfLongestSubstring(exp);
    printf("\n%s size = %u\n\n", exp, size);

    strcpy(exp, "tmmzuxt");
    size = lengthOfLongestSubstring(exp);
    printf("\n%s size = %u\n\n", exp, size);

    return 0;
}