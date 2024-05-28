#ifndef CACTION_H_
// Stands for "Computer Action", this is where file processes happen (create, copy, delete).
#define CACTION_H_

void copy_file(const char *src_filename, const char *dest_filename);
void file_constructor(char *folder_name, char *file_name);

#endif