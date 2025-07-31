#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int times = 0;

int main()
{
    printf("\nHello, World!\n\n");

    // Taking times from the file
    FILE *file = fopen("Prototypes\\times.dat", "r");
    if (file)
    {
        fscanf(file, "%d", &times);
        fclose(file);
    }

    file = fopen("Prototypes\\Prototype.html", "r");
    if (!file)
    {
        perror("Failed to open the file Prototype.html!!!");
        return 1;
    }

    char content[2048] = {0};
    fread(content, 1, sizeof(content) - 1, file);
    fclose(file);

    char *str_ptr = strstr(content, "<body>");
    if (!str_ptr)
    {
        fprintf(stderr, "Could not find <body> tag.\n");
        return 1;
    }

    // Split content
    unsigned int head_len = (str_ptr - content) + strlen("<body>\n");
    char pre_body[1024] = {0};
    strncpy(pre_body, content, head_len);

    char to_append[strlen("<hr><h1>This is the appended head! </h1><hr>") + 6];
    sprintf(to_append + strlen(to_append), "<hr><h1>This is the appended head! %i</h1><hr>\0", times);
    char *post_body = str_ptr + strlen("<body>\n");

    // Final content
    char final_content[2048] = {0};
    snprintf(final_content, sizeof(final_content), "%s\t%s\n%s", pre_body, to_append, post_body);

    // Now rewrite the file
    file = fopen("Prototypes\\Prototype.html", "w");
    if (!file)
    {
        perror("Failed to open file for writing");
        return 1;
    }

    fputs(final_content, file);
    fclose(file);

    printf("File Prototype.html has been updated successfully!!!\npre_body:\n%s\nto_append:\n%s\npost_body:\n%s\n\n", pre_body, to_append, post_body);

    times++;
    file = fopen("Prototypes\\times.dat", "w");
    if (file)
    {
        fprintf(file, "%d", times);
        fclose(file);
    }

    return 0;
}