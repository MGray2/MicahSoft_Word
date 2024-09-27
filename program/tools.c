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
Additional arguments must be string, you must add 'NULL' at the end of the parameters. (tools.h) */
void print_debug(char *message, char *file, int line, ...)
{
    va_list args;
    va_start(args, line);

    char buffer[1024];
    snprintf(buffer, sizeof(buffer), "***DEBUG*** (%s ln:%d) %s", file, line, message);

    const char *arg;
    while ((arg = va_arg(args, const char *)) != NULL)
    {
        strncat(buffer, " ", sizeof(buffer) - strlen(buffer) - 1);
        strncat(buffer, arg, sizeof(buffer) - strlen(buffer) - 1);
    }

    va_end(args);

    printf("\x1b[45m\x1b[30m%s\x1b[0m\n", buffer);
}

// Prints terminal red text, additional arguments must be string and end with NULL. (tools.h)
void print_red(char *message, ...)
{
    va_list args;
    va_start(args, message);

    char buffer[1024];
    snprintf(buffer, sizeof(buffer), "%s", message);

    const char *arg;
    while ((arg = va_arg(args, const char *)) != NULL)
    {
        strncat(buffer, " ", sizeof(buffer) - strlen(buffer) - 1);
        strncat(buffer, arg, sizeof(buffer) - strlen(buffer) - 1);
    }
    va_end(args);

    printf("\x1b[31m%s\x1b[0m\n", buffer);
}

// Prints terminal green text, additional arguments must be string and end with NULL. (tools.h)
void print_grn(char *message, ...)
{
    va_list args;
    va_start(args, message);

    char buffer[1024];
    snprintf(buffer, sizeof(buffer), "%s", message);

    const char *arg;
    while ((arg = va_arg(args, const char *)) != NULL)
    {
        strncat(buffer, " ", sizeof(buffer) - strlen(buffer) - 1);
        strncat(buffer, arg, sizeof(buffer) - strlen(buffer) - 1);
    }
    va_end(args);

    printf("\x1b[32m%s\x1b[0m\n", buffer);
}

// Prints terminal blue text, additional arguments must be string and end with NULL. (tools.h)
void print_blu(char *message, ...)
{
    va_list args;
    va_start(args, message);

    char buffer[1024];
    snprintf(buffer, sizeof(buffer), "%s", message);

    const char *arg;
    while ((arg = va_arg(args, const char *)) != NULL)
    {
        strncat(buffer, " ", sizeof(buffer) - strlen(buffer) - 1);
        strncat(buffer, arg, sizeof(buffer) - strlen(buffer) - 1);
    }
    va_end(args);

    printf("\x1b[34m%s\x1b[0m\n", buffer);
}

// Prints terminal yellow text, additional arguments must be string and end with NULL. (tools.h)
void print_ylw(char *message, ...)
{
    va_list args;
    va_start(args, message);

    char buffer[1024];
    snprintf(buffer, sizeof(buffer), "%s", message);

    const char *arg;
    while ((arg = va_arg(args, const char *)) != NULL)
    {
        strncat(buffer, " ", sizeof(buffer) - strlen(buffer) - 1);
        strncat(buffer, arg, sizeof(buffer) - strlen(buffer) - 1);
    }
    va_end(args);

    printf("\x1b[33m%s\x1b[0m\n", buffer);
}

// Prints terminal cyan text, additional arguments must be string and end with NULL. (tools.h)
void print_cyn(char *message, ...)
{
    va_list args;
    va_start(args, message);

    char buffer[1024];
    snprintf(buffer, sizeof(buffer), "%s", message);

    const char *arg;
    while ((arg = va_arg(args, const char *)) != NULL)
    {
        strncat(buffer, " ", sizeof(buffer) - strlen(buffer) - 1);
        strncat(buffer, arg, sizeof(buffer) - strlen(buffer) - 1);
    }
    va_end(args);

    printf("\x1b[36m%s\x1b[0m\n", buffer);
}

// Prints terminal magenta text, additional arguments must be string and end with NULL. (tools.h)
void print_mgt(char *message, ...)
{
    va_list args;
    va_start(args, message);

    char buffer[1024];
    snprintf(buffer, sizeof(buffer), "%s", message);

    const char *arg;
    while ((arg = va_arg(args, const char *)) != NULL)
    {
        strncat(buffer, " ", sizeof(buffer) - strlen(buffer) - 1);
        strncat(buffer, arg, sizeof(buffer) - strlen(buffer) - 1);
    }
    va_end(args);

    printf("\x1b[35m%s\x1b[0m\n", buffer);
}

/* Prints enumerated colored lines with formatted spacing, for use with read and write menus.
color arguments include: "red", "green" and "blue". (tools.h) */
void print_enum(char *color, const unsigned int number_counter, char *text)
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
        printf("%s   %d:\x1b[0m %s\n", font_color, number_counter, text); // 3 spaces
    }
    else if (number_counter < 100)
    {
        printf("%s  %d:\x1b[0m %s\n", font_color, number_counter, text); // 2 spaces
    }
    else if (number_counter < 1000)
    {
        printf("%s %d:\x1b[0m %s\n", font_color, number_counter, text); // 1 space
    }
    else
    {
        printf("%s%d:\x1b[0m %s\n", font_color, number_counter, text); // no space
    }
}

// Prints enumerated blue input with formatted spacing, for use with writer. (tools.h)
void print_eninp(const unsigned int number_counter)
{
    // Print the line number and the line
    if (number_counter < 10)
    {
        printf("\x1b[34m   %d:\x1b[0m ", number_counter); // 3 spaces
    }
    else if (number_counter < 100)
    {
        printf("\x1b[34m  %d:\x1b[0m ", number_counter); // 2 spaces
    }
    else if (number_counter < 1000)
    {
        printf("\x1b[34m %d:\x1b[0m ", number_counter); // 1 space
    }
    else
    {
        printf("\x1b[34m%d:\x1b[0m ", number_counter); // no space
    }
}

// Prints enumerated yellow line with formatted spacing, for use with reading directory. (tools.h)
void print_enylw(const unsigned int number_counter, char *text)
{
    // Print the line number and the line
    if (number_counter < 10)
    {
        printf("\x1b[33m    !: %s\x1b[0m\n", text); // 4 spaces
    }
    else if (number_counter < 100)
    {
        printf("\x1b[33m   !: %s\x1b[0m\n", text); // 3 spaces
    }
    else if (number_counter < 1000)
    {
        printf("\x1b[33m  !: %s\x1b[0m\n", text); // 2 spaces
    }
    else
    {
        printf("\x1b[33m !: %s\x1b[0m\n", text); // 1 space
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
    clear_input_buffer();
    clear_input_buffer();
}

// Returns true if file is empty, false otherwise. Calls 'rewind()' on file. (tools.h)
int is_file_empty(FILE *file)
{
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);
    return size == 0 ? 1 : 0;
}