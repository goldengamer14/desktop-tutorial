#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

int main()
{
    char buffer[64] = "\nHello, World!\n\n";
    write(1, buffer, strlen(buffer));

    int p[2];

    pipe(p);

    if (fork() == 0)
    {
        dup2(p[1], 1);
        close(p[0]);

        // write(1, "ls", 2);

        execl("/bin/ls", "ls", "--all", NULL);
    }

    if (fork() == 0)
    {
        dup2(p[0], 0);
        close(p[1]);

        // write(1, "wc", 2);

        execl("/bin/wc", "wc", NULL);
    }

    close(p[0]);
    close(p[1]);

    wait(NULL);
    wait(NULL);

    return 0;
}