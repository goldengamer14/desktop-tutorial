#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

int main()
{
    char buffer[64] = "\nHello, World!\n\n";
    write(1, buffer, strlen(buffer));

    int fd[2];

    pipe(fd);

    // WRITING TO fd[1]
    strcpy(buffer, "Writing to fd: ");
    pid_to_ascii(fd[1], buffer + strlen(buffer), 10);
    strcat(buffer, "\n");

    write(1, buffer, strlen(buffer));

    // READING FROM fd[0]
    strcpy(buffer, "Reading from fd: ");
    pid_to_ascii(fd[0], buffer + strlen(buffer), 10);
    strcat(buffer, "\n");

    write(1, buffer, strlen(buffer));

    int n = read(fd[0], buffer, 10);

    write(1, buffer, n);
    write(1, "\n", 1);

    close(fd[0]);
    close(fd[1]);

    return 0;
}