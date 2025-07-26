#define set_last_bytes(bytes, type, first_1) \
    (type)(~(type)0 - (~(type)bytes << (8 * sizeof(type) - first_1)))
    /* (type)((~(type)0 >> 2) + (type)(~(unsigned char)bytes << (8 * sizeof(type) - first_1))) */
