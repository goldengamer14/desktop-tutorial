int countValidSelections(const int *const nums, const int numsSize)
{
    int glob_curr = 0, curr = 0, times_zero = 0;
    int *copy_nums = (int *)calloc(numsSize * 2, sizeof(int));
    const int *zero_nums = copy_nums + numsSize;
    signed char move;

    while (glob_curr < numsSize)
    {
        while (glob_curr < numsSize && nums[glob_curr] > 0)
            glob_curr++;
        if (glob_curr >= numsSize)
            break;

        memcpy(copy_nums, nums, numsSize * sizeof(int));
        curr = glob_curr;
        move = 1;

        // RIGHT LOOP
        while (curr >= 0 && curr < numsSize)
        {
            // if (copy_nums[curr] == 0)
            //     {curr += move;}
            // else {
            if (copy_nums[curr] > 0)
            {
                printf("for curr = %i; move = %i changed to ", curr, move);
                copy_nums[curr]--;
                move *= -1;
                printf("move = %i\n", move);
                // curr += move;
            }
            curr += move;
        }
        if (memcmp(copy_nums, zero_nums, numsSize * sizeof(int)) == 0)
            times_zero++;

        printf("After RIGHT LOOP glob_curr=%i:\n", glob_curr);
        printf("times_zero = %i; move = %i\n", times_zero, move);
        for (int i = 0; i < numsSize; i++)
            printf("copy_nums[%i] = %i\n", i, copy_nums[i]);

        memcpy(copy_nums, nums, numsSize * sizeof(int));
        curr = glob_curr;
        move = -1;

        // LEFT LOOP
        while (curr >= 0 && curr < numsSize)
        {
            // if (copy_nums[curr] == 0)
            //     {curr += move;}
            // else {
            if (copy_nums[curr] > 0)
            {
                printf("for curr = %i; move = %i changed to ", curr, move);
                copy_nums[curr]--;
                move *= -1;
                printf("move = %i\n", move);
                // curr += move;
            }
            curr += move;
        }

        if (memcmp(copy_nums, zero_nums, numsSize * sizeof(int)) == 0)
            times_zero++;

        printf("After LEFT LOOP glob_curr=%i:\n", glob_curr);
        printf("times_zero = %i; move = %i\n", times_zero, move);
        for (int i = 0; i < numsSize; i++)
            printf("copy_nums[%i] = %i\n", i, copy_nums[i]);

        glob_curr++;
    }

    free(copy_nums);
    return times_zero;
}