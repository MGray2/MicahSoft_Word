#ifndef TOOLS_H_
// Set of helper functions for input and output.
#define TOOLS_H_

void clear_input_buffer(void);
void remove_newline(char *str);
int yes_no_response(void);
void clear_screen(void);
void print_debug(char *message, int line, ...);
void print_red(char *message);
void print_grn(char *message);
void print_blu(char *message);
void print_ylw(char *message);

#endif