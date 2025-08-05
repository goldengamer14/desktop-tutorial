#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int file_size(FILE *file)
{
    if (!file)
    {
        perror("Failed to open file");
        return 0;
    }

    fseek(file, 0, SEEK_END);
    unsigned int size = ftell(file);

    if (size < 0)
    {
        perror("Failed to determine file size");
        return 0;
    }
    fseek(file, 0, SEEK_SET);
    return size;
}

int main()
{
    printf("\nHello, World!\n\n");

    FILE *file = fopen("Prototypes\\Prototype.html", "r+");
    unsigned short int size = file_size(file);
    printf("File size: %u bytes\n\n", size);
    if (!file)
    {
        perror("Failed to open file for writing");
        return 1;
    }

    char content_fread[size + 1];

    fread(content_fread, 1, size, file);
    content_fread[size] = '\0';
    fseek(file, 0, SEEK_SET);

    printf("content_fread:\n%s\n\n", content_fread);

    memset(content_fread, 0, size);
    fwrite(content_fread, 1, size, file);

    fclose(file);
    remove("Prototypes\\Prototype.html");
    printf("File cleared and removed successfully\n\n");

    return 0;
}