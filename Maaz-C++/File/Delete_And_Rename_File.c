#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    printf("\nHello, World!\n\n");

    FILE *file = fopen("Prototypes\\Prototype.txt", "r");
    char str[512];
    memset(str, 0, sizeof(str));

    if (file)
    {
        char *str_ptr = str;
        while (fgets(str_ptr, 200, file))
            str_ptr += strlen(str_ptr);
        fclose(file);
        // remove("Prototype.txt");
        signed char status = rename("Prototypes\\Prototype.txt", "Prototypes\\Prototype.html");
        if (status >= 0)
            printf("%i Prototype.txt renamed to Prototype.html with content:\n%s\n\n", status, str);
        else
        {
            perror("Prototype.html couldn't be renamed!!!");
            printf("Status: %i\n\n", status);
        }
    }
    else
    {
        perror("Failed to open the file Prototype.txt!!!");
        printf("\n");

        file = fopen("Prototypes\\Prototype.html", "w");
        if (!file || ferror(file))
        {
            perror("Failed to open file");
            return 1;
        }

        strcpy(str, "<!DOCTYPE html>\n\
<html lang =\"en\">\n\
<head>\n\
\t<meta charset=\"UTF-8\">\n\
\t<meta name=\"viewport\" content=\"width=device-width,initial-scale=1.0\">\n\
\t<title>Document</title>\n\
</head>\n\
<body>\n\
\n\
</body>\n\
</html>");

        fputs(str, file);
        fflush(file); // Forces the buffer in which to-write content is stored to flush it's content into the file
        printf("%s\n\nWritten to Prototype.html\n", str);

        fclose(file); // Or it'll automatically be written on closing the file

        file = fopen("Prototypes\\times.dat", "w");
        if (file)
        {
            fclose(file);
            remove("Prototypes\\times.dat");
        }
    }

    return 0;
}