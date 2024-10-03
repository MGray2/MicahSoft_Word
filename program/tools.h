#ifndef TOOLS_H_
// Set of helper functions for input and output.
#define TOOLS_H_
#include <stdio.h>

void clear_input_buffer(void);
void remove_newline(char *str);
void remove_asterisk(char *str);
char *remove_substring(const char *str, const char *sub);
int yes_no_response(void);
int confirmation(const char *message, ...);
void ne_input(char *dest_str, int input_size);
void pause_input(void);
void clear_screen(void);
int is_file_empty(FILE *file);
int is_integer(const char *str);
void print_debug(char *file, int line, char *message, ...);
void print_clr(const char *color, const char *message, ...);
void print_enum(const char *color, const unsigned int number_counter, char *message);
void print_eninp(const char *color, const unsigned int number_counter);
void print_enylw(const unsigned int number_counter, char *message);
int command_detector(char *str);
int *number_extractor(char *str, int *count);

#endif