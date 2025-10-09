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
bool correct_label(int label);

const size_t buffer_start_size = 5;
const size_t buffer_size_mult = 2;

const size_t MAX_JUMPS = 10;
const size_t MAX_LABELS = 10;


typedef struct jump_memory
{
    int label;
    size_t offcet;
} jump_memory;

typedef struct Assembler 
{
    char* buffer;
    size_t buffer_size;
    size_t offset;

    size_t lines_count;
    size_t line_offset;

    Line* text;

    int labels[MAX_LABELS];
    jump_memory jumps[MAX_JUMPS];
    size_t current_jump_memory = 0;

} Assembler;

int ASSInit(Assembler* ass);
void ASSDestroy(Assembler* ass);
int ASSPostCompile(Assembler* ass);

#endif