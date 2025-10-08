#ifndef ASSEMBLER_READ_H_
#define ASSEMBLER_READ_H_

#include <stdio.h>

#include "../language.h"

typedef struct Line
{
    char* line;

    char* args[ARG_LIMIT];

    size_t arg_count;
    
}   Line;


void initialize_buffer(char** buffer, size_t* size, FILE* input_file);
size_t initialize_text(Line** text, char* buffer, size_t size);
size_t file_len(FILE* file);
size_t parse(char* source, char* dist, size_t max, size_t* read = NULL);


#endif