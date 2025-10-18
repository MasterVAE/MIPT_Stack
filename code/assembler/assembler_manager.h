#ifndef ASSEMBLER_LIFE_H_
#define ASSEMBLER_LIFE_H_

#include <stdio.h>

#include "../constants.h"

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
    char* arg;
    bool is_have_arg;
} Line;

size_t InitializeText(Line** text, char* buffer, size_t size);
size_t Parse(const char* source, char* dest, size_t max, size_t* read = NULL);
void ErrorPrinter(ASSErr_t error);
const char* ErrorParser(ASSErr_t error);

const size_t BUFFER_START_SIZE = 5;
const size_t BUFFER_SIZE_MULT = 2;

const size_t MAX_JUMPS = 40;
const size_t MAX_LABELS = 40;

const size_t MAX_COMMAND_LENGHT = 20;

typedef struct forward_jump
{
    char label[MAX_COMMAND_LENGHT];
    size_t command_pointer;
} forward_jump;

typedef struct label
{
    char name[MAX_COMMAND_LENGHT];
    int address;
} label;

typedef struct label_table
{
    label labels[MAX_LABELS];
    forward_jump forward_jumps[MAX_JUMPS];
    size_t current_forward_jump;
} label_table;

typedef struct Assembler 
{
    char* bin_buffer;
    size_t buffer_size;
    size_t offset;

    size_t line_offset;
    
    Line* text;
    size_t lines_count;

    label_table lbl_table;
} Assembler;

const size_t asmsds = sizeof(Assembler);

ASSErr_t ASSInit(Assembler* ass);
void ASSDestroy(Assembler* ass);
ASSErr_t ASSPostCompile(Assembler* ass);

label* GetLabel(Assembler* ass, char* label_name);
void AddLabel(Assembler* ass, char* name, int value); 

#endif // ASSEMBLER_LIFE_H_