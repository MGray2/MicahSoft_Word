# include "../headers/menu.h"
# include <ctype.h>
# include <stdio.h>
# include <string.h>


#ifdef _WIN32 // for Windows users
#include <windows.h>
#else // for Unix users (macOS, Linux, etc.)
#include <dirent.h>
#endif

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void remove_newline(char *str) {
    if (str[strlen(str) - 1] == '\n') {
        str[strlen(str) - 1] = '\0'; 
    }
}

int yes_no_response() {
    char response;
    do {
    response = getchar();
    } while (tolower(response) != 'y' && tolower(response) != 'n');
    if (tolower(response) == 'y') {
        return 1;
    } else {
        return 0;
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
    clear_input_buffer();
    printf("New file name: "); 
    fgets(file_name, sizeof(file_name), stdin);
    remove_newline(file_name);
    strcpy(folder_path, "mscache");
    
    #ifdef _WIN32
    strcat(folder_path, "\\");
    #else
    strcat(folder_path, "/");
    #endif

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
        printf("File '%s' already exists in the directory. Rename? Y/N ", file_name);
            if (yes_no_response()) {
                new_file_screen();
                // recurse
            } else {
                char new_file_name[120];  
                strcpy(new_file_name, "CopyOf");
                strcat(new_file_name, file_name);
                file_constructor(folder_path, new_file_name);
            }
        
    } else {
        // create the file
        file_constructor(folder_path, file_name);
    }
    // For Unix
    #else 
    
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(folder_path)) != NULL) {
        int file_found = 0;
        while ((ent = readdir(dir)) != NULL) {
            printf("%s\n", ent->d_name); // show files
            if (strcmp(ent->d_name, file_name) == 0) {
                printf("\x1b[31m%s\x1b[0m\n", ent->d_name); // red highlight for duplicate
                file_found = 1;
                break;
            }
        }
        closedir(dir);

        if (file_found) {
            printf("File '%s' already exists in the directory. Rename? Y/N ", file_name);
            if (yes_no_response()) {
                new_file_screen();
                // recurse
            } else {
                char new_file_name[120];  
                strcpy(new_file_name, "CopyOf");
                strcat(new_file_name, file_name);
                file_constructor(folder_path, new_file_name);
            }

        } else {
            // create the file
            file_constructor(folder_path, file_name);
        }
    } else {
        // if the folder could not be found
        printf("Error opening directory: %s\n", folder_path);
        return;
    }
    #endif
}

void file_constructor(char folder_name[], char file_name[]) {
    remove_newline(file_name);
    char full_path_name[200];
    sprintf(full_path_name, "%s%s", folder_name, file_name); // construct path as "folder/filename"
    FILE *file = fopen(full_path_name, "w");
    fclose(file);
}

void file_find_screen(void) {
    char file_name[100];
    char folder_path[100];
    clear_screen();
    clear_input_buffer();
    printf("File name: "); 
    // come back later
}

// arg1 = source file, arg2 = new file
void copy_file(const char *src_file, const char *dest_file);

void copy_file_screen(void) {
    char file_name[100];
    char folder_path[100];
    clear_screen();
    clear_input_buffer();
    strcpy(folder_path, "mscache");

    #ifdef _WIN32
    strcat(folder_path, "\\*"); // Append * to the folder path to list all files
    WIN32_FIND_DATA find_file_data;
    HANDLE hFind = FindFirstFile(folder_path, &find_file_data); 
    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Error opening directory: %s\n", folder_path);
        return; 
    }
    int file_found = 0;
    do {
        printf("%s\n", find_file_data.cFileName); // Print each file name
        
    } while (FindNextFile(hFind, &find_file_data) != 0);
    FindClose(hFind);
    #else
    // For Unix
    strcat(folder_path, "/");
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(folder_path)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            printf("%s\n", ent->d_name); // show files for reference
        }
        closedir(dir);
    #endif
    }
    printf("File name: "); 
    fgets(file_name, sizeof(file_name), stdin);
    remove_newline(file_name);

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
            // copy the file
            char dest_file[120];
            strcpy(dest_file, "CopyOf");
            strcat(dest_file, file_name);
            file_constructor(folder_path, dest_file);
            printf("Copying into '%s'.\n", dest_file);
            copy_file(file_name, dest_file);
        } else {
            // couldnt copy the file
            printf("File not found. Try again? Y/N ");
            if (yes_no_response()) {
                copy_file_screen();
                // recurse
            } else {
                return;
            }
        }
    }
    
}

void copy_file(const char *src_filename, const char *dest_filename) {
    FILE *src_file = fopen(src_filename, "rb"); // read from
    if (src_file == NULL) {
        printf("Could not open '%s'. ", src_filename);
        return;
    }
    FILE *dest_file = fopen(dest_filename, "wb"); // write to
    if (dest_file == NULL) {
        printf("Could not open '%s'. ", dest_filename);
        fclose(src_file);  // Close the source file before returning
        return;
    }
    char buffer[1024];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), src_file)) > 0) {
        fwrite(buffer, 1, bytes_read, dest_file);
    }

    fclose(src_file);
    fclose(dest_file);
}
