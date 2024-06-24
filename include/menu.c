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
#include <unistd.h>
#endif

// Title screen art. (menu.h)
void title_screen(void)
{
    printf(" \n");
    printf("\x1b[46m                                              \x1b[0m\n");
    printf("\x1b[46m         \x1b[0m       MicahSoft Word       \x1b[46m         \x1b[0m\n");
    printf("\x1b[46m         \x1b[0m         C Edition          \x1b[46m         \x1b[0m\n");
    printf("\x1b[46m                                              \x1b[0m\n");
    printf("\x1b[46m  \x1b[0m                                          \x1b[46m  \x1b[0m\n");
    printf("\x1b[46m  \x1b[0m       \x1b[30m\x1b[47mn\x1b[0mew file        \x1b[30m\x1b[47mf\x1b[0mind file          \x1b[46m  \x1b[0m\n");
    printf("\x1b[46m  \x1b[0m                                          \x1b[46m  \x1b[0m\n");
    printf("\x1b[46m  \x1b[0m       \x1b[30m\x1b[47mi\x1b[0mnformation     \x1b[30m\x1b[47mq\x1b[0muit               \x1b[46m  \x1b[0m\n");
    printf("\x1b[46m  \x1b[0m                                          \x1b[46m  \x1b[0m\n");
    printf("\x1b[46m                                              \x1b[0m\n");
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
    printf("\x1b[46m              New File              \x1b[0m\n");
    clear_input_buffer();
    printf("New file name: ");
    ne_input(file_name, sizeof(file_name));
    strcpy(folder_path, "mscache");

#ifdef _WIN32
    strcat(folder_path, "\\*");
#else
    strcat(folder_path, "/");
#endif

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
}

// Screen for locating existing files. Returns string of file path. (menu.h)
char *file_find_screen(void)
{
    char file_name[100];
    char folder_path[100];
    clear_screen();
    clear_input_buffer();
    printf("\x1b[46m             Locate File              \x1b[0m\n");
    strcpy(folder_path, "mscache");

#ifdef _WIN32
    strcat(folder_path, "\\*");
#else
    strcat(folder_path, "/");
#endif

    show_files(folder_path);
    printf("File name: ");
    ne_input(file_name, sizeof(file_name));

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
        else
        {
            return NULL;
        }
    }
}

// Private interface for copying targeted file on the writer screen (menu.c)
void copy_miniscreen(char *source_path, char *file_name)
{
    char folder[200];
#ifdef _WIN32
    strcpy(folder, "mscache\\*");
#else
    strcpy(folder, "mscache/");
#endif
    printf("Do you want to rename the copy? Y/N ");
    if (yes_no_response()) // If yes rename copy
    {
        char copy_name[100];
        clear_input_buffer();
        printf("File name: ");
        ne_input(copy_name, sizeof(copy_name));
        while (1)
        {
            if (file_search(folder, copy_name)) // If duplicate name
            {
                print_ylw("Name is already in use. Try again? Y/N ", NULL);
                if (yes_no_response()) // If yes
                {
                    copy_miniscreen(source_path, file_name);
                }
                else // If no
                {
                    char dest_file[200];
#ifdef _WIN32
                    remove_asterisk(folder);
#endif
                    strcpy(dest_file, folder);
                    strcat(dest_file, "CopyOf");
                    strcat(dest_file, file_name);
                    copy_file(source_path, dest_file);
                    break;
                }
            }
            else // If not duplicate name
            {
                char dest_file[200];
#ifdef _WIN32
                remove_asterisk(folder);
#endif
                strcpy(dest_file, folder);
                strcat(dest_file, copy_name);
                copy_file(source_path, dest_file);
                break;
            }
        }
    }
    else // If no rename copy
    {
        char dest_file[200];
#ifdef _WIN32
        remove_asterisk(folder);
#endif
        strcpy(dest_file, folder);
        strcat(dest_file, "CopyOf");
        strcat(dest_file, file_name);
        copy_file(source_path, dest_file);
    }
}

// Private interface for file deletions
int delete_miniscreen(char *source_file)
{
    char showpath[100];
#ifdef _WIN32
    strcpy(showpath, remove_substring(source_file, "mscache\\"));
#else
    strcpy(showpath, remove_substring(source_file, "mscache/"));
#endif
    print_red("Are you sure you want to delete", showpath, "? Y/N", NULL);
    if (confirmation())
    {
        if (remove(source_file) == 0)
        {
            print_grn("File", source_file, "was deleted successfully.", NULL);
            free(source_file);
            source_file = NULL;
            return 1;
        }
        else
        {
            perror("File deletion error");
            return -1;
        }
    }
    else
    {
        return 0;
    }
}

void write_miniscreen(char *source_file)
{
    FILE *file;
    char response[1024] = "";
    clear_input_buffer();

    while (1)
    {
        clear_screen();
        print_cyn("Write mode", NULL);
        printf("\x1b[3m\x1b[30m\x1b[46mAvailable commands: /clear  /replace  /remove  /undo  /shift  /quit \x1b[0m\n");
        unsigned int line_counter = line_reader(source_file);
        file = fopen(source_file, line_counter == 0 ? "w" : "a");
        if (line_counter == 0)
        {
            line_counter = 1;
        }
        printf("\x1b[34m%d:\x1b[0m ", line_counter);
        fgets(response, sizeof(response), stdin);
        if (strcmp(response, "/quit\n") == 0)
        {
            fclose(file);
            break;
        }
        if (strcmp(response, "/replace\n") == 0)
        {
            unsigned int line_target;
            char new_text[1024];
            Str_array arr;
            init_str_array(&arr, 2);
            read_file_to_array(source_file, &arr);
            while (1)
            {
                printf("Line number to replace: ");
                int status = scanf("%d", &line_target);
                clear_input_buffer();
                if (line_target < 0)
                {
                    continue;
                }
                if (status == 1)
                {
                    line_target--; // 0 based index
                    printf("New text: ");
                    fgets(new_text, sizeof(new_text), stdin);
                    remove_newline(new_text);
                    replace_line(&arr, line_target, new_text);
                    write_array_to_file(source_file, &arr);
                    free_str_array(&arr);
                    break;
                }
            }
            continue;
        }
        if (strcmp(response, "/clear\n") == 0)
        {
            print_red("Are you sure you want to clear all text in this file? Y/N ", NULL);
            if (confirmation())
            {
                fclose(file);
                FILE *c_file = fopen(source_file, "w");
                fclose(c_file);
            }
            continue;
        }
        if (strcmp(response, "/undo\n") == 0)
        {
            Str_array arr;
            init_str_array(&arr, 2);
            read_file_to_array(source_file, &arr);
            remove_last_line(&arr);
            write_array_to_file(source_file, &arr);
            free_str_array(&arr);
            continue;
        }
        if (strcmp(response, "/shift\n") == 0)
        {
            unsigned int line_target;
            printf("Shift at line: ");
            int status = scanf("%d", &line_target);
            clear_input_buffer();
            if (line_target < 0)
            {
                continue;
            }
            if (status == 1)
            {
                line_target--;
                Str_array arr;
                init_str_array(&arr, 2);
                read_file_to_array(source_file, &arr);
                insert_string_at(&arr, line_target, "");
                write_array_to_file(source_file, &arr);
                free_str_array(&arr);
            }
            continue;
        }
        if (strcmp(response, "/remove\n") == 0)
        {
            unsigned int line_target;
            printf("Remove at line: ");
            int status = scanf("%d", &line_target);
            clear_input_buffer();
            if (line_target < 0)
            {
                continue;
            }
            if (status == 1)
            {
                line_target--;
                Str_array arr;
                init_str_array(&arr, 2);
                read_file_to_array(source_file, &arr);
                remove_string_at(&arr, line_target);
                write_array_to_file(source_file, &arr);
                free_str_array(&arr);
            }
            continue;
        }
        fwrite(response, 1, strlen(response), file);
        line_counter++;
        fclose(file);
    }
}

/* The file menu before any changes occur. Needs the full file path as an argument.
Provides options to write, copy, read and delete target file. (menu.c) */
void file_write_screen(char *file_path)
{
    if (file_path == NULL)
    {
        return;
    }
    clear_screen();
    char showpath[100];
    char folder[200];
#ifdef _WIN32
    strcpy(showpath, remove_substring(file_path, "mscache\\")); // file name without folder
#else
    strcpy(showpath, remove_substring(file_path, "mscache/"));
#endif
    printf("\x1b[46m          Now editing \x1b[3m'%s'          \x1b[0m\n", showpath);
    printf("\n \x1b[30m\x1b[47mw\x1b[0mrite into file    \x1b[30m\x1b[47mr\x1b[0mead file\n\n");
    printf(" \x1b[30m\x1b[47mc\x1b[0mopy file          \x1b[30m\x1b[47md\x1b[0melete file\n\n");
    printf(" \x1b[30m\x1b[47mm\x1b[0main menu          word count: %d       \n", word_count(file_path));

    printf("\n ->");
    char response;
    do
    {
        response = getchar();
    } while (response != 'w' && response != 'c' && response != 'd' && response != 'r' && response != 'm');
    switch (response)
    {
    case 'w':
        // write file
        clear_screen();
        write_miniscreen(file_path);
        file_write_screen(file_path); // return
        break;
    case 'r':
        // read file
        clear_screen();
        print_cyn("Read Mode", NULL);
        if (line_reader(file_path) == 0)
        {
            print_ylw("This file is empty.", NULL);
        }
        print_blu("\nPress enter to continue.", NULL);
        pause_input();
        file_write_screen(file_path); // return
        break;
    case 'c':
        // copy file
        copy_miniscreen(file_path, showpath);
        file_write_screen(file_path); // return
        break;
    case 'd':
        // delete file
        if (!delete_miniscreen(file_path))
        {
            file_write_screen(file_path);
        }
        break;
    case 'm':
        // goes back to main
        break;
    default:
        break;
    }
}

void information_screen()
{
    char info_path[21];
    char file_name[] = "information";
#ifdef _WIN32
    strcpy(info_path, "mscache\\*");
#else
    strcpy(info_path, "mscache/");
#endif

    if (file_search(info_path, file_name))
    {
#ifdef _WIN32
        remove_asterisk(info_path);
#endif
        // open in read mode
        strcat(info_path, file_name);
        clear_screen();
        line_reader(info_path);
        print_blu("\nPress enter to continue.", NULL);
        pause_input();
    }
    else
    {
#ifdef _WIN32
        remove_asterisk(info_path);
#endif
        // could not read the file
        strcat(info_path, file_name);
        clear_screen();
        print_red("Cannot read from", info_path, ": File is missing.", NULL);
        print_blu("\nPress enter to continue.", NULL);
        pause_input();
    }
}