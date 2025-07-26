#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    printf("\n\nHello, World!\n\n");
    // char filename[] = "/tmp/mytempfile_XXXXXX";
    char filename[] = "C:\\Users\\Admin\\AppData\\Local\\Temp\\temp_XXXXXX";

    int fd = mkstemp(filename);
    if (fd == -1)
    {
        perror("mkstemp");
        return 1;
    }

    printf("Temp file created: %s\n", filename);
    write(fd, "hello", 5);
    close(fd);

    return 0;
}