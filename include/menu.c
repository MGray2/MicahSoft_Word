# include "../headers/menu.h"
# include <ctype.h>
# include <stdio.h>
# include <string.h>
# include <stdbool.h>

#ifdef _WIN32 // for Windows users
#include <windows.h>
#else // for Unix users (macOS, Linux, etc.)
#include <dirent.h>
#endif

bool yes_no_response() {
    char response;
    do {
    response = getchar();
    } while (tolower(response) != 'y' && tolower(response) != 'n');
    if (tolower(response) == 'y') {
        return true;
    } else {
        return false;
    }
}

void clear_screen(void) {
    printf("\x1b[2J"); // Simply clears the screen
}

void title_screen(void) {
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

int title_selection(void) {
    char response; 
    // translator for title action
    do {
        clear_screen();
        title_screen();
        printf("->");
        response = getchar();

    } while (response != 'n' && response != 'f' && response !='c' && response != 'd' && response != 'i' && response != 'q');

    switch (tolower(response)) {
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

void file_constructor(char folder_name[], char file_name[]);

void new_file_screen(void) {
    char file_name[100];
    char folder_path[100];
    clear_screen();
    printf("New file name: ");
    fgets(file_name, sizeof(file_name), stdin);

    strcpy(folder_path, "mscache");

    #ifdef _WIN32
    strcat(folder_path, "\\");
    #else
    strcat(folder_path, "/");
    #endif

    clear_screen();
    printf("New file name: ");
    fgets(file_name, sizeof(file_name), stdin);

    // For Windows
    #ifdef _WIN32 
    WIN32_FIND_DATA find_file_data;
    HANDLE hFind = FindFirstFile(folder_path, &find_file_data); 
    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Error opening directory: %s\n", folder_path);
        return; 
    }
    int file_found = 0;
    do {
        if (strcmp(find_file_data.cFileName, file_name) == 0) {
            file_found = 1;
            break;
        }
    } while (FindNextFile(hFind, &find_file_data) != 0);
    FindClose(hFind);

    if (file_found) {
        printf("File '%s' already exists in the directory.\n", file_name);
    } else {
        file_constructor(folder_path, file_name);
    }
    // For Unix
    #else 
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(folder_path)) != NULL) {
        int file_found = 0;
        while ((ent = readdir(dir)) != NULL) {
            if (strcmp(ent->d_name, file_name) == 0) {
                file_found = 1;
                break;
            }
        }
        closedir(dir);

        if (file_found) {
            printf("File '%s' already exists in the directory. Rename? Y/N. \n", file_name);
            if (yes_no_response()) {
                new_file_screen();
                // recurse
            }

        } else {
            file_constructor(folder_path, file_name);
        }
    } else {
        printf("Error opening directory: %s\n", folder_path);
        return;
    }
    #endif
}

void file_constructor(char folder_name[], char file_name[]) {
    if (file_name[strlen(file_name) - 1] == '\n') {
            file_name[strlen(file_name) - 1] = '\0'; // replace carriage return with null terminator
            }
            char full_path_name[200];
            sprintf(full_path_name, "%s%s", folder_name, file_name); // construct path with folder/filename
            FILE *file = fopen(full_path_name, "w");
            fclose(file);
}
