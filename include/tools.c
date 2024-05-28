#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include "tools.h"

// Consumes the input line
void clear_input_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

// Processes newline out of input
void remove_newline(char *str)
{
    if (str[strlen(str) - 1] == '\n')
    {
        str[strlen(str) - 1] = '\0';
    }
}

// Returns true if response is 'y' or 'Y', false otherwise.
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

// Simply clears the screen
void clear_screen(void)
{
    printf("\x1b[2J");
}

// Prints message suitable for debug purposes, use '__LINE__' for line argument.
// Additional arguments must be string, you must add 'NULL' at the end of the parameters.
void print_debug(char *message, int line, ...)
{
    va_list args;
    va_start(args, line);

    char buffer[1024];
    snprintf(buffer, sizeof(buffer), "***DEBUG*** (ln: %d) %s", line, message);

    const char *arg;
    while ((arg = va_arg(args, const char *)) != NULL)
    {
        strncat(buffer, " ", sizeof(buffer) - strlen(buffer) - 1);
        strncat(buffer, arg, sizeof(buffer) - strlen(buffer) - 1);
    }

    va_end(args);

    printf("\x1b[35m%s\x1b[0m\n", buffer);
}

// prints red text
void print_red(char *message)
{
    printf("\x1b[31m %s \x1b[0m\n", message);
}

// prints green text
void print_grn(char *message)
{
    printf("\x1b[32m %s \x1b[0m\n", message);
}

// prints blue text
void print_blu(char *message)
{
    printf("\x1b[34m %s \x1b[0m\n", message);
}

// prints yellow text
void print_ylw(char *message)
{
    printf("\x1b[33m %s \x1b[0m\n", message);
}