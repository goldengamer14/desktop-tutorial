#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    // Display a greeting message to the user
    printf("\nHello, World!\n\n");

    // Declare a buffer to store file contents (99 chars + 1 null terminator)
    char buffer[100] = "\0";
    // Declare a character variable to capture user input
    char ch;
    // Variables to store the number of bytes read and written
    DWORD bytesRead = 0, bytesWritten = 0;
    // Variable to store the file size
    DWORD old_file_size, file_size;

    // Attempt to open an existing file named "file.txt"
    // Parameters: filename, access mode (read+write), sharing mode, security attributes,
    // creation disposition (open only if exists), file attributes, template file handle
    HANDLE hfile = CreateFile(
        "file.txt",
        GENERIC_READ | GENERIC_WRITE, // Request both read and write permissions
        FILE_SHARE_READ,              // Allow other processes to read the file
        NULL,                         // Use default security settings
        OPEN_EXISTING,                // Only open if file already exists
        FILE_ATTRIBUTE_NORMAL,        // Standard file attributes
        NULL                          // No template file to copy attributes from
    );
    LARGE_INTEGER hbuffer;

    // Check if file opening failed
    if (hfile == INVALID_HANDLE_VALUE)
    {
        // Print error message (Note: hfile is invalid here, so this won't show meaningful info)
        printf("Failed to open file. Error: %lu\n", GetLastError());

        // If file doesn't exist, create a new one with the same parameters
        hfile = CreateFile(
            "file.txt",
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ,
            NULL,
            CREATE_NEW, // Create file only if it doesn't exist
            FILE_ATTRIBUTE_NORMAL,
            NULL);
    }
    else
    {
        // Read up to 99 bytes from the file into the buffer
        // Note: 4th parameter (bytes read) returns the actual number of bytes read
        ReadFile(hfile, buffer, sizeof(buffer) - 1, &bytesRead, NULL);
        buffer[bytesRead] = '\0'; // Null-terminate the string

        // Display the original file content
        printf("Read %lu bytes of the content of the file:\n%s\n", bytesRead, buffer);
    }

    // Get the current file size
    GetFileSizeEx(hfile, &hbuffer);
    old_file_size = hbuffer.QuadPart;
    printf("\n\tCurrent file size: %zu bytes.\n", old_file_size);

    puts("\nWould you like to write new content to the file yourself? (y/n): ");
    ch = getchar();

    // Move file pointer back to the beginning before writing
    SetFilePointer(hfile, 0, NULL, FILE_BEGIN);

    if (memicmp(&ch, "y", 1) == 0)
    {
        // Get new content from the user
        printf("Enter the new content to write to the file (max 99 characters): ");
        scanf(" %99[^\n]", buffer);
        getchar(); // Clear the newline character from the input buffer
        printf("\nYou entered: %s\n", buffer);
    }
    else
        strcpy(buffer, "This text was written using WriteFile syscall.");

    // Write the buffer content to the file
    WriteFile(hfile, buffer, (DWORD)strlen(buffer), &bytesWritten, NULL);
    FlushFileBuffers(hfile);
    printf("\n\tWrote %lu bytes to the file.\n", bytesWritten);

    file_size = bytesWritten;
    // if (old_file_size > file_size)
    // {
    //     LARGE_INTEGER current_pos = FILE_CURRENT;
    //     memset(buffer, 0, old_file_size - file_size);
    //     // Write null byte to erase the extra file content if new content is smaller
    //     WriteFile(hfile, buffer, old_file_size - file_size, &bytesWritten, NULL);
    //     printf("Erased %lu extra bytes from the file.\n", old_file_size - file_size);
    //     SetFilePointer(hfile, 0, NULL, current_pos);
    // }

    // Truncate file to current size after writing
    SetEndOfFile(hfile);

    // Move file pointer back to the beginning to re-read the file
    SetFilePointer(hfile, 0, NULL, FILE_BEGIN);
    // Reset bytesRead for the new read operation
    bytesRead = 0;

    // Read the updated file content into the buffer
    ReadFile(hfile, buffer, sizeof(buffer) - 1, &bytesRead, NULL);
    buffer[bytesRead] = '\0'; // Null-terminate the string

    // Display the new file content after the write operation
    printf("\tThis is the new content of the file:\n%s\n\n", buffer);

    // Close the file handle and release system resources
    CloseHandle(hfile);

    puts("Should I delete the file? (y/n): ");
    scanf(" %c", &ch);
    if (memicmp(&ch, "y", 1) == 0)
    {
        DeleteFile("file.txt"); // Delete the file after operations
        puts("Deleted File: file.txt");
    }
    else
        puts("\nExiting without deleting the file.");

    // Return 0 to indicate successful program execution
    return 0;
}