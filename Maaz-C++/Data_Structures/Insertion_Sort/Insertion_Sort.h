#define Insertion_Sort_Ascend(type)                                       \
    type *Insert_Sort_ascend_##type(type *arr, type data, unsigned int n) \
    {                                                                     \
        while (n > 0 && arr[n - 1] > data)                                \
        {                                                                 \
            arr[n] = arr[n - 1];                                          \
            n--;                                                          \
        }                                                                 \
        arr[n] = data;                                                    \
        return arr;                                                       \
    }
