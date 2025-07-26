#define DECLARE_DE_Queue(type)                                                                                                     \
    struct DE_Queue_##type                                                                                                         \
    {                                                                                                                              \
        unsigned int max_size;                                                                                                     \
        type *arr;                                                                                                                 \
        unsigned int front;                                                                                                        \
        unsigned int back;                                                                                                         \
    };                                                                                                                             \
                                                                                                                                   \
    unsigned int DE_Queue_##type##_size(struct DE_Queue_##type *self)                                                              \
    {                                                                                                                              \
        return (self->back >= self->front ? self->back - self->front - 1                                                           \
                                          : self->max_size - (self->front - self->back) - 1);                                      \
    }                                                                                                                              \
                                                                                                                                   \
    unsigned int DE_Queue_##type##_max_size(struct DE_Queue_##type *self) { return self->max_size; }                               \
                                                                                                                                   \
    struct DE_Queue_##type *push_DE_Queue_##type(struct DE_Queue_##type *self, type data)                                          \
    {                                                                                                                              \
        if ((self->back + 1) % self->max_size == self->front)                                                                      \
            perror("Queue Overflow!!!");                                                                                           \
        else                                                                                                                       \
        {                                                                                                                          \
            self->arr[self->back] = data;                                                                                          \
            self->back = (self->back + 1) % self->max_size;                                                                        \
        }                                                                                                                          \
                                                                                                                                   \
        return self;                                                                                                               \
    }                                                                                                                              \
                                                                                                                                   \
    struct DE_Queue_##type *push_DE_Queue_##type##_array(struct DE_Queue_##type *self, type *data, unsigned int arr_size)          \
    {                                                                                                                              \
        if ((self->back + 1) % self->max_size == self->front)                                                                      \
            perror("Queue Overflow!!!");                                                                                           \
        else if (DE_Queue_##type##_size(self) + arr_size > DE_Queue_##type##_max_size(self))                                       \
            perror("Queue Input Overflow!!!");                                                                                     \
        else                                                                                                                       \
            for (unsigned int i = 0; i < arr_size; i++)                                                                            \
                push_DE_Queue_##type(self, data[i]);                                                                               \
                                                                                                                                   \
        return self;                                                                                                               \
    }                                                                                                                              \
                                                                                                                                   \
    struct DE_Queue_##type *unshift_DE_Queue_##type(struct DE_Queue_##type *self, type data)                                       \
    {                                                                                                                              \
        if ((self->back + 1) % self->max_size == self->front)                                                                      \
            perror("Queue Overflow!!!");                                                                                           \
        else                                                                                                                       \
        {                                                                                                                          \
            if (self->front == 0)                                                                                                  \
                self->front = self->max_size - 1;                                                                                  \
            else                                                                                                                   \
                self->front--;                                                                                                     \
            self->arr[self->front] = data;                                                                                         \
        }                                                                                                                          \
                                                                                                                                   \
        return self;                                                                                                               \
    }                                                                                                                              \
                                                                                                                                   \
    struct DE_Queue_##type *unshift_DE_Queue_##type##_array(struct DE_Queue_##type *self, type *data, unsigned int arr_size)       \
    {                                                                                                                              \
        if ((self->back + 1) % self->max_size == self->front)                                                                      \
            perror("Queue Overflow!!!");                                                                                           \
        else if (DE_Queue_##type##_size(self) + arr_size > DE_Queue_##type##_max_size(self))                                         \
            perror("Queue Input Overflow!!!");                                                                                     \
        else                                                                                                                       \
            for (unsigned int i = 0; i < arr_size; i++)                                                                            \
                push_DE_Queue_##type(self, data[i]);                                                                               \
                                                                                                                                   \
        return self;                                                                                                               \
    }                                                                                                                              \
                                                                                                                                   \
    type pop_DE_Queue_##type(struct DE_Queue_##type *self)                                                                         \
    {                                                                                                                              \
        type return_data;                                                                                                          \
                                                                                                                                   \
        if (self->front == self->back)                                                                                             \
        {                                                                                                                          \
            perror("Queue Underflow!!!");                                                                                          \
            memset(&return_data, 0, sizeof(return_data));                                                                          \
        }                                                                                                                          \
        else                                                                                                                       \
        {                                                                                                                          \
            if (self->back == 0)                                                                                                   \
                self->max_size - 1;                                                                                                \
            else                                                                                                                   \
                self->back--;                                                                                                      \
                                                                                                                                   \
            return_data = self->arr[self->back];                                                                                   \
        }                                                                                                                          \
                                                                                                                                   \
        return return_data;                                                                                                        \
    }                                                                                                                              \
                                                                                                                                   \
    struct DE_Queue_##type *pop_DE_Queue_##type##_array(struct DE_Queue_##type *self, unsigned int pop_size, type *return_arr)     \
    {                                                                                                                              \
        if (self->front == self->back)                                                                                             \
            perror("Queue Underflow!!!");                                                                                          \
        else                                                                                                                       \
            for (unsigned char i = 0; i < pop_size; i++)                                                                           \
                if (return_arr && return_arr != NULL)                                                                              \
                    return_arr[i] = pop_DE_Queue_##type(self);                                                                     \
                else                                                                                                               \
                    pop_DE_Queue_##type(self);                                                                                     \
                                                                                                                                   \
        return self;                                                                                                               \
    }                                                                                                                              \
                                                                                                                                   \
    type shift_DE_Queue_##type(struct DE_Queue_##type *self)                                                                       \
    {                                                                                                                              \
        type return_data;                                                                                                          \
                                                                                                                                   \
        if (self->front == self->back)                                                                                             \
        {                                                                                                                          \
            perror("Queue Underflow!!!");                                                                                          \
            memset(&return_data, 0, sizeof(return_data));                                                                          \
        }                                                                                                                          \
        else                                                                                                                       \
        {                                                                                                                          \
            return_data = self->arr[self->front];                                                                                  \
            self->front = (self->front + 1) % self->max_size;                                                                      \
        }                                                                                                                          \
                                                                                                                                   \
        return return_data;                                                                                                        \
    }                                                                                                                              \
                                                                                                                                   \
    struct DE_Queue_##type *shift_DE_Queue_##type##_array(struct DE_Queue_##type *self, unsigned int shift_size, type *return_arr) \
    {                                                                                                                              \
        if (self->front == self->back)                                                                                             \
            perror("Queue Underflow!!!");                                                                                          \
        else                                                                                                                       \
            for (unsigned char i = 0; i < shift_size; i++)                                                                         \
                if (return_arr && return_arr != NULL)                                                                              \
                    return_arr[i] = shift_DE_Queue_##type(self);                                                                   \
                else                                                                                                               \
                    shift_DE_Queue_##type(self);                                                                                   \
                                                                                                                                   \
        return self;                                                                                                               \
    }                                                                                                                              \
                                                                                                                                   \
    struct DE_Queue_##type construct_DE_Queue_##type(unsigned int size)                                                            \
    {                                                                                                                              \
        struct DE_Queue_##type self;                                                                                               \
                                                                                                                                   \
        if (size < 1)                                                                                                              \
            perror("You can't create a Queue of size 0");                                                                          \
        else                                                                                                                       \
        {                                                                                                                          \
            self.max_size = size + 1;                                                                                              \
            self.front = self.back = 0;                                                                                            \
            self.arr = (type *)calloc(self.max_size, sizeof(struct DE_Queue_##type));                                              \
        }                                                                                                                          \
                                                                                                                                   \
        return self;                                                                                                               \
    }                                                                                                                              \
                                                                                                                                   \
    struct DE_Queue_##type construct_DE_Queue_##type##_initialized(type data, unsigned int size)                                          \
    {                                                                                                                              \
        struct DE_Queue_##type self = construct_DE_Queue_##type(size);                                                             \
                                                                                                                                   \
        self.arr[self.back] = data;                                                                                                \
        self.back = (self.back + 1) % self.max_size;                                                                               \
                                                                                                                                   \
        return self;                                                                                                               \
    }                                                                                                                              \
                                                                                                                                   \
    struct DE_Queue_##type construct_DE_Queue_##type##_initialized_array(type *data, unsigned int arr_size, unsigned int size)     \
    {                                                                                                                              \
        struct DE_Queue_##type self = construct_DE_Queue_##type(size);                                                             \
                                                                                                                                   \
        memcpy(self.arr, data, arr_size * sizeof(type));                                                                           \
        self.back = arr_size;                                                                                                      \
                                                                                                                                   \
        return self;                                                                                                               \
    }                                                                                                                              \
                                                                                                                                   \
    struct DE_Queue_##type *destruct_DE_Queue_##type(struct DE_Queue_##type *self)                                                 \
    {                                                                                                                              \
        memset(self->arr, 0, DE_Queue_##type##_size(self));                                                                        \
        if (self->arr && self->arr != NULL)                                                                                        \
            free(self->arr);                                                                                                       \
        self->arr = NULL;                                                                                                          \
                                                                                                                                   \
        self->back = self->front = 0;                                                                                              \
        self->max_size = 0;                                                                                                        \
                                                                                                                                   \
        return self;                                                                                                               \
    }
