#define DECLARE_Stack(type)                                                                                      \
    struct Stack_##type                                                                                          \
    {                                                                                                            \
        type *arr;                                                                                               \
        size_t arr_size;                                                                                         \
        size_t max_size;                                                                                         \
    };                                                                                                           \
                                                                                                                 \
    struct Stack_##type construct_Stack_##type(size_t max_size)                                                  \
    {                                                                                                            \
        struct Stack_##type self;                                                                                \
        self.arr = (type *)malloc(sizeof(type) * max_size);                                                      \
        self.arr_size = 0;                                                                                       \
        self.max_size = max_size;                                                                                \
                                                                                                                 \
        return self;                                                                                             \
    }                                                                                                            \
                                                                                                                 \
    struct Stack_##type construct_Stack_##type##_initialized(type data, size_t max_size)                         \
    {                                                                                                            \
        struct Stack_##type self;                                                                                \
        self.arr = (type *)malloc(sizeof(type) * max_size);                                                      \
        self.arr[0] = data;                                                                                      \
        self.arr_size = 1;                                                                                       \
        self.max_size = max_size;                                                                                \
                                                                                                                 \
        return self;                                                                                             \
    }                                                                                                            \
                                                                                                                 \
    struct Stack_##type construct_Stack_##type##_initialized_array(type *data, size_t arr_size, size_t max_size) \
    {                                                                                                            \
        struct Stack_##type self;                                                                                \
        self.arr = (type *)malloc(sizeof(type) * max_size);                                                      \
        for (size_t i = 0; i < arr_size; i++)                                                                    \
            self.arr[i] = data[i];                                                                               \
        self.arr_size = arr_size;                                                                                \
        self.max_size = max_size;                                                                                \
                                                                                                                 \
        return self;                                                                                             \
    }                                                                                                            \
                                                                                                                 \
    type peek_Stack_##type(struct Stack_##type *self, size_t index)                                              \
    {                                                                                                            \
        if (index > self->max_size - 1)                                                                          \
        {                                                                                                        \
            printf("Stack Underflow!\n");                                                                        \
            return self->arr[index];                                                                             \
        }                                                                                                        \
        else if (index > self->arr_size - 1)                                                                     \
        {                                                                                                        \
            printf("Stack Unused!\n");                                                                           \
            return self->arr[index];                                                                             \
        }                                                                                                        \
        return self->arr[index];                                                                                 \
    }                                                                                                            \
                                                                                                                 \
    struct Stack_##type *push_Stack_##type(struct Stack_##type *self, type data)                                 \
    {                                                                                                            \
        if (self->arr_size >= self->max_size)                                                                    \
            printf("Stack Overflow!!!\n");                                                                       \
        else                                                                                                     \
            self->arr[self->arr_size++] = data;                                                                  \
                                                                                                                 \
        return self;                                                                                             \
    }                                                                                                            \
                                                                                                                 \
    struct Stack_##type *push_Stack_##type##_array(struct Stack_##type *self, type *data, size_t data_size)      \
    {                                                                                                            \
        if (self->arr_size >= self->max_size)                                                                    \
            printf("Stack Overflow!!!\n");                                                                       \
        else if (self->arr_size + data_size > self->max_size)                                                    \
            printf("Push Overflow!!!\n");                                                                        \
        else                                                                                                     \
            for (size_t i = 0; i < data_size; i++)                                                               \
                self->arr[self->arr_size++] = data[i];                                                           \
                                                                                                                 \
        return self;                                                                                             \
    }                                                                                                            \
                                                                                                                 \
    struct Stack_##type *pop_Stack_##type(struct Stack_##type *self)                                             \
    {                                                                                                            \
        if (self->arr_size < 1)                                                                                  \
            printf("Stack Underflow!!!");                                                                        \
        else                                                                                                     \
            self->arr_size--;                                                                                    \
                                                                                                                 \
        return self;                                                                                             \
    }                                                                                                            \
                                                                                                                 \
    struct Stack_##type *pop_Stack_##type##_array(struct Stack_##type *self, size_t pop_size)                    \
    {                                                                                                            \
        if (self->arr_size < 1)                                                                                  \
            printf("Stack Underflow!!!\n");                                                                      \
        else if (self->arr_size - pop_size < 1)                                                                  \
            printf("Pop Underflow!!!\n");                                                                        \
        else                                                                                                     \
            while (pop_size-- > 0)                                                                               \
                self->arr_size--;                                                                                \
                                                                                                                 \
        return self;                                                                                             \
    }                                                                                                            \
                                                                                                                 \
    void destruct_Stack_##type(struct Stack_##type *self)                                                        \
    {                                                                                                            \
        if (self->arr != NULL)                                                                                   \
        {                                                                                                        \
            free(self->arr);                                                                                     \
            self->arr = NULL;                                                                                    \
        }                                                                                                        \
        self->arr_size = self->max_size = 0;                                                                     \
    }
