#ifndef MENU_H_
// Set of functions for displaying menus and gathering responses.
#define MENU_H_

void title_screen(void);
int title_selection(void);
char *new_file_screen(void);
char *file_find_screen(void);
void file_write_screen(char *file_path);
#endif