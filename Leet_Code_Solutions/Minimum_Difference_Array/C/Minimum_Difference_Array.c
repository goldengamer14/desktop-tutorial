#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap(int *a, int *b)
{
    *a += *b;
    *b = *a - *b;
    *a -= *b;
}

void bubble_sort_min(int **array, int index)
{
    do
        if (*array[index] < *array[index - 1])
            swap(array[index], array[index - 1]);
        else
            return;
    while (--index > 0);
}

void bubble_sort_max(int **array, int index)
{
    do
        if (*array[index] > *array[index - 1])
            swap(array[index], array[index - 1]);
        else
            return;
    while (--index > 0);
}

#define n numsSize / 3

long long minimumDifference(int *nums, int numsSize)
{
    if (numsSize % 3 != 0)
    {
        perror("The Array size must be a multiple of 3");
        return ~0;
    }

    int *max_from_left = nums;
    int *min_from_right = nums + numsSize - 1;

    int *maxes_to_remove[n / 2 + 1];
    int *mins_to_remove[n / 2 + 1];

    memset(maxes_to_remove, 0, sizeof(int *) * (n / 2 + 1));
    memset(mins_to_remove, 0, sizeof(int *) * (n / 2 + 1));

    int index = 0;

    do
    {
        if (!maxes_to_remove[index] || maxes_to_remove[index] == NULL || maxes_to_remove[index] == 0)
        {
            maxes_to_remove[index] = max_from_left;
            mins_to_remove[index] = min_from_right;
        }
        else
        {
            maxes_to_remove[index] = (*max_from_left > *maxes_to_remove[index]
                                          ? max_from_left
                                          : maxes_to_remove[index]);
            mins_to_remove[index] = (*min_from_right < *mins_to_remove[index] ? min_from_right : mins_to_remove[index]);
        }

        if (index > 0 && maxes_to_remove[index] > maxes_to_remove[index - 1])
            bubble_sort_max(maxes_to_remove, index);
        if (index > 0 && mins_to_remove[index] < mins_to_remove[index - 1])
            bubble_sort_min(mins_to_remove, index);

        printf("maxes_to_remove = %i, mins_to_remove = %i\n", *maxes_to_remove[index], *mins_to_remove[index]);
        printf("left = %x -> %i, right = %x -> %i\n", max_from_left - nums, *(max_from_left), min_from_right - nums, *(min_from_right));
        max_from_left++;
        min_from_right--;
        if (index < n / 2 - 1)
            index++;
        printf("difference = %i\n\n", (int)(min_from_right - max_from_left));
    } while ((int)(min_from_right - max_from_left) > 0);

    for (int i = 0; i <= index; i++)
        printf("maxes[%i] = %i; mins[%i] = %i;\n", i, *maxes_to_remove[i], i, *mins_to_remove[i]);
    printf("\n");

    long long plus_left = 0, plus_right = 0;
    max_from_left = nums;
    min_from_right = nums + numsSize - 1;

    int dirty_value = 0;
    memset((char *)&dirty_value + sizeof(int) - 1, 0x80, 1);

    if (n % 2)
    {
        if (abs(*maxes_to_remove[index]) > abs(*mins_to_remove[index]))
            *maxes_to_remove[index] = dirty_value;
        else
            *mins_to_remove[index] = dirty_value;
        index--;
    }

    for (int i = 0; i <= index; i++)
        *maxes_to_remove[i] = *mins_to_remove[i] = dirty_value;

    do
    {
        printf("max_from_left = %x -> %i; min_from_right = %x -> %i\n",
               max_from_left - nums, *max_from_left, min_from_right - nums, *min_from_right);

        if (*max_from_left == dirty_value)
        {
            max_from_left++;
            printf("max_from_left = %x -> %i; plus_left = %i\n\n", max_from_left - nums, *max_from_left, plus_left);
            continue;
        }
        if (*min_from_right == dirty_value)
        {
            min_from_right--;
            printf("min_from_right = %x -> %i; plus_right = %i\n\n", min_from_right - nums, *min_from_right, plus_right);
            continue;
        }

        plus_left += *max_from_left;
        plus_right += *min_from_right;

        max_from_left++;
        min_from_right--;

        printf("plus_left = %lli; plus_right = %lli;\n\n", plus_left, plus_right);
    } while ((int)(min_from_right - max_from_left) >= 0);

    return plus_left - plus_right;
}

#undef n

int main()
{
    printf("\nHello, World!\n\n");

    {
        int nums[] = {7, 9, 5, 8, 1, 3};
        printf("%lli\n\n", minimumDifference(nums, 6));
    }
    {
        int nums[] = {3, 1, 2};
        printf("%lli\n", minimumDifference(nums, 3));
    }
    return 0;
}