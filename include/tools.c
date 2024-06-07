#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include "tools.h"

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

// Simply clears the screen. (tools.h)
void clear_screen(void)
{
    printf("\x1b[2J");
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

    printf("\x1b[35m%s\x1b[0m\n", buffer);
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