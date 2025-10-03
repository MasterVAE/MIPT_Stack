#ifndef ASSEMBLER_READ_H_
#define ASSEMBLER_READ_H_

#include <stdio.h>

typedef struct Line
{
    char* str;
    size_t len;
} Line;

void initialize_buffer(char** buffer, size_t* size, FILE* input_file);
size_t initialize_text(Line** text, char* buffer, size_t size);
size_t file_len(FILE* file);

#endif