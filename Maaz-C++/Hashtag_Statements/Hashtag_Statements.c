// #include used to include files
#include <stdio.h>

// #define used to define macros
#define PI 3.142 // Wherever we write PI will be replaced by 3.142
#define size(item) {sizeof(item)}

// #undef used to undefine macros to redefine them
#undef size

int main()
{
    printf("\n");
// #if, #ifdef, #ifndef, #elif, #endif are obvious
#ifdef PI
    printf("PI is already defined as %f\n", PI);
#else
    printf("PI is undefined\n");
#endif

#ifndef size
    printf("Yes, size has been undefined successfully!\n");
#else
    printf("Couldn't successfully undefine size\n");
#endif

#define undefined 102

#if undefined < 0 || undefined > 100
#undef undefined
#define undefined 100
    printf("Undefined changed to %d\n", undefined);
#else
    printf("How many times are you gonna recheck it? It's defined as %d\n", undefined);
#endif

    return 0;
}

// #error & #warning will actually trick the compiler into thinking it to be an error or warning message,
// #error will even stop the compilation of the program