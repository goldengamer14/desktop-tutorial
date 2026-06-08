#include <unistd.h>
#include <sys/wait.h>
#include <stdint.h>
#include <string.h>
#include "utils.h"

int main()
{
    write(1, "\nHello, World!\n\n", 16);

    write(1, "Hello\n", 6);
    pid_t pid = fork();

    if (pid == 0)
    {
        pid = getpid();
        char buffer[50] = "Child Process, pid: ";
        int8_t buffer_length = strlen(buffer);
        pid_to_ascii(pid, buffer + buffer_length, 10);
        strcat(buffer, "\n");
        buffer_length = strlen(buffer);
        write(1, buffer, buffer_length);

        // char *args[] = {"/bin/echo", "Written by echo", NULL};
        // execv("/bin/echo", args);
        char *args[] = {"/bin/sh", NULL};
        execv("/bin/sh", args);
    }

    wait(NULL);

    pid = getpid();
    char buffer[50] = "Parent Process, pid: ";
    int8_t buffer_length = strlen(buffer);
    pid_to_ascii(pid, buffer + buffer_length, 10);
    strcat(buffer, "\n");
    buffer_length = strlen(buffer);
    write(1, buffer, buffer_length);

    return 0;
}
