typedef char *struct_arr_t;

// #define temp_str TEMPORARY_STRUCTURE_ARRAY_TYPE
struct_arr_t TEMPORARY_STRUCTURE_ARRAY_TYPE = NULL;

#define cu unsigned char;

struct_arr_t construct_temp_struct_arr_t(unsigned char *arr_of_sizes, unsigned char num_of_sizes)
{
    TEMPORARY_STRUCTURE_ARRAY_TYPE = (struct_arr_t)malloc(num_of_sizes);
    memcpy(TEMPORARY_STRUCTURE_ARRAY_TYPE, arr_of_sizes, num_of_sizes);

    return TEMPORARY_STRUCTURE_ARRAY_TYPE;
}

#define construct_struct_arr_t(str, arr_of_sizes, num_of_sizes)                            \
    memcpy(str[1], construct_temp_struct_arr_t(arr_of_sizes, num_of_sizes), num_of_sizes); \
    free(TEMPORARY_STRUCTURE_ARRAY_TYPE);

struct_arr_t get_struct_arr_t_ptr(struct_arr_t *str, unsigned char index)
{
    unsigned short int skip_size = 0;
    while (index-- > 0)
        skip_size += str[1][index];

    return (str[0] + skip_size);
}

#define get_struct_arr_t(str, index, type) \
    *(type *)get_struct_arr_t_ptr(str, index)

#undef cu
// #undef temp_str