#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tools.h"

#ifdef _WIN32 // for Windows users
#include <windows.h>
#else // for Unix users (macOS, Linux, etc.)
#include <dirent.h>
#endif

// Copies the first argument to the address of the second argument
void copy_file(const char *src_filename, const char *dest_filename)
{
    FILE *src_file = fopen(src_filename, "rb"); // read from
    if (src_file == NULL)
    {
        perror("Could not open file");
        return;
    }
    FILE *dest_file = fopen(dest_filename, "wb"); // write to
    if (dest_file == NULL)
    {
        perror("Could not open file");
        fclose(src_file);
        return;
    }
    char buffer[1024];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), src_file)) > 0)
    {
        fwrite(buffer, 1, bytes_read, dest_file);
    }

    fclose(src_file);
    fclose(dest_file);
}

// Creates files by given path arguments
void file_constructor(char *folder_name, char *file_name)
{
    remove_newline(file_name);
#ifdef _WIN32
    if (folder_name[strlen(folder_name) - 1] == '*')
    {
        folder_name[strlen(folder_name) - 1] = '\0';
    }
#endif
    char full_path_name[200];
    sprintf(full_path_name, "%s%s", folder_name, file_name); // construct path as "folder/filename"
    FILE *file = fopen(full_path_name, "w");
    if (file == NULL)
    {
        perror("file_constructor");
        fclose(file);
        return;
    }
}