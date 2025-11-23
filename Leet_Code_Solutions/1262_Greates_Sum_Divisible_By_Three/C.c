static inline void swap(int* restrict a, int* b) {
    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
}

static int partition(int* arr, int low, int high) {
    int pivot = arr[low], i = low + 1, j = high;

    do {
        while (arr[i] <= pivot && i < high)
            i++;

        while (arr[j] > pivot && j > low)
            j--;

        if (i < j)
            swap(arr + i, arr + j);
    } while (i < j);

    swap(arr + low, arr + j);
    return j;
}

static void Quick_Sort(int* arr, int low, int high) {
    if (low < high) {
        int part = partition(arr, low, high);
        Quick_Sort(arr, low, part - 1);
        Quick_Sort(arr, part + 1, high);
    }
}

int maxSumDivThree(int* nums, int numsSize) {
    int sum = 0;

    for (int i = 0; i < numsSize; i++)
        sum += nums[i];

    printf("sum = %i\n\n", sum);

    if (sum % 3) {
        int i = 0, const_nums_i = 0;
        unsigned int minus_sum = 0;
        Quick_Sort(nums, 0, numsSize - 1);

        while (sum % 3 && i < numsSize) {
            if (sum % 3 == nums[i] % 3) {
                printf("Changing sum = %i; nums[i=%i] = %i\n", sum, i, nums[i]);
                if (!const_nums_i)
                    const_nums_i = nums[i];
            }

            if ((sum - minus_sum) % 3) {
                minus_sum += nums[i];
                printf("sum = %i; minus_sum = %i\n", sum, minus_sum);
            }

            if (const_nums_i && const_nums_i < minus_sum) {
                printf("Returning sum = %i; const_nums_i = %i\n", sum,
                       const_nums_i);
                return sum - const_nums_i;
            } else if (minus_sum < nums[i]) {
                printf("Returning sum = %i; minus_sum = %i\n", sum, minus_sum);
                return sum - minus_sum;
            }

            i++;
        }
    }

    return (sum % 3 ? 0 : sum);
}

#include<limits.h>
// Solution
int maxSumDivThree(int* nums, int numsSize) {
    int sum = 0;

    int min1 = INT_MAX, min2 = INT_MAX;
    int min11 = INT_MAX, min22 = INT_MAX;

    for (int i = 0; i < numsSize; i++) {
        int x = nums[i];
        sum += x;
        int r = x % 3;

        if (r == 1) {
            if (x < min1) { min11 = min1; min1 = x; }
            else if (x < min11) min11 = x;
        } else if (r == 2) {
            if (x < min2) { min22 = min2; min2 = x; }
            else if (x < min22) min22 = x;
        }
    }

    int rem = sum % 3;

    if (rem == 0) return sum;

    if (rem == 1) {
        int r1 = min1;
        int r2 = (min2 == INT_MAX || min22 == INT_MAX) ? INT_MAX : min2 + min22;
        int remove = r1 < r2 ? r1 : r2;
        return remove == INT_MAX ? 0 : sum - remove;
    } else {
        int r1 = min2;
        int r2 = (min1 == INT_MAX || min11 == INT_MAX) ? INT_MAX : min1 + min11;
        int remove = r1 < r2 ? r1 : r2;
        return remove == INT_MAX ? 0 : sum - remove;
    }
}