#ifndef ACTOR_H_
// This is where file processes happen (create, copy, delete).
#define ACTOR_H_

void copy_file(const char *src_filename, const char *dest_filename);
void file_constructor(char *folder_name, char *file_name);
int file_search(char *folder_name, char *file_name);
void show_files(char *folder_path);
unsigned int line_reader(const char *src_path);
int word_count(const char *src_path);
#endif