#ifndef ASSEMBLER_READ_H_
#define ASSEMBLER_READ_H_

#include <stdio.h>

#include "../language.h"

typedef enum ASSErr
{
    ASS_CORRECT, 
    ASS_ASSEMBLER_NULL,
    ASS_NULL_TEXT_POINTER,
    ASS_NULL_BUFFER_POINTER,
    ASS_EMPTY_PROGRAMM,
    ASS_UNKNOWN_COMMAND,
    ASS_ARGUMENT_INVALID,
    ASS_NULL_FILE,
    ASS_SYNTAX_ERROR,
    ASS_USED_LABEL,
    ASS_LABEL_INVALID,
    ASS_TOO_MANY_JUMPS
} ASSErr_t;

typedef struct Line
{
    char* line;
    char* args[ARG_LIMIT];
    size_t arg_count;
} Line;

size_t initialize_text(Line** text, char* buffer, size_t size);
size_t parse(char* source, char* dist, size_t max, size_t* read = NULL);
void error_printer(ASSErr_t error);
const char* error_parser(ASSErr_t error);

const size_t buffer_start_size = 5;
const size_t buffer_size_mult = 2;

const size_t MAX_JUMPS = 40;
const size_t MAX_LABELS = 40;

const size_t MAX_COMMAND_LENGHT = 20;

typedef struct jump_memory
{
    char label[MAX_COMMAND_LENGHT];
    size_t offcet;
} jump_memory;

typedef struct label
{
    char name[MAX_COMMAND_LENGHT];;
    int value;
} label;

typedef struct Assembler 
{
    char* buffer;
    size_t buffer_size;
    size_t offset;

    size_t lines_count;
    size_t line_offset;

    Line* text;

    label labels[MAX_LABELS];
    jump_memory jumps[MAX_JUMPS];
    size_t current_jump_memory;

} Assembler;

ASSErr_t ASSInit(Assembler* ass);
void ASSDestroy(Assembler* ass);
ASSErr_t ASSPostCompile(Assembler* ass);

label* get_label(Assembler* ass, char* label_name);
void add_label(Assembler* ass, char* name, int value); 

#endif //ASSEMBLER_READ_H_