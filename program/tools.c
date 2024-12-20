#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include "tools.h"

#ifdef _WIN32 // for Windows users
#include <windows.h>
#else // for Unix users (macOS, Linux, etc.)
#include <dirent.h>
#include <unistd.h>
#endif

// Consumes the input line for future use. (tools.h)
void clear_input_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

// Processes the newline out of the string. (tools.h)
void remove_newline(char *str)
{
    if (str[strlen(str) - 1] == '\n')
    {
        str[strlen(str) - 1] = '\0';
    }
}

// Processes the asterisk out of the string, for windows wild search pattern. (tools.h)
void remove_asterisk(char *str)
{
    if (str[strlen(str) - 1] == '*')
    {
        str[strlen(str) - 1] = '\0';
    }
}

// Returns true if response is 'y' or 'Y', false otherwise. (tools.h)
int yes_no_response()
{
    char response;
    do
    {
        response = getchar();
    } while (tolower(response) != 'y' && tolower(response) != 'n');
    if (tolower(response) == 'y')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/* Similar to yes_no_response() but with a delayed timer for use with permanent decisions.
Allows string arguments to write warning message. (tools.h)
*/
int confirmation(const char *message, ...)
{
    char formatted_message[256];
    va_list args;
    va_start(args, message);
    vsnprintf(formatted_message, sizeof(formatted_message), message, args);
    va_end(args);

    char response;
    short loop = 5;
    while (loop > -1)
    {
#ifdef _WIN32
        Sleep(1000);
#else
        sleep(1);
#endif
        printf("\r\x1b[31m%s\x1b[0m %d", formatted_message, loop);
        fflush(stdout);
        loop--;
    }
    printf("\r\x1b[31m%s\x1b[0m %s", formatted_message, ">");
    fflush(stdout);
    do
    {
        response = getchar();
    } while (tolower(response) != 'y' && tolower(response) != 'n');
    if (tolower(response) == 'y')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// Simply clears the screen. (tools.h)
void clear_screen(void)
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/* Prints message suitable for debug purposes,
use '__FILE__' for file argument.
use '__LINE__' for line argument.
Additional arguments can be used to format to string. (tools.h) */
void print_debug(char *file, int line, char *message, ...)
{
    va_list args;
    va_start(args, message);

    char buffer[1024];
    snprintf(buffer, sizeof(buffer), "***DEBUG*** (%s ln:%d) ", file, line);

    vsnprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), message, args);

    va_end(args);

    printf("\x1b[45m\x1b[30m%s\x1b[0m\n", buffer);
}

/* Prints colored terminal text, additional arguments can be used to format variables into string.
Color arguments include: "red", "blue", "green", "yellow", "cyan" and "magenta"
Default color is white (tools.h) */
void print_clr(const char *color, const char *message, ...)
{
    char font_color[10];
    if (strcmp(color, "red") == 0)
    {
        strcpy(font_color, "\x1b[31m");
    }
    else if (strcmp(color, "blue") == 0)
    {
        strcpy(font_color, "\x1b[34m");
    }
    else if (strcmp(color, "green") == 0)
    {
        strcpy(font_color, "\x1b[32m");
    }
    else if (strcmp(color, "yellow") == 0)
    {
        strcpy(font_color, "\x1b[33m");
    }
    else if (strcmp(color, "cyan") == 0)
    {
        strcpy(font_color, "\x1b[36m");
    }
    else if (strcmp(color, "magenta") == 0)
    {
        strcpy(font_color, "\x1b[35m");
    }
    else
    {
        strcpy(font_color, "");
    }

    va_list args;
    va_start(args, message);

    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), message, args);

    va_end(args);

    printf("%s%s\x1b[0m\n", font_color, buffer);
}

/* Prints enumerated colored lines with formatted spacing, for use with read and write menus.
color arguments include: "red", "green" and "blue". (tools.h) */
void print_enum(const char *color, const unsigned int number_counter, char *message)
{
    char font_color[10];
    if (strcmp(color, "red") == 0)
    {
        strcpy(font_color, "\x1b[31m");
    }
    else if (strcmp(color, "blue") == 0)
    {
        strcpy(font_color, "\x1b[34m");
    }
    else if (strcmp(color, "green") == 0)
    {
        strcpy(font_color, "\x1b[32m");
    }
    else
    {
        strcpy(font_color, "");
    }

    // Print the line number and the line
    if (number_counter < 10)
    {
        printf("%s   %d:\x1b[0m %s\n", font_color, number_counter, message); // 3 spaces
    }
    else if (number_counter < 100)
    {
        printf("%s  %d:\x1b[0m %s\n", font_color, number_counter, message); // 2 spaces
    }
    else if (number_counter < 1000)
    {
        printf("%s %d:\x1b[0m %s\n", font_color, number_counter, message); // 1 space
    }
    else
    {
        printf("%s%d:\x1b[0m %s\n", font_color, number_counter, message); // no space
    }
}

/* Prints enumerated colored input with formatted spacing, for use with writer.
color arguments include: "red", "green" and "blue". (tools.h) */
void print_eninp(const char *color, const unsigned int number_counter)
{
    char font_color[10];
    if (strcmp(color, "red") == 0)
    {
        strcpy(font_color, "\x1b[31m");
    }
    else if (strcmp(color, "blue") == 0)
    {
        strcpy(font_color, "\x1b[34m");
    }
    else if (strcmp(color, "green") == 0)
    {
        strcpy(font_color, "\x1b[32m");
    }
    else
    {
        strcpy(font_color, "");
    }
    // Print the line number and the line
    if (number_counter < 10)
    {
        printf("%s   %d:\x1b[0m ", font_color, number_counter); // 3 spaces
    }
    else if (number_counter < 100)
    {
        printf("%s  %d:\x1b[0m ", font_color, number_counter); // 2 spaces
    }
    else if (number_counter < 1000)
    {
        printf("%s %d:\x1b[0m ", font_color, number_counter); // 1 space
    }
    else
    {
        printf("%s%d:\x1b[0m ", font_color, number_counter); // no space
    }
}

// Prints enumerated yellow line with formatted spacing, for use with reading directory. (tools.h)
void print_enylw(const unsigned int number_counter, char *message)
{
    // Print the line number and the line
    if (number_counter < 10)
    {
        printf("\x1b[33m    !: %s\x1b[0m\n", message); // 4 spaces
    }
    else if (number_counter < 100)
    {
        printf("\x1b[33m   !: %s\x1b[0m\n", message); // 3 spaces
    }
    else if (number_counter < 1000)
    {
        printf("\x1b[33m  !: %s\x1b[0m\n", message); // 2 spaces
    }
    else
    {
        printf("\x1b[33m !: %s\x1b[0m\n", message); // 1 space
    }
}

// Removes the substring from the given string, returns a new string. (tools.h)
char *remove_substring(const char *str, const char *sub)
{
    size_t len = strlen(str);
    size_t sub_len = strlen(sub);
    char *result = (char *)malloc(len + 1);
    char *p_result = result;

    if (!result)
    {
        return NULL;
    }

    const char *p = str;
    const char *next;

    while ((next = strstr(p, sub)) != NULL)
    {
        // Copy characters before the substring
        size_t num_chars = next - p;
        strncpy(p_result, p, num_chars);
        p_result += num_chars;
        p = next + sub_len;
    }
    // Copy the remaining part of the original string
    strcpy(p_result, p);
    return result;
}

/* Non-empty input, asks for input until input is at least one character.
first arg is variable to recieve the return value, second arg is input capture size.
This function will automatically call 'remove_newline()' on itself. (tools.h) */
void ne_input(char *dest_str, int input_size)
{
    fgets(dest_str, input_size, stdin);
    while (strlen(dest_str) < 2)
    {
        fgets(dest_str, input_size, stdin);
    }
    remove_newline(dest_str);
}

// Halts the program until 'enter' is pressed. (tools.h)
void pause_input(void)
{
    char input[2] = "";
    fgets(input, sizeof(input), stdin);
}

// Returns true if file is empty, false otherwise. Calls 'rewind()' on file. (tools.h)
int is_file_empty(FILE *file)
{
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);
    return size == 0 ? 1 : 0;
}

// Evaluates a string to confirm that every character is a number and can be safely converted to int (tools.h)
int is_integer(const char *str)
{
    if (*str == '-' || *str == '+') // skipping the number sign
    {
        str++;
    }
    if (!*str) // if empty
    {
        return 0;
    }
    while (*str)
    {
        if (!isdigit(*str)) // if at any point the string isnt a number
        {
            return 0;
        }
        *str++;
    }
    return 1;
}

/*
Interprets strings that follow command formatting and returns a number depending on command type.
0 = none
1 = quit
2 = replace
3 = clear
4 = undo
5 = shift
6 = remove
7 = copy
8 = cut
9 = help
*/
int command_detector(char *str)
{
    if (strncmp(str, "/quit", 5) == 0)
        return 1;
    else if (strncmp(str, "/replace", 8) == 0)
        return 2;
    else if (strncmp(str, "/clear", 6) == 0)
        return 3;
    else if (strncmp(str, "/undo", 5) == 0)
        return 4;
    else if (strncmp(str, "/shift", 6) == 0)
        return 5;
    else if (strncmp(str, "/remove", 7) == 0)
        return 6;
    else if (strncmp(str, "/copy", 5) == 0)
        return 7;
    else if (strncmp(str, "/cut", 4) == 0)
        return 8;
    else if (strncmp(str, "/help", 5) == 0)
        return 9;
    else
        return 0;
}

/* Collects numbers from a string and returns an array of int.
&count argument must equal 0 and will be updated after function call (tools.h) */
int *number_extractor(char *str, int *count)
{
    int *numbers = NULL; // dynamic array
    int capacity = 0;    // capacity of the numbers array
    *count = 0;          // initialize count to 0
    while (*str)
    {
        // skip non-number characters
        if (!isdigit(*str) && *str != '-' && *str != '+')
        {
            str++;
            continue;
        }

        char buffer[20]; // buffer to hold number string
        int i = 0;

        if (*str == '-' || *str == '+')
        {
            buffer[i++] = *str++;
        }
        while (isdigit(*str))
        {
            buffer[i++] = *str++;
        }
        buffer[i] = '\0'; // null-terminate the buffer

        if (i > 0) // if numbers
        {
            int num = atoi(buffer); // str -> int

            // Resize the array if needed
            if (*count >= capacity)
            {
                capacity = (capacity == 0) ? 1 : capacity * 2;
                int *temp = realloc(numbers, capacity * sizeof(int));

                if (temp == NULL)
                {
                    free(numbers); // free previously allocated memory
                    return NULL;   // failure
                }
                numbers = temp; // update numbers to the new memory
            }
            numbers[*count] = num; // store the number
            (*count)++;
        }
    }
    return numbers;
}