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

size_t initialize_text(Line** text, char* buffer, size_t size);
size_t parse(char* source, char* dist, size_t max, size_t* read = NULL);

typedef struct Assembler 
{
    char* buffer;
    size_t buffer_size;
    size_t offset;

    size_t lines_count;
    size_t line_offset;

    Line* text;

    int labels[10];

} Assembler;

int ASSInit(Assembler* ass);
void ASSDestroy(Assembler* ass);

const size_t buffer_start_size = 5;
const size_t buffer_size_mult = 2;

#endif
