#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "menu.h"
#include "tools.h"
#include "caction.h"

#ifdef _WIN32 // for Windows users
#include <windows.h>
#else // for Unix users (macOS, Linux, etc.)
#include <dirent.h>
#endif

// Title screen art
void title_screen(void)
{
    printf(" \n");
    printf("\x1b[44m                                              \x1b[0m\n");
    printf("\x1b[44m         \x1b[0m       MicahSoft Word       \x1b[44m         \x1b[0m\n");
    printf("\x1b[44m         \x1b[0m         C Edition          \x1b[44m         \x1b[0m\n");
    printf("\x1b[44m                                              \x1b[0m\n");
    printf("\x1b[44m  \x1b[0m                                          \x1b[44m  \x1b[0m\n");
    printf("\x1b[44m  \x1b[0m       \x1b[47mn\x1b[0mew file        \x1b[47mf\x1b[0mind file          \x1b[44m  \x1b[0m\n");
    printf("\x1b[44m  \x1b[0m       \x1b[47mc\x1b[0mopy file       \x1b[47md\x1b[0melete file        \x1b[44m  \x1b[0m\n");
    printf("\x1b[44m  \x1b[0m       \x1b[47mi\x1b[0mnformation     \x1b[47mq\x1b[0muit               \x1b[44m  \x1b[0m\n");
    printf("\x1b[44m  \x1b[0m                                          \x1b[44m  \x1b[0m\n");
    printf("\x1b[44m                                              \x1b[0m\n");
}

// Controller for title input
int title_selection(void)
{
    char response;
    do
    {
        clear_screen();
        title_screen();
        printf("->");
        response = getchar();

    } while (response != 'n' && response != 'f' && response != 'c' && response != 'd' && response != 'i' && response != 'q');

    switch (tolower(response))
    {
    case 'n':
        return 0;
    case 'f':
        return 1;
    case 'c':
        return 2;
    case 'd':
        return 3;
    case 'i':
        return 4;
    case 'q':
        return 5;
    default:
        return 9;
    }
}

// Screen for creating a new file
void new_file_screen(void)
{
    char file_name[100];
    char folder_path[100];
    clear_screen();
    printf("\x1b[44m              New File              \x1b[0m\n");
    clear_input_buffer();
    printf("New file name: ");
    fgets(file_name, sizeof(file_name), stdin);
    remove_newline(file_name);
    strcpy(folder_path, "mscache");

#ifdef _WIN32
    strcat(folder_path, "\\*");
#else
    strcat(folder_path, "/");
#endif

// For Windows
#ifdef _WIN32
    WIN32_FIND_DATA find_file_data;
    HANDLE hFind = FindFirstFile(folder_path, &find_file_data);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf("Error opening directory: %s\n", folder_path);
        return;
    }
    int file_found = 0;
    do
    {
        if (strcmp(find_file_data.cFileName, file_name) == 0)
        {
            file_found = 1;
            break;
        }
    } while (FindNextFile(hFind, &find_file_data) != 0);
    FindClose(hFind);

    if (file_found)
    {
        printf("File '%s' already exists in the directory. Rename? Y/N ", file_name);
        if (yes_no_response())
        {
            new_file_screen();
            // recurse
        }
        else
        {
            char new_file_name[] = "CopyOf";
            strcat(new_file_name, file_name);
            file_constructor(folder_path, new_file_name);
        }
    }
    else
    {
        // create the file
        file_constructor(folder_path, file_name);
    }
// For Unix
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

        if (file_found)
        {
            printf("File '%s' already exists in the directory. Rename? Y/N ", file_name);
            if (yes_no_response())
            {
                new_file_screen();
                // recurse
            }
            else
            {
                char new_file_name[120];
                strcpy(new_file_name, "CopyOf");
                strcat(new_file_name, file_name);
                file_constructor(folder_path, new_file_name);
            }
        }
        else
        {
            // create the file
            file_constructor(folder_path, file_name);
            return;
        }
    }
    else
    {
        // if the folder could not be found
        printf("Error opening directory: %s\n", folder_path);
        return;
    }
#endif
}

// Screen for locating existing files. Returns string of file path.
char *file_find_screen(void)
{
    char file_name[100];
    char folder_path[100];
    clear_screen();
    clear_input_buffer();
    printf("\x1b[42m             Locate File              \x1b[0m\n");
    strcpy(folder_path, "mscache");

#ifdef _WIN32
// windows code goes here
#else
    strcat(folder_path, "/");
    DIR *dir;
    struct dirent *ent;
    // show folder contents
    if ((dir = opendir(folder_path)) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            printf("%s\n", ent->d_name);
        }
        closedir(dir);
    }

    printf("File name: ");
    fgets(file_name, sizeof(file_name), stdin);
    remove_newline(file_name);

    if ((dir = opendir(folder_path)) != NULL)
    {
        int file_found = 0;
        while ((ent = readdir(dir)) != NULL)
        {
            if (strcmp(ent->d_name, file_name) == 0)
            {
                file_found = 1;
                break;
            }
        }
        closedir(dir);
        if (file_found)
        {
            strcat(folder_path, file_name);
            // preserves the value of the file name after function end
            char *found_file = (char *)malloc(strlen(folder_path) + 1);
            if (found_file == NULL)
            {
                perror("Memory Allocation Failed.");
                exit(EXIT_FAILURE);
            }
            strcat(found_file, folder_path);
            return found_file;
        }
        else
        {
            // couldnt locate the file
            printf("File not found. Try again? Y/N ");
            if (yes_no_response())
            {
                file_find_screen();
                // recurse
            }
        }
    }
    perror("EOF");
    return NULL;

#endif
}

void copy_file_screen(void)
{
    char file_name[100];
    char folder_path[100];
    clear_screen();
    clear_input_buffer();
    printf("\x1b[44m             Copy File              \x1b[0m\n");
    strcpy(folder_path, "mscache");

#ifdef _WIN32
    strcat(folder_path, "\\*"); // Append * to the folder path to list all files
    WIN32_FIND_DATA find_file_data;
    HANDLE hFind = FindFirstFile(folder_path, &find_file_data);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf("Error opening directory: %s\n", folder_path);
        return;
    }
    int file_found = 0;
    do
    {
        printf("%s\n", find_file_data.cFileName); // Print each file name

    } while (FindNextFile(hFind, &find_file_data) != 0);
    FindClose(hFind);

#else
    // For Unix
    strcat(folder_path, "/");
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(folder_path)) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            printf("%s\n", ent->d_name); // show files for reference
        }
        closedir(dir);

        printf("File name: ");
        fgets(file_name, sizeof(file_name), stdin);
        remove_newline(file_name);

        if ((dir = opendir(folder_path)) != NULL)
        {
            int file_found = 0;
            while ((ent = readdir(dir)) != NULL)
            {
                if (strcmp(ent->d_name, file_name) == 0)
                {
                    file_found = 1;
                    break;
                }
            }
            closedir(dir);
            if (file_found)
            {
                // copy the file
                char dest_file[] = "CopyOf";
                strcat(dest_file, file_name);
                file_constructor(folder_path, dest_file);
                // convert file name to path
                char dest_full[120];
                strcpy(dest_full, folder_path);
                strcat(dest_full, dest_file);

                char src_full[120];
                strcpy(src_full, folder_path);
                strcat(src_full, file_name);

                printf("Copying into '%s'.\n", dest_file);
                copy_file(src_full, dest_full);
            }
            else
            {
                // couldnt copy the file
                printf("File not found. Try again? Y/N ");
                if (yes_no_response())
                {
                    copy_file_screen();
                    // recurse
                }
                else
                {
                    return;
                }
            }
        }
    }

#endif
}
