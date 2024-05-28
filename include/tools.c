#include <stdio.h>
#include <string.h>
#include <ctype.h>
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