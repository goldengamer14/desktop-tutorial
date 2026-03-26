#include <windows.h>
#include <stdlib.h>
#include <string.h>

char *DWORD_to_ascii(DWORD value, char *buffer, int base)
{
    if (value == 0)
    {
        buffer[0] = '0';
        buffer[1] = '\0';
        return buffer;
    }

    DWORD i = 0;

    switch (base)
    {
    case 2:
    {
        while (value > 0)
        {
            buffer[i++] = (value & 1) ? '1' : '0';
            value >>= 1;
        }
        // buffer[i] = '\0';
        strcpy(buffer + i, "b0");
        strrev(buffer);
        break;
    }

    case 8:
    {
        while (value > 0)
        {
            buffer[i++] = '0' + (value & 7);
            value >>= 3;
        }
        // buffer[i] = '\0';
        strcpy(buffer + i, "o0");
        strrev(buffer);
        break;
    }

    case 16:
    {
        while (value > 0)
        {
            signed char digit = value & 15; // 1111
            buffer[i++] = (digit < 10) ? ('0' + digit) : ('a' + (digit - 10));
            value >>= 4;
        }
        // buffer[i] = '\0';
        strcpy(buffer + i, "x0");
        strrev(buffer);
        break;
    }

    default:
    {
        while (value > 0)
        {
            DWORD digit = value % base;
            value /= base;
            buffer[i++] = (digit < 10) ? ('0' + digit) : ('a' + (digit - 10));
        }
        buffer[i] = '\0';
        strrev(buffer);
        break;
    }
    }

    return buffer;
}

int main()
{
    const HANDLE h_in = GetStdHandle(STD_INPUT_HANDLE), h_out = GetStdHandle(STD_OUTPUT_HANDLE), h_err = GetStdHandle(STD_ERROR_HANDLE);
    char prompt[256];
    DWORD bytes_read, bytes_written;

    // Display a greeting message to the user
    WriteConsoleA(h_out, "\nHello, World!\n\n", 15, NULL, NULL);

    WriteConsoleA(h_out, "Which file would you like to open? (Press Enter for 'setup.bat'): ", 66, NULL, NULL);
    ReadConsoleA(h_in, prompt, sizeof(prompt), &bytes_read, NULL);
    prompt[bytes_read] = '\0';

    WriteConsoleA(h_out, "\nOpening file... ", 20, NULL, NULL);
    WriteConsoleA(h_out, prompt, strlen(prompt), NULL, NULL);

    if (prompt[bytes_read - 1] == '\n' || prompt[bytes_read - 1] == '\r')
        prompt[--bytes_read] = '\0';
    if (prompt[bytes_read - 1] == '\n' || prompt[bytes_read - 1] == '\r')
        prompt[--bytes_read] = '\0';

    if (!bytes_read)
        strcpy(prompt, "setup.bat");

    HANDLE hfile = CreateFile(prompt,                             // File name
                              GENERIC_READ,                       // Desired Access  {GENERIC_READ, GENERIC_WRITE, GENERIC_EXECUTE, GENERIC_ALL}
                              FILE_SHARE_READ | FILE_SHARE_WRITE, // Share Mode(For other processes while we hold it) {FILE_SHARE_READ, FILE_SHARE_WRITE, FILE_SHARE_DELETE, 0}
                              NULL,                               // Security Attributes(Whether Child Processes can inherit the Attributes) {NULL, default security}
                              OPEN_EXISTING,                      // Creation Disposition {CREATE_NEW, CREATE_ALWAYS, OPEN_EXISTING, OPEN_ALWAYS, TRUNCATE_EXISTING}
                              FILE_ATTRIBUTE_NORMAL,              // Flags and Attributes {FILE_ATTRIBUTE_NORMAL, FILE_ATTRIBUTE_READONLY, FILE_FLAG_APPEND, FILE_FLAG_OVERLAPPED, FILE_FLAG_DELETE_ON_CLOSE}
                              NULL);                              // Template File Handle {NULL, handle to template file}

    if (hfile == INVALID_HANDLE_VALUE)
    {
        // If the file could not be opened, display an error message and exit
        strcpy(prompt, "\nError opening file: ");
        DWORD_to_ascii((DWORD)GetLastError(), prompt + strlen(prompt), 10);
        strcat(prompt, "\n");

        WriteConsoleA(h_err, prompt, strlen(prompt), NULL, NULL);
        return 1;
    }

    char buffer[1024];
    DWORD total_file_size = 0;

    do
    {
        ReadFile(hfile, buffer, sizeof(buffer - 1), &bytes_read, NULL);
        buffer[bytes_read] = '\0'; // Null-terminate the string
        total_file_size += bytes_read;

        WriteConsoleA(h_out, buffer, bytes_read, NULL, NULL);
        WriteConsoleA(h_out, "\n", 1, NULL, NULL);

        WriteConsoleA(h_out, "Read Bytes: ", 12, NULL, NULL);
        DWORD_to_ascii(bytes_read, prompt, 10);
        WriteConsoleA(h_out, prompt, strlen(prompt), NULL, NULL);
        WriteConsoleA(h_out, "\nTotal File Size So Far: ", 25, NULL, NULL);
        DWORD_to_ascii(total_file_size, prompt, 10);
        WriteConsoleA(h_out, prompt, strlen(prompt), NULL, NULL);
        WriteConsoleA(h_out, "\n", 1, NULL, NULL);
    } while (bytes_read >= sizeof(buffer - 1)); // Continue reading until the end of the file

    strcpy(prompt, "\nContents of length ");
    DWORD_to_ascii(total_file_size, prompt + strlen(prompt), 2);
    strcat(prompt, " ");
    DWORD_to_ascii(total_file_size, prompt + strlen(prompt), 10);
    strcat(prompt, " ");
    DWORD_to_ascii(total_file_size, prompt + strlen(prompt), 8);
    strcat(prompt, " ");
    DWORD_to_ascii(total_file_size, prompt + strlen(prompt), 16);
    strcat(prompt, " characters\n");
    WriteConsoleA(h_out, prompt, strlen(prompt), NULL, NULL);

    // Close the file handle
    CloseHandle(hfile);

    // {{{{{{{{FILE POINTER MECHANICS}}}}}}}}
    HANDLE h_temp = CreateFile(
        "temp.txt",
        GENERIC_WRITE | GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        CREATE_ALWAYS,
        FILE_FLAG_DELETE_ON_CLOSE,
        NULL);

    WriteConsoleA(h_out, "\nWrite something to temp.txt:\n ", 32, NULL, NULL);
    ReadConsoleA(h_in, buffer, sizeof(buffer), &bytes_read, NULL);
    buffer[bytes_read] = '\0';

    WriteFile(h_temp, buffer, bytes_read, &bytes_written, NULL);

    WriteConsoleA(h_out, "\nWhat should be the offset to move the file pointer to? (Press Enter for 0): ", 78, NULL, NULL);
    ReadConsoleA(h_in, prompt, sizeof(prompt), &bytes_read, NULL);
    prompt[bytes_read] = '\0';

    LARGE_INTEGER temp_offset = {.QuadPart = atoi(prompt)}; // Convert the input string to an integer (offset)
    SetFilePointerEx(h_temp, temp_offset, NULL, FILE_BEGIN);

    // Read the content from the new file pointer position
    ReadFile(h_temp, buffer, sizeof(buffer) - 1, &bytes_read, NULL);
    buffer[bytes_read] = '\0'; // Null-terminate the string

    WriteConsoleA(h_out, "\nContent from the new file pointer position:\n ", 45, NULL, NULL);
    WriteConsoleA(h_out, buffer, bytes_read, &bytes_written, NULL);

    // SEEK_SET: FILE_BEGIN
    temp_offset.QuadPart = 0; // Move the file pointer to the beginning of the file
    SetFilePointerEx(h_temp, temp_offset, NULL, FILE_BEGIN);

    // SEEK_END: FILE_END
    temp_offset.QuadPart = 0; // Move the file pointer to the end of the file
    SetFilePointerEx(h_temp, temp_offset, NULL, FILE_END);

    CloseHandle(h_temp);

    // Return 0 to indicate successful program execution
    return 0;
}