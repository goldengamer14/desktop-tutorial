#define Merge_Sort_ascend(type)                                                                               \
    type *Merge_Sort_ascend_##type(type *arr_1, type *arr_2, unsigned int n_1, unsigned int n_2, type *arr_3) \
    {                                                                                                         \
        unsigned int i = 0, j = 0, k = 0;                                                                     \
        if (!arr_3)                                                                                           \
        {                                                                                                     \
            perror("Give a valid array for copition");                                                        \
            return NULL;                                                                                      \
        }                                                                                                     \
                                                                                                              \
        while (i < n_1 && j < n_2)                                                                            \
            if (arr_1[i] < arr_2[j])                                                                          \
                arr_3[k++] = arr_1[i++];                                                                      \
            else                                                                                              \
                arr_3[k++] = arr_2[j++];                                                                      \
                                                                                                              \
        while (i < n_1)                                                                                       \
            arr_3[k++] = arr_1[i++];                                                                          \
        while (j < n_2)                                                                                       \
            arr_3[k++] = arr_2[j++];                                                                          \
                                                                                                              \
        return arr_3;                                                                                         \
    }
