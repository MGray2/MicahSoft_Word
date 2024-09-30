#ifndef ACTOR_H_
// This is where file processes happen (create, copy, delete).
#define ACTOR_H_
#include <stddef.h>

void copy_file(const char *src_filename, const char *dest_filename);
void file_constructor(char *folder_name, char *file_name);
int file_search(char *folder_name, char *file_name);
char *file_index(const char *folder_path, const int index);
void show_files(char *folder_path);
unsigned int line_reader(const char *src_path);
unsigned int line_reader_nonum(const char *src_path);
int word_count(const char *src_path);

typedef struct
{
    char **array;
    size_t size;
    size_t capacity;
} Str_array;

void init_str_array(Str_array *arr, size_t initial_capacity);
void resize_str_array(Str_array *arr);
void add_str(Str_array *arr, const char *str);
void free_str_array(Str_array *arr);
void read_file_to_array(const char *file_name, Str_array *arr);
void replace_line(Str_array *arr, size_t line_number, const char *new_line);
void write_array_to_file(const char *file_name, const Str_array *arr);
void remove_last_line(Str_array *arr);
void insert_string_at(Str_array *arr, size_t index, const char *str);
void remove_string_at(Str_array *arr, size_t index);
#endif