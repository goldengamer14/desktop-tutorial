#define Bubble_Sort_ascend_c(num_type)                                     \
    num_type *Bubble_Sort_ascend_##num_type(num_type *arr, unsigned int n) \
    {                                                                      \
        if (n > 1)                                                         \
        {                                                                  \
            signed char swapped;                                           \
            do                                                             \
            {                                                              \
                swapped = 0;                                               \
                for (unsigned int j = 0; j < n - 1; j++)                   \
                    if (arr[j] > arr[j + 1])                               \
                    {                                                      \
                        arr[j + 1] += arr[j];                              \
                        arr[j] = arr[j + 1] - arr[j];                      \
                        arr[j + 1] -= arr[j];                              \
                        swapped = 1;                                       \
                    }                                                      \
            } while (--n > 1 && swapped);                                  \
        }                                                                  \
        return arr;                                                        \
    }
