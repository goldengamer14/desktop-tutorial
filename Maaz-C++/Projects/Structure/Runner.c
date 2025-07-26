#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Structure.h"

#define cu unsigned char
#define size sizeof(int) + sizeof(char) + sizeof(float)

int main()
{
    printf("\nHello, World!\n\n");

    struct_arr_t str[2] = {(char[size]){}, (char[3]){}};
    struct_arr_t another_str[2] = {(char[size + sizeof(double) + sizeof(long double)]){}, (char[5]){}};

    construct_struct_arr_t(str, ((char[3]){sizeof(int), sizeof(float), sizeof(char)}), 3);
    construct_struct_arr_t(another_str, ((char[5]){sizeof(int), sizeof(float), sizeof(double), sizeof(long double), sizeof(char)}), 5);

    printf("str[1]: %u, %u, %u\n", str[1][0], str[1][1], str[1][2]);
    printf("another_str[1]: %u, %u, %u, %u, %u\n", another_str[1][0], another_str[1][1], another_str[1][2], another_str[1][3], another_str[1][4]);

    get_struct_arr_t(str, 0, int) = 541232;
    get_struct_arr_t(str, 1, float) = 3.14159;
    get_struct_arr_t(str, 2, char) = 'V';

    get_struct_arr_t(another_str, 0, int) = 84579666;
    get_struct_arr_t(another_str, 1, float) = 0.434294;
    get_struct_arr_t(another_str, 2, double) = 0.49714987269;
    get_struct_arr_t(another_str, 3, long double) = 2.71828182846;
    get_struct_arr_t(another_str, 4, char) = '+';

    printf("str[0]: {int = %i}, {float = %f}, {char = %c}\n", get_struct_arr_t(str, 0, int),
           get_struct_arr_t(str, 1, float), get_struct_arr_t(str, 2, char));

    printf("another_str[0]: {int = %i}, {float = %f}, {double = %lf}, {long double = %llf}, {char = %c}\n\n",
           get_struct_arr_t(another_str, 0, int),
           get_struct_arr_t(another_str, 1, float),
           get_struct_arr_t(another_str, 2, double),
           (double)get_struct_arr_t(another_str, 3, long double),
           get_struct_arr_t(another_str, 4, char));

    return 0;
}