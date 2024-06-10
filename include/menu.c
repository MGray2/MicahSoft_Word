#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "menu.h"
#include "tools.h"
#include "actor.h"

#ifdef _WIN32 // for Windows users
#include <windows.h>
#else // for Unix users (macOS, Linux, etc.)
#include <dirent.h>
#endif

// Title screen art. (menu.h)
void title_screen(void)
{
    printf(" \n");
    printf("\x1b[44m                                              \x1b[0m\n");
    printf("\x1b[44m         \x1b[0m       MicahSoft Word       \x1b[44m         \x1b[0m\n");
    printf("\x1b[44m         \x1b[0m         C Edition          \x1b[44m         \x1b[0m\n");
    printf("\x1b[44m                                              \x1b[0m\n");
    printf("\x1b[44m  \x1b[0m                                          \x1b[44m  \x1b[0m\n");
    printf("\x1b[44m  \x1b[0m       \x1b[47mn\x1b[0mew file        \x1b[47mf\x1b[0mind file          \x1b[44m  \x1b[0m\n");
    printf("\x1b[44m  \x1b[0m                                          \x1b[44m  \x1b[0m\n");
    printf("\x1b[44m  \x1b[0m       \x1b[47mi\x1b[0mnformation     \x1b[47mq\x1b[0muit               \x1b[44m  \x1b[0m\n");
    printf("\x1b[44m  \x1b[0m                                          \x1b[44m  \x1b[0m\n");
    printf("\x1b[44m                                              \x1b[0m\n");
}

// Controller for title input. (menu.h)
int title_selection(void)
{
    char response;
    do
    {
        clear_screen();
        title_screen();
        printf("->");
        response = getchar();

    } while (response != 'n' && response != 'f' && response != 'i' && response != 'q');

    switch (tolower(response))
    {
    case 'n':
        return 0;
    case 'f':
        return 1;
    case 'i':
        return 2;
    case 'q':
        return 3;
    default:
        return 9;
    }
}

// Menu interface for creating a new file. Returns string of file path. (menu.h)
char *new_file_screen(void)
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
    if (file_search(folder_path, file_name))
    {
        printf("File '%s' already exists in the directory. Rename? Y/N ", file_name);
        if (yes_no_response())
        {
            new_file_screen();
            // recurse
        }
        else
        {
            char new_file_name[200];
            strcpy(new_file_name, "CopyOf");
            strcat(new_file_name, file_name);
            file_constructor(folder_path, new_file_name);
            char *p_created_file = (char *)malloc(200);
            if (p_created_file == NULL)
            {
                perror("Memory Allocation Failed.");
                exit(EXIT_FAILURE);
            }
            strcpy(p_created_file, new_file_name);
            return p_created_file;
        }
    }
    else
    {
        // create the file
        file_constructor(folder_path, file_name);
        char *p_created_file = (char *)malloc(200);
        if (p_created_file == NULL)
        {
            perror("Memory Allocation Failed.");
            exit(EXIT_FAILURE);
        }
        strcat(folder_path, file_name);
        strcpy(p_created_file, folder_path);
        return p_created_file;
    }
// For Unix
#else
    if (file_search(folder_path, file_name))
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
    print_red("Error opening directory", folder_path, NULL);
    return;
#endif
}

// Screen for locating existing files. Returns string of file path. (menu.h)
char *file_find_screen(void)
{
    char file_name[100];
    char folder_path[100];
    clear_screen();
    clear_input_buffer();
    printf("\x1b[42m             Locate File              \x1b[0m\n");
    strcpy(folder_path, "mscache");

#ifdef _WIN32
    strcat(folder_path, "\\*");

#else
    strcat(folder_path, "/");

#endif
    show_files(folder_path);
    printf("File name: ");
    fgets(file_name, sizeof(file_name), stdin);
    remove_newline(file_name);

    if (file_search(folder_path, file_name))
    {
        remove_asterisk(folder_path);
        strcat(folder_path, file_name);
        // preserves the value of the file name after function end
        char *p_found_file = (char *)malloc(200);
        if (p_found_file == NULL)
        {
            perror("Memory Allocation Failed.");
            exit(EXIT_FAILURE);
        }
        strcpy(p_found_file, folder_path);
        clear_screen();
        return p_found_file;
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
    perror("EOF");
    return NULL;
}

// Menu interface for selecting file to copy. (menu.h)
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

#else
    // For Unix
    strcat(folder_path, "/");

#endif
    if (file_search(folder_path, file_name))
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

void copy_miniscreen(char *source_path, char *file_name)
{
    char folder[200];
#ifdef _WIN32
    strcpy(folder, "mscache\\*");
#else
    strcpy(folder, "mscache/");
#endif
    printf("Would you like to rename the copy? Y/N ");
    if (yes_no_response()) // If yes
    {
        char copy_name[100];
        clear_input_buffer();
        printf("File name: ");
        fgets(copy_name, sizeof(copy_name), stdin);
        remove_newline(copy_name);
        while (file_search(folder, copy_name) != 0)
        {
            print_ylw("Name is already in use. Try again? Y/N ", NULL);
            if (yes_no_response())
            {
                copy_miniscreen(source_path, file_name);
            }
            else
            {
                char dest_file[200];
#ifdef _WIN32
                remove_asterisk(folder);
#endif
                strcpy(dest_file, folder);
                strcat(dest_file, "CopyOf");
                strcat(dest_file, file_name);
                print_debug(dest_file, __FILE__, __LINE__, NULL);
            }
        }
    }
    else // If No
    {
        char dest_file[200];
#ifdef _WIN32
        remove_asterisk(folder);
#endif
        strcpy(dest_file, folder);
        strcat(dest_file, "CopyOf");
        strcat(dest_file, file_name);
        print_debug(dest_file, __FILE__, __LINE__, NULL);
    }
}

/* The file menu before any changes occur. Needs the full file path as an argument.
Provides options to write, copy, read and delete target file. (menu.c) */
void file_write_screen(char *file_path)
{
    clear_screen();
    char showpath[200];
    char folder[200];
#ifdef _WIN32
    strcpy(showpath, remove_substring(file_path, "mscache\\")); // file name without folder
#else
    strcpy(showpath, remove_substring(file_path, "mscache/"));
#endif
    print_debug("real path:", __FILE__, __LINE__, file_path, NULL);
    printf("\x1b[44m          Now editing \x1b[3m'%s'          \x1b[0m\n", showpath);
    printf("\n \x1b[47mw\x1b[0mrite into file    \x1b[47mr\x1b[0mead file\n\n");
    printf(" \x1b[47mc\x1b[0mopy file          \x1b[47md\x1b[0melete file\n\n");
    printf(" \x1b[47mm\x1b[0main menu                 \n");

    printf("\n ->");
    char response;
    do
    {
        response = getchar();
    } while (response != 'w' && response != 'c' && response != 'd' && response != 'r' && response != 'm');
    switch (response)
    {
    case 'w':
        // implement write
        print_ylw("Write mode", NULL);
        break;
    case 'r':
        // implement read
        print_grn("Read Mode", NULL);
        break;
    case 'c':
        // implement copy
        copy_miniscreen(file_path, showpath);
        // file_write_screen(file_path);
        break;
    case 'd':
        // implement delete
        print_red("Delete Mode", NULL);
        break;
    case 'm':
        // goes back to main
        break;
    default:
        break;
    }
}