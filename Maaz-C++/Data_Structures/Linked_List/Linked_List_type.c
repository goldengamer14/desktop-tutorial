#define DECLARE_Linked_List(type)                                                                                                           \
    struct Node_##type                                                                                                                      \
    {                                                                                                                                       \
        type data;                                                                                                                          \
        struct Node_##type *next;                                                                                                           \
    };                                                                                                                                      \
                                                                                                                                            \
    struct Linked_List_##type                                                                                                               \
    {                                                                                                                                       \
        struct Node_##type *head;                                                                                                           \
        size_t size;                                                                                                                        \
    };                                                                                                                                      \
                                                                                                                                            \
    struct Linked_List_##type construct_Linked_List_##type()                                                                                \
    {                                                                                                                                       \
        struct Linked_List_##type self;                                                                                                     \
        self.head = NULL;                                                                                                                   \
        self.size = 0;                                                                                                                      \
                                                                                                                                            \
        return self;                                                                                                                        \
    }                                                                                                                                       \
                                                                                                                                            \
    struct Linked_List_##type construct_Linked_List_##type##_initialized(type data)                                                         \
    {                                                                                                                                       \
        struct Linked_List_##type self;                                                                                                     \
        self.head = (struct Node_##type *)malloc(sizeof(struct Node_##type));                                                               \
        self.head->data = data;                                                                                                             \
        self.head->next = NULL;                                                                                                             \
        self.size = 1;                                                                                                                      \
                                                                                                                                            \
        return self;                                                                                                                        \
    }                                                                                                                                       \
                                                                                                                                            \
    struct Node_##type *peek_Linked_List_##type(struct Linked_List_##type *self, size_t index)                                              \
    {                                                                                                                                       \
        if (index < 0 || index >= self->size)                                                                                               \
        {                                                                                                                                   \
            printf("Linked List Underflow!!!\n");                                                                                           \
            return self->head;                                                                                                              \
        }                                                                                                                                   \
                                                                                                                                            \
        struct Node_##type *peek_element = self->head;                                                                                      \
        while (index-- > 0 && peek_element->next != NULL)                                                                                   \
            peek_element = peek_element->next;                                                                                              \
        return peek_element;                                                                                                                \
    }                                                                                                                                       \
                                                                                                                                            \
    struct Linked_List_##type *push_Linked_List_##type(struct Linked_List_##type *self, type data)                                          \
    {                                                                                                                                       \
        struct Node_##type *ptr = self->head;                                                                                               \
                                                                                                                                            \
        if (ptr == NULL)                                                                                                                    \
        {                                                                                                                                   \
            ptr = (struct Node_##type *)malloc(sizeof(struct Node_##type));                                                                 \
            ptr->data = data;                                                                                                               \
            ptr->next = NULL;                                                                                                               \
                                                                                                                                            \
            self->size++;                                                                                                                   \
            return self;                                                                                                                    \
        }                                                                                                                                   \
                                                                                                                                            \
        while (ptr->next != NULL)                                                                                                           \
            ptr = ptr->next;                                                                                                                \
                                                                                                                                            \
        ptr->next = (struct Node_##type *)malloc(sizeof(struct Node_##type));                                                               \
        ptr->next->data = data;                                                                                                             \
        ptr->next->next = NULL;                                                                                                             \
                                                                                                                                            \
        self->size++;                                                                                                                       \
                                                                                                                                            \
        return self;                                                                                                                        \
    }                                                                                                                                       \
                                                                                                                                            \
    struct Linked_List_##type *push_Linked_List_##type##_string(struct Linked_List_##type *self, type *data, size_t length)                 \
    {                                                                                                                                       \
        struct Node_##type *ptr = self->head;                                                                                               \
                                                                                                                                            \
        if (ptr == NULL)                                                                                                                    \
        {                                                                                                                                   \
            ptr = (struct Node_##type *)malloc(sizeof(struct Node_##type));                                                                 \
            ptr->data = (type)data[0];                                                                                                      \
            ptr->next = NULL;                                                                                                               \
                                                                                                                                            \
            for (size_t i = 1; i < length; i++)                                                                                             \
            {                                                                                                                               \
                ptr->next = (struct Node_##type *)malloc(sizeof(struct Node_##type));                                                       \
                ptr->next->data = data[i];                                                                                                  \
                ptr->next->next = NULL;                                                                                                     \
                                                                                                                                            \
                self->size++;                                                                                                               \
            }                                                                                                                               \
                                                                                                                                            \
            return self;                                                                                                                    \
        }                                                                                                                                   \
                                                                                                                                            \
        while (ptr->next != NULL)                                                                                                           \
            ptr = ptr->next;                                                                                                                \
                                                                                                                                            \
        for (size_t i = 0; i < length; i++)                                                                                                 \
        {                                                                                                                                   \
            ptr->next = (struct Node_##type *)malloc(sizeof(struct Node_##type));                                                           \
            ptr->next->data = data[i];                                                                                                      \
            ptr->next->next = NULL;                                                                                                         \
                                                                                                                                            \
            ptr = ptr->next;                                                                                                                \
            self->size++;                                                                                                                   \
        }                                                                                                                                   \
                                                                                                                                            \
        return self;                                                                                                                        \
    }                                                                                                                                       \
                                                                                                                                            \
    struct Linked_List_##type *unshift_Linked_List_##type(struct Linked_List_##type *self, type data)                                       \
    {                                                                                                                                       \
        if (self->head == NULL)                                                                                                             \
        {                                                                                                                                   \
            self->head = (struct Node_##type *)malloc(sizeof(struct Node_##type));                                                          \
            self->head->data = data;                                                                                                        \
            self->head->next = NULL;                                                                                                        \
                                                                                                                                            \
            self->size++;                                                                                                                   \
            return self;                                                                                                                    \
        }                                                                                                                                   \
                                                                                                                                            \
        struct Node_##type *new_head = (struct Node_##type *)malloc(sizeof(struct Node_##type));                                            \
        new_head->data = data;                                                                                                              \
        new_head->next = self->head;                                                                                                        \
        self->head = new_head;                                                                                                              \
                                                                                                                                            \
        self->size++;                                                                                                                       \
        return self;                                                                                                                        \
    }                                                                                                                                       \
                                                                                                                                            \
    struct Linked_List_##type *unshift_Linked_List_##type##_string(struct Linked_List_##type *self, type *data, size_t length)              \
    {                                                                                                                                       \
        if (self->head == NULL)                                                                                                             \
        {                                                                                                                                   \
            self->head = (struct Node_##type *)malloc(sizeof(struct Node_##type));                                                          \
            self->head->data = data[0];                                                                                                     \
            self->head->next = NULL;                                                                                                        \
                                                                                                                                            \
            for (size_t i = 1; i < length; i++)                                                                                             \
            {                                                                                                                               \
                struct Node_##type *new_head = (struct Node_##type *)malloc(sizeof(struct Node_##type));                                    \
                new_head->data = data[i];                                                                                                   \
                new_head->next = self->head;                                                                                                \
                self->head = new_head;                                                                                                      \
                                                                                                                                            \
                self->size++;                                                                                                               \
            }                                                                                                                               \
                                                                                                                                            \
            return self;                                                                                                                    \
        }                                                                                                                                   \
                                                                                                                                            \
        for (size_t i = 0; i < length; i++)                                                                                                 \
        {                                                                                                                                   \
            struct Node_##type *new_head = (struct Node_##type *)malloc(sizeof(struct Node_##type));                                        \
            new_head->data = data[i];                                                                                                       \
            new_head->next = self->head;                                                                                                    \
            self->head = new_head;                                                                                                          \
                                                                                                                                            \
            self->size++;                                                                                                                   \
        }                                                                                                                                   \
                                                                                                                                            \
        return self;                                                                                                                        \
    }                                                                                                                                       \
                                                                                                                                            \
    struct Linked_List_##type *insert_Linked_List_##type(struct Linked_List_##type *self, type data, size_t index)                          \
    {                                                                                                                                       \
        if (index < 0 || index >= self->size)                                                                                               \
        {                                                                                                                                   \
            printf("Linked List Underflow!!!\n");                                                                                           \
            return self;                                                                                                                    \
        }                                                                                                                                   \
        else if (index == 0)                                                                                                                \
            return unshift_Linked_List_##type(self, data);                                                                                  \
        else if (index == self->size - 1)                                                                                                   \
            return push_Linked_List_##type(self, data);                                                                                     \
                                                                                                                                            \
        struct Node_##type *ptr = self->head;                                                                                               \
        while (index-- > 0 && ptr->next != NULL)                                                                                            \
            ptr = ptr->next;                                                                                                                \
                                                                                                                                            \
        struct Node_##type *new_node = (struct Node_##type *)malloc(sizeof(struct Node_##type));                                            \
        new_node->data = data;                                                                                                              \
        new_node->next = ptr->next;                                                                                                         \
        ptr->next = new_node;                                                                                                               \
                                                                                                                                            \
        self->size++;                                                                                                                       \
        return self;                                                                                                                        \
    }                                                                                                                                       \
                                                                                                                                            \
    struct Linked_List_##type *insert_Linked_List_##type##_string(struct Linked_List_##type *self, type *data, size_t index, size_t length) \
    {                                                                                                                                       \
        if (index < 0 || index >= self->size)                                                                                               \
        {                                                                                                                                   \
            printf("Linked List Underflow!!!\n");                                                                                           \
            return self;                                                                                                                    \
        }                                                                                                                                   \
        else if (index == 0)                                                                                                                \
            return unshift_Linked_List_##type##_string(self, data, length);                                                                 \
        else if (index == self->size - 1)                                                                                                   \
            return push_Linked_List_##type##_string(self, data, length);                                                                    \
                                                                                                                                            \
        struct Node_##type *ptr = self->head;                                                                                               \
        while (index-- > 0 && ptr->next != NULL)                                                                                            \
            ptr = ptr->next;                                                                                                                \
                                                                                                                                            \
        for (size_t i = 0; i < length; i++)                                                                                                 \
        {                                                                                                                                   \
            struct Node_##type *new_node = (struct Node_##type *)malloc(sizeof(struct Node_##type));                                        \
            new_node->data = data[i];                                                                                                       \
            new_node->next = ptr->next;                                                                                                     \
            ptr->next = new_node;                                                                                                           \
                                                                                                                                            \
            self->size++;                                                                                                                   \
            ptr = ptr->next;                                                                                                                \
        }                                                                                                                                   \
                                                                                                                                            \
        return self;                                                                                                                        \
    }                                                                                                                                       \
                                                                                                                                            \
    struct Linked_List_##type *pop_Linked_List_##type(struct Linked_List_##type *self)                                                      \
    {                                                                                                                                       \
        if (self->size < 1)                                                                                                                 \
        {                                                                                                                                   \
            printf("Linked List Underflow!!!\n");                                                                                           \
            return self;                                                                                                                    \
        }                                                                                                                                   \
                                                                                                                                            \
        struct Node_##type *ptr = self->head;                                                                                               \
        while (ptr->next != NULL && ptr->next->next != NULL)                                                                                \
            ptr = ptr->next;                                                                                                                \
                                                                                                                                            \
        free(ptr->next);                                                                                                                    \
        ptr->next = NULL;                                                                                                                   \
        self->size--;                                                                                                                       \
                                                                                                                                            \
        return self;                                                                                                                        \
    }                                                                                                                                       \
                                                                                                                                            \
    struct Linked_List_##type *pop_Linked_List_##type##_string(struct Linked_List_##type *self, size_t index)                               \
    {                                                                                                                                       \
        if (self->size < 1)                                                                                                                 \
        {                                                                                                                                   \
            printf("Linked List Underflow!!!\n");                                                                                           \
            return self;                                                                                                                    \
        }                                                                                                                                   \
        else if (index < 0 || index >= self->size - 1)                                                                                      \
        {                                                                                                                                   \
            printf("Invalid Index!!!\n");                                                                                                   \
            return self;                                                                                                                    \
        }                                                                                                                                   \
                                                                                                                                            \
        struct Node_##type *ptr = self->head;                                                                                               \
        while (index-- > 0)                                                                                                                 \
            ptr = ptr->next;                                                                                                                \
                                                                                                                                            \
        while (ptr->next != NULL)                                                                                                           \
        {                                                                                                                                   \
            struct Node_##type *delete_node = ptr->next;                                                                                    \
            ptr->next = ptr->next->next;                                                                                                    \
            free(delete_node);                                                                                                              \
                                                                                                                                            \
            self->size--;                                                                                                                   \
        }                                                                                                                                   \
                                                                                                                                            \
        return self;                                                                                                                        \
    }                                                                                                                                       \
                                                                                                                                            \
    struct Linked_List_##type *shift_Linked_List_##type(struct Linked_List_##type *self)                                                    \
    {                                                                                                                                       \
        if (self->size < 1)                                                                                                                 \
        {                                                                                                                                   \
            printf("Linked List Underflow!!!\n");                                                                                           \
            return self;                                                                                                                    \
        }                                                                                                                                   \
                                                                                                                                            \
        struct Node_##type *new_head = self->head->next;                                                                                    \
        free(self->head);                                                                                                                   \
        self->head = new_head;                                                                                                              \
                                                                                                                                            \
        self->size--;                                                                                                                       \
        return self;                                                                                                                        \
    }                                                                                                                                       \
                                                                                                                                            \
    struct Linked_List_##type *shift_Linked_List_##type##_string(struct Linked_List_##type *self, size_t index)                             \
    {                                                                                                                                       \
        if (self->size < 1)                                                                                                                 \
        {                                                                                                                                   \
            printf("Linked List Underflow!!!\n");                                                                                           \
            return self;                                                                                                                    \
        }                                                                                                                                   \
        else if (index < 0 || index > self->size - 1)                                                                                       \
        {                                                                                                                                   \
            printf("Invalid Index!!!\n");                                                                                                   \
            return self;                                                                                                                    \
        }                                                                                                                                   \
                                                                                                                                            \
        while (index-- > 0)                                                                                                                 \
        {                                                                                                                                   \
            struct Node_##type *new_head = self->head->next;                                                                                \
            free(self->head);                                                                                                               \
            self->head = new_head;                                                                                                          \
                                                                                                                                            \
            self->size--;                                                                                                                   \
        }                                                                                                                                   \
                                                                                                                                            \
        return self;                                                                                                                        \
    }                                                                                                                                       \
                                                                                                                                            \
    struct Linked_List_##type *remove_Linked_List_##type(struct Linked_List_##type *self, size_t index)                                     \
    {                                                                                                                                       \
        if (index < 0 || index >= self->size)                                                                                               \
        {                                                                                                                                   \
            printf("Linked List Underflow!!!\n");                                                                                           \
            return self;                                                                                                                    \
        }                                                                                                                                   \
        else if (index == 0)                                                                                                                \
            return shift_Linked_List_##type(self);                                                                                          \
        else if (index == self->size - 1)                                                                                                   \
            return pop_Linked_List_##type(self);                                                                                            \
                                                                                                                                            \
        struct Node_##type *ptr = self->head;                                                                                               \
        while (index-- > 0 && ptr->next != NULL)                                                                                            \
            ptr = ptr->next;                                                                                                                \
                                                                                                                                            \
        struct Node_##type *delete_node = ptr->next;                                                                                        \
        ptr->next = ptr->next->next;                                                                                                        \
        free(delete_node);                                                                                                                  \
                                                                                                                                            \
        self->size--;                                                                                                                       \
        return self;                                                                                                                        \
    }                                                                                                                                       \
                                                                                                                                            \
    struct Linked_List_##type *remove_Linked_List_##type##_string(struct Linked_List_##type *self, size_t start_index, size_t end_index)    \
    {                                                                                                                                       \
        if (start_index >= end_index || start_index < 0 || start_index >= self->size || end_index < 0 || end_index >= self->size)           \
        {                                                                                                                                   \
            printf("Invalid Indices!!!\n");                                                                                                 \
            return self;                                                                                                                    \
        }                                                                                                                                   \
        else if (start_index == 0)                                                                                                          \
            return shift_Linked_List_##type##_string(self, end_index);                                                                      \
        else if (end_index == self->size - 1)                                                                                               \
            return pop_Linked_List_##type##_string(self, start_index);                                                                      \
                                                                                                                                            \
        struct Node_##type *ptr = self->head;                                                                                               \
        for (size_t i = 0; i < start_index && ptr->next != NULL; i++)                                                                       \
            ptr = ptr->next;                                                                                                                \
                                                                                                                                            \
        for (size_t i = 0; i <= end_index && ptr->next != NULL; i++)                                                                        \
        {                                                                                                                                   \
            struct Node_##type *delete_node = ptr->next;                                                                                    \
            ptr->next = ptr->next->next;                                                                                                    \
            free(delete_node);                                                                                                              \
                                                                                                                                            \
            self->size--;                                                                                                                   \
        }                                                                                                                                   \
                                                                                                                                            \
        return self;                                                                                                                        \
    }                                                                                                                                       \
                                                                                                                                            \
    void destruct_Linked_List_##type##_Node(struct Node_##type *ptr)                                                                        \
    {                                                                                                                                       \
        if (ptr->next != NULL)                                                                                                              \
            destruct_Linked_List_##type##_Node(ptr->next);                                                                                  \
        free(ptr);                                                                                                                          \
    }                                                                                                                                       \
                                                                                                                                            \
    void destruct_Linked_List_##type(struct Linked_List_##type *self)                                                                       \
    {                                                                                                                                       \
        if (self->head != NULL)                                                                                                             \
            destruct_Linked_List_##type##_Node(self->head);                                                                                 \
    }
