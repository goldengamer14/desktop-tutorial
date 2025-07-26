#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("\nHello, World!\n\n");

    FILE *file = fopen("Prototypes\\Prototype.txt", "w+");
    if (!file)
    {
        perror("Failed to open/create file");
        return 1;
    }

    char str[] = "<!DOCTYPE html>\n\
<html lang=\"en\">\n\
<head>\n\
\t<meta charset=\"UTF-8\">\n\
\t<meta name=\"viewport\" content=\"width=device-width,initial-scale=1.0\">\n\
\t<title>Document</title>\n\
</head>\n\
<body>\n\
\n\
</body>\n\
</html>";

    fputs(str, file);
    printf("%s\n\n", str);

    fclose(file);
    return 0;
}
