// file_operations.h
#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>

enum FILE_MODE {TEXT = 0, BINARY = 1};


// Function to check if file exists
bool check_if_file_exists(const char *file_name);

// Function to read a file
int read_file(const char* file_name, char *buffer, size_t buffer_size, enum FILE_MODE mode);

// Function to write to a file
int write_file(const char* file_name, const char *data, enum FILE_MODE mode);

void read_text_buffer(char *buffer, size_t start, size_t end);
void read_buffer(char *buffer, size_t start, size_t end);

// Function to trim whitespace from a string
char *trim(char *str);

// Function to trim double quotes and whitespace
char* trim_dbl_q_and_whitespace(char *str);
#endif  // FILE_OPERATIONS_H
