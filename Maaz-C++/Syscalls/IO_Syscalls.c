#include <windows.h>
#include <string.h>

void Drain_Console_Input()
{
    const HANDLE h_in = GetStdHandle(STD_INPUT_HANDLE), h_out = GetStdHandle(STD_OUTPUT_HANDLE);
    char dump_buffer[128];
    DWORD bytes_dumped = 0, dumped_bytes, mode;

    // DISABLE LINE INPUT TO READ INPUT CHARACTER BY CHARACTER
    GetConsoleMode(h_in, &mode);
    SetConsoleMode(h_in, mode & ~ENABLE_ECHO_INPUT);

    do
    {
        WriteConsoleA(h_out, "Press Enter to continue:\t", 28, NULL, NULL);
        ReadConsoleA(h_in, dump_buffer, sizeof(dump_buffer), &dumped_bytes, NULL);

        // OVERWRITE THE LINE WITH SPACES
        strcpy(dump_buffer, "\r");
        memset(dump_buffer + 1, ' ', sizeof(dump_buffer) - 3);
        strcpy(dump_buffer + sizeof(dump_buffer) - 2, "\r");
        WriteConsoleA(h_out, dump_buffer, sizeof(dump_buffer), NULL, NULL);

        bytes_dumped += dumped_bytes;
    } while (dumped_bytes == sizeof(dump_buffer));

    // RE-ENABLE LINE INPUT
    SetConsoleMode(h_in, mode | ENABLE_LINE_INPUT);
}

int main()
{
    HANDLE h_in = GetStdHandle(STD_INPUT_HANDLE), h_out = GetStdHandle(STD_OUTPUT_HANDLE);

    char buffer[20] = {0};
    char console_prompt[64] = "C'mon, enter something:\t";
    DWORD bytes_read, bytes_written;

    /* ----------------------------------ReadFile & WriteFile--------------------------------------------------------------- */

    // WRITE console_prompt TO stdout & READ stdin TO buffer using ReadFile
    WriteFile(h_out, console_prompt, strlen(console_prompt), &bytes_written, NULL);
    ReadFile(h_in, buffer, sizeof(buffer), &bytes_read, NULL);
    buffer[bytes_read] = '\0';

    strcpy(console_prompt, "\nYou typed this:\t");

    // WRITE console_prompt and buffer TO stdout
    WriteFile(h_out, console_prompt, strlen(console_prompt), &bytes_written, NULL);
    WriteFile(h_out, buffer, strlen(buffer), &bytes_written, NULL);

    // WRITE A NEWLINE TO stdout
    strcpy(console_prompt, "\n");
    WriteFile(h_out, console_prompt, 1, &bytes_written, NULL);

    // WRITE THE VALUE OF bytes_read TO stdout
    strcpy(console_prompt, "bytes_read = ");
    itoa(bytes_read, console_prompt + strlen(console_prompt), 10);
    strcat(console_prompt, "\n\n");

    WriteFile(h_out, console_prompt, strlen(console_prompt), &bytes_written, NULL);

    // FlushConsoleInputBuffer(h_in); // doesn't work
    // FlushFileBuffers(h_in); // doesn't work
    Drain_Console_Input();

    /* ----------------------------------ReadConsoleA--------------------------------------------------------------- */

    // WRITE console_prompt TO stdout & READ stdin TO buffer USING ReadConsoleA()
    strcpy(console_prompt, "Now Write for ReadConsoleA:\t");

    WriteFile(h_out, console_prompt, strlen(console_prompt), &bytes_written, NULL);
    ReadConsoleA(h_in, buffer, sizeof(buffer), &bytes_read, NULL);
    buffer[bytes_read] = '\0';

    // WRITE A NEWLINE TO stdout
    strcpy(console_prompt, "\n");
    WriteFile(h_out, console_prompt, 1, &bytes_written, NULL);

    // WRITE THE console_prompt & buffer TO stdout
    strcpy(console_prompt, "You Wrote this:\t");

    WriteFile(h_out, console_prompt, strlen(console_prompt), &bytes_written, NULL);
    WriteFile(h_out, buffer, strlen(buffer), &bytes_written, NULL);

    // WRITE THE VALUE OF bytes_read TO stdout
    strcpy(console_prompt, "bytes_read = ");
    itoa(bytes_read, console_prompt + strlen(console_prompt), 10);
    strcat(console_prompt, "\n");

    WriteFile(h_out, console_prompt, strlen(console_prompt), &bytes_written, NULL);

    // WRITE A NEWLINE TO stdout
    strcpy(console_prompt, "\n");
    WriteFile(h_out, console_prompt, 1, &bytes_written, NULL);

    Drain_Console_Input();

    /* ----------------------------------SCANNING INTEGERS--------------------------------------------------------------- */
    int user_integer = 0;

    // WRITE console_prompt TO stdout & READ int FROM stdin TO buffer USING ReadFile
    strcpy(console_prompt, "\nNow Enter an Integer:\t");
    WriteFile(h_out, console_prompt, strlen(console_prompt), &bytes_written, NULL);
    ReadFile(h_in, buffer, sizeof(buffer), &bytes_read, NULL);
    buffer[bytes_read] = '\0';

    // WRITE A NEWLINE TO stdout
    strcpy(console_prompt, "\n");
    WriteFile(h_out, console_prompt, 1, &bytes_written, NULL);

    // CONVERT buffer TO INTEGER AND STORE IN user_integer
    user_integer = atoi(buffer);

    // WRITE THE VALUE OF user_integer TO stdout
    strcpy(console_prompt, "You Entered Integer: ");
    itoa(user_integer, console_prompt + strlen(console_prompt), 10);
    strcat(console_prompt, "\n");

    WriteFile(h_out, console_prompt, strlen(console_prompt), &bytes_written, NULL);

    // WRITE THE VALUE OF bytes_read TO stdout
    strcpy(console_prompt, "bytes_read = ");
    itoa(bytes_read, console_prompt + strlen(console_prompt), 10);
    strcat(console_prompt, "\n");

    return 0;
}