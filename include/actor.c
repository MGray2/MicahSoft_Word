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

// Copies the first argument to the address of the second argument. (saction.h)
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

// Creates files by given path arguments. (saction.h)
void file_constructor(char *folder_name, char *file_name)
{
    remove_newline(file_name);
#ifdef _WIN32
    remove_asterisk(folder_name);
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

/* Evaluates the folder for exact instances of file name.
Returns true if duplicate located, false otherwise. (saction.h)*/
int file_search(char *folder_path, char *file_name)
{
#ifdef _WIN32
    WIN32_FIND_DATA find_file_data;
    HANDLE hFind = FindFirstFile(folder_path, &find_file_data);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf("Error opening directory: %s\n", folder_path);
        return 0;
    }
    int file_found = 0;
    do
    {
        printf("%s\n", find_file_data.cFileName); // Print each file name
        if (strcmp(find_file_data.cFileName, file_name) == 0)
        {
            print_ylw(find_file_data.cFileName, "!", NULL);
            file_found = 1;
            break;
        }
    } while (FindNextFile(hFind, &find_file_data) != 0);
    FindClose(hFind);
    if (file_found)
    {
        return 1;
    }
    else
    {
        return 0;
    }
#else
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(folder_path)) != NULL)
    {
        int file_found = 0;
        while ((ent = readdir(dir)) != NULL)
        {
            printf("%s\n", ent->d_name); // show files
            if (strcmp(ent->d_name, file_name) == 0)
            {
                printf("\x1b[31m%s\x1b[0m\n", ent->d_name); // red highlight for duplicate
                file_found = 1;
                break;
            }
        }
        closedir(dir);
    }
    if (file_found)
    {
        return 1;
    }
    else
    {
        return 0;
    }
#endif
}

// Simply displays all file names under the folder. (saction.h)
void show_files(char *folder_path)
{
#ifdef _WIN32
    WIN32_FIND_DATA find_file_data;
    HANDLE hFind = FindFirstFile(folder_path, &find_file_data);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf("Error opening directory: %s\n", folder_path);
        return;
    }

    do
    {
        printf("%s\n", find_file_data.cFileName); // Print each file name

    } while (FindNextFile(hFind, &find_file_data) != 0);
    FindClose(hFind);
#else
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(folder_path)) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            printf("%s\n", ent->d_name); // show files
        }
        closedir(dir);
    }
#endif
}