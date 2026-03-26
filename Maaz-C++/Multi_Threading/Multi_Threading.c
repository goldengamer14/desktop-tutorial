#include <stdio.h>
#include <conio.h>
#include <pthread.h>

void *func(void *start_and_end)
{
    static int func_num = 0;
    const int func_name = func_num + 1;

    const int start = *(int *)start_and_end, end = ((int *)start_and_end)[1];

    for (int i = start; i < end; i++)
        printf("%i from func%i\n", i, func_name);

    pthread_exit(NULL);
    return NULL;
}

int main()
{
    puts("Hello, World!");

    pthread_t thread_1, thread_2;
    const int args[] = {0, 200, 200, 400};

    pthread_create(&thread_1, NULL, func, (void *)&args[0]);
    pthread_create(&thread_2, NULL, func, (void *)&args[2]);

    // pthread_join(thread_1, NULL);
    // pthread_join(thread_2, NULL);
    pthread_detach(thread_1);
    pthread_detach(thread_2);

    _getch();

    return 0;
}