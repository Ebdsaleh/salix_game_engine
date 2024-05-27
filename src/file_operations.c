// file_operations.c

#include "file_operations.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>




// Function to check if file exists
bool check_if_file_exists(const char *file_name) {
    struct stat buffer;
    if (stat(file_name, &buffer) == 0) {
        // Check if it's a regular file
        if (S_ISREG(buffer.st_mode)) {
            return true; // File exists.
        }
    }
    return false;
}



// Function to read a file
int read_file(const char *file_name, char *buffer, size_t buffer_size, enum FILE_MODE mode) {
    const char *mode_flag; 

    if (mode == TEXT) {
       mode_flag = "r";
    }

    else if (mode == BINARY) {
        mode_flag = "rb";   
    }

    else {
        fprintf(stderr, "ERROR: Invalid FILE_MODE set for 'mode'.\n");
    }
    

    FILE *file = fopen(file_name, mode_flag);

    if (file == NULL) {
        fprintf(stderr, "ERROR: Failed to open file for reading: '%s'.\n", file_name);
        return -1;
    }


    size_t bytes_read = fread(buffer, 1, buffer_size, file);
    fclose(file);

    return bytes_read;
}



// Function to write to a file
int write_file(const char *file_name, const char *data, enum FILE_MODE mode) {
    const char *mode_flag;

    if (mode == TEXT) {
        mode_flag = "w";
    }

    else if (mode == BINARY) {
        mode_flag = "wb";
    }

    else {
        fprintf(stderr, "ERROR: Invalid FILE_MODE set for 'mode'\n");
    }


    FILE *file = fopen(file_name, mode_flag);

    if (file == NULL) {
        fprintf(stderr, "ERROR: Failed to open file for writing: '%s'\n", file_name);
        return -1;
    }


    fputs(data, file);
    fclose(file);

    return 0;
}


void read_text_buffer(char *buffer, size_t start, size_t end) {
    if (buffer == NULL) {
        fprintf(stderr, "ERROR: Buffer is NULL\n");
        return;
    }

    if (start >= end) {
        printf("Invalid start and end positions.\n");
        return;
    }
    
    printf("Buffer contents from position %zu to %zu:\n", start, end);

    // Ensure end doesn't exceed the buffer size
    end = (end > strlen(buffer)) ? strlen(buffer) : end;

    for (size_t i = start; i < end; ++i) {
        printf("%c", buffer[i]);
    }
    printf("\n");
}


void read_buffer(char *buffer, size_t start, size_t end) {
    if (buffer == NULL) {
        fprintf(stderr, "ERROR: Buffer is NULL\n");
        return;
    }

    if (start >= end) {
        printf("Invalid start and end positions.\n");
        return;
    }
    
    printf("Buffer contents from position %zu to %zu:\n", start, end);

    // this may need to be abandoned 
}


// Function to trim whitepace from a string
char* trim(char *str) {
    // Trim leading whitespace
    while (isspace(*str)) {
        str++;
    }

    // Trim trailing whitespace
    char *end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) {
        end--;
    }
    // Null-terminate the trimmed string
    *(end + 1) = '\0';

    return str;
}



// This version will remove double quotes and whitespace from strings
char* trim_dbl_q_and_whitespace(char *str) {
    // Trim leading whitespace
    while (isspace(*str)) {
        str++;
    }

    // Trim trailing whitespace
    char *end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) {
        end--;
    }
    
    // Preserve double quotes for strings
    if (*str == '"' && *end == '"') {
        str++;
        *end = '\0';
    }

    // Null-terminate the trimmed string
    *(end + 1) = '\0';

    return str;
}
