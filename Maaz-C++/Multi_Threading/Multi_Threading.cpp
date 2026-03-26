#include <iostream>
#include <thread>
#include <pthread.h>
#include <cstring>
#include <conio.h>
using namespace std;

void func(void *start_and_end)
{
    static int func_num = 0;
    func_num++;
    const int func_name = func_num;

    const int start = *(int *)start_and_end, end = (int *)start_and_end[1];

    for (int i = start; i < end; i++)
        printf("%i from func%i\n", i, func_name);
}

int main()
{
    cout << "\nHello, World!\n"
         << endl;

    pthread_t thread_1, thread_2;
    const int *args = {0, 200, 200, 400};

    pthread_create(&thread_1, NULL, func, args);
    pthread_create(&thread_2, NULL, func, (void *)((int *)args + 2));

    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);

    return 0;
}