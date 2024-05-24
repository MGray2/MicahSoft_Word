#ifndef MENU_H_
#define MENU_H_

int yes_no_response(void);
void remove_newline(char *str);
void clear_screen(void);
void title_screen(void);
int title_selection(void);
void new_file_screen(void);
char *file_find_screen(void);
void copy_file_screen(void);
#endif