#ifndef SACTION_H_
// Stands for "System Action", this is where file processes happen (create, copy, delete).
#define SACTION_H_

void copy_file(const char *src_filename, const char *dest_filename);
void file_constructor(char *folder_name, char *file_name);
int file_search_WIN32(char *folder_path, char *file_name);
int file_search_UNIX(char *folder_path, char *file_name);

#endif