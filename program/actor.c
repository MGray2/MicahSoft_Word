#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tools.h"

#ifdef _WIN32 // for Windows users
#include <windows.h>
#else // for Unix users (macOS, Linux, etc.)
#include <dirent.h>
#include <sys/types.h>
#endif

// Copies the first argument to the address of the second argument. (actor.h)
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

// Creates files by given path arguments. (actor.h)
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
Returns true if duplicate located, false otherwise. (actor.h)*/
int file_search(char *folder_name, char *file_name)
{
    int file_found = 0;
    unsigned int item_counter = 1;
#ifdef _WIN32
    WIN32_FIND_DATA find_file_data;
    HANDLE hFind = FindFirstFile(folder_name, &find_file_data);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf("Error opening directory: %s\n", folder_name);
        return 0;
    }
    do
    {
        if (strcmp(find_file_data.cFileName, "..") != 0 && strcmp(find_file_data.cFileName, ".") != 0)
        {
            print_enum("green", item_counter, find_file_data.cFileName); // Print each file name
            if (strcmp(find_file_data.cFileName, file_name) == 0)
            {
                print_enylw(item_counter, find_file_data.cFileName);
                file_found = 1;
                break;
            }
            item_counter++;
        }
    } while (FindNextFile(hFind, &find_file_data) != 0);
    FindClose(hFind);
#else
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(folder_path)) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (strcmp(ent->d_name, "..") != 0 && strcmp(ent->d_name, ".") != 0)
            {
                print_enum("green", item_counter, ent->d_name);
                if (strcmp(ent->d_name, file_name) == 0)
                {
                    print_enylw(item_counter, ent->d_name);
                    file_found = 1;
                    break;
                }
                item_counter++;
            }
        }
        closedir(dir);
    }
#endif
    if (file_found)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// Similar to file_search but uses index instead of file name, returns folder name (actor.h)
char *file_index(const char *folder_path, const int index)
{
    unsigned int item_counter = 0;
#ifdef _WIN32
    WIN32_FIND_DATA find_file_data;
    HANDLE hFind = FindFirstFile(folder_path, &find_file_data);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf("Error opening directory: %s\n", folder_path);
        return NULL;
    }
    do
    {
        if (strcmp(find_file_data.cFileName, "..") != 0 && strcmp(find_file_data.cFileName, ".") != 0)
        {
            if (item_counter == index)
            {
                FindClose(hFind);
                return strdup(find_file_data.cFileName); // return file name
            }
            item_counter++;
        }
    } while (FindNextFile(hFind, &find_file_data) != 0);
    FindClose(hFind);
#else
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(folder_path)) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (strcmp(ent->d_name, "..") != 0 && strcmp(ent->d_name, ".") != 0)
            {
                if (item_counter == index)
                {
                    closedir(dir);
                    return strdup(ent->d_name); // return file name
                }
                item_counter++;
            }
        }
        closedir(dir);
    }
    else
    {
        printf("Error opening directory: %s\n", folder_path);
        return NULL;
    }
#endif
}

// Simply displays all file names under the folder. (actor.h)
void show_files(char *folder_path)
{
    unsigned int item_counter = 1;
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
        if (strcmp(find_file_data.cFileName, "..") != 0 && strcmp(find_file_data.cFileName, ".") != 0)
        {
            print_enum("green", item_counter, find_file_data.cFileName); // Print each file name
            item_counter++;
        }

    } while (FindNextFile(hFind, &find_file_data) != 0);
    FindClose(hFind);
#else
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(folder_path)) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (strcmp(ent->d_name, "..") != 0 && strcmp(ent->d_name, ".") != 0)
            {
                print_enum("green", item_counter, ent->d_name);
                item_counter++;
            }
        }
        closedir(dir);
    }
#endif
}

// For use in write mode, scans file path and writes line-by-line enumerated text into output. Returns last number in the line counter. (actor.h)
unsigned int line_reader(const char *src_path)
{
    char *buffer = NULL;
    size_t bufsize = 0;
    ssize_t line_length;
    unsigned int line_counter = 1;
    FILE *file = fopen(src_path, "r");
    if (is_file_empty(file))
    {
        return 0;
    }
    while ((line_length = getline(&buffer, &bufsize, file)) != -1)
    {
        if (line_length == -1)
        {
            if (feof(file))
            {
                // End of file reached
                return 0;
            }
            else
            {
                perror("Error reading line");
                return 0;
            }
        }
        // Remove the newline character
        if (buffer[line_length - 1] == '\n')
        {
            buffer[line_length - 1] = '\0';
        }

        // Print the line number and the line
        print_enum("blue", line_counter, buffer);
        line_counter++;
    }
    free(buffer);
    fclose(file);
    return line_counter;
}

// Line reader function without a number counter for use with read mode. Only returns 0 if file empty. (actor.h)
unsigned int line_reader_nonum(const char *src_path)
{
    char *buffer = NULL;
    size_t bufsize = 0;
    ssize_t line_length;
    FILE *file = fopen(src_path, "r");
    if (is_file_empty(file))
    {
        return 0;
    }
    while ((line_length = getline(&buffer, &bufsize, file)) != -1)
    {
        if (line_length == -1)
        {
            if (feof(file))
            {
                // End of file reached
                return 0;
            }
            else
            {
                perror("Error reading line");
                return 0;
            }
        }
        // Remove the newline character
        if (buffer[line_length - 1] == '\n')
        {
            buffer[line_length - 1] = '\0';
        }

        // Print the line number and the line
        printf("%s\n", buffer);
    }
    free(buffer);
    fclose(file);
    return 1;
}

// Returns the word count of a file. (actor.h)
int word_count(const char *src_path)
{
    FILE *file = fopen(src_path, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return -1;
    }

    int word_count = 0;
    int ch;
    int in_word = 0; // Flag to indicate if we are inside a word

    // Read characters one by one until EOF is reached
    while ((ch = fgetc(file)) != EOF)
    {
        if (isspace(ch) || ch == '\n')
        {
            if (in_word)
            {
                in_word = 0; // Not inside a word anymore
                word_count++;
            }
        }
        else
        {
            in_word = 1; // Inside a word
        }
    }

    // Check if the last word was counted
    if (in_word)
    {
        word_count++;
    }

    fclose(file);
    return word_count;
}

// structure for adaptable string array (actor.h)
typedef struct
{
    char **array;
    size_t size;
    size_t capacity;
} Str_array;

// Constructor for Str_array (actor.h)
void init_str_array(Str_array *arr, size_t initial_capacity)
{
    arr->array = malloc(initial_capacity * sizeof(char *));
    arr->size = 0;
    arr->capacity = initial_capacity;
}

// Helper function to upgrade Str_array size (actor.h)
void resize_str_array(Str_array *arr)
{
    arr->capacity *= 2;
    arr->array = realloc(arr->array, arr->capacity * sizeof(char *));
}

// Helper function to add a string to Str_array, will call resize function. (actor.h)
void add_str(Str_array *arr, const char *str)
{
    if (arr->size >= arr->capacity)
    {
        resize_str_array(arr);
    }
    arr->array[arr->size] = malloc((strlen(str) + 1) * sizeof(char));
    strcpy(arr->array[arr->size], str);
    arr->size++;
}

// Helper function to free allocated memory of Str_array. (actor.h)
void free_str_array(Str_array *arr)
{
    for (size_t i = 0; i < arr->size; i++)
    {
        free(arr->array[i]);
    }
    free(arr->array);
}

// Reads from file and writes to Str_array. (actor.h)
void read_file_to_array(const char *file_name, Str_array *arr)
{
    FILE *file = fopen(file_name, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, file)) != -1)
    {
        // Remove the newline character if present
        if (line[read - 1] == '\n')
        {
            line[read - 1] = '\0';
        }
        add_str(arr, line);
    }

    free(line);
    fclose(file);
}

// Replaces the targeted line in Str_array. (actor.h)
void replace_line(Str_array *arr, size_t line_number, const char *new_line)
{
    if (line_number < arr->size)
    {
        free(arr->array[line_number]);                                           // Free the old line
        arr->array[line_number] = malloc((strlen(new_line) + 1) * sizeof(char)); // Allocate memory for the new line
        strcpy(arr->array[line_number], new_line);                               // Copy the new line into the allocated memory
    }
}

// Overwrites file with contents of Str_array. (actor.h)
void write_array_to_file(const char *file_name, const Str_array *arr)
{
    FILE *file = fopen(file_name, "w");
    if (file == NULL)
    {
        perror("Error opening file for writing");
        return;
    }

    for (size_t i = 0; i < arr->size; i++)
    {
        // Write each string to the file
        fprintf(file, "%s\n", arr->array[i]);
    }

    fclose(file);
}

// Remove the last line in Str_array
void remove_last_line(Str_array *arr)
{
    if (arr->size > 0)
    {
        // Free the memory for the last line
        free(arr->array[arr->size - 1]);
        // Decrease the size of the array
        arr->size--;
    }
    else
    {
        printf("The array is empty, no line to remove.\n");
    }
}

// Places a string in the Str_array and moves all strings after it down by one place. (actor.h)
void insert_string_at(Str_array *arr, size_t index, const char *str)
{
    if (index > arr->size)
    {
        return; // Invalid index
    }

    if (arr->size == arr->capacity)
    {
        arr->capacity *= 2;
        arr->array = realloc(arr->array, arr->capacity * sizeof(char *));
    }

    // Shift elements down
    for (size_t i = arr->size; i > index; i--)
    {
        arr->array[i] = arr->array[i - 1];
    }

    // Insert the new string
    arr->array[index] = strdup(str);
    arr->size++;
}

// Removes specified line and shifts below line up. (actor.h)
void remove_string_at(Str_array *arr, size_t index)
{
    if (index >= arr->size)
    {
        return; // Invalid index
    }

    // Free the memory for the string at the index
    free(arr->array[index]);

    // Shift elements up
    for (size_t i = index; i < arr->size - 1; i++)
    {
        arr->array[i] = arr->array[i + 1];
    }

    arr->size--;
}