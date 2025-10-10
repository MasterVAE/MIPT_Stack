#ifndef DISASSEMLER_LIFE_H_
#define DISASSEMLER_LIFE_H_

#include <stdio.h>

#include "../language.h"

enum disassembler_errors
{
    DIS_CORRECT = 0,
    DIS_NULL_DISASSEMBLER,
    DIS_NULL_BUFFER,
    DIS_NULL_FILE,
    DIS_EMPTY_PROGRAMM,
    DIS_UNKNOWN_COMMAND,
    DIS_SYNTAX_ERROR,
    DIS_ARGUMENT_INVALID
};

const size_t MAX_LABELS = 10;

typedef struct Disassembler 
{
    char* buffer;
    size_t buffer_size;
    size_t offset;
    int labels[MAX_LABELS];
    
} Disassembler;

int DISInit(Disassembler* dis);
void DISDestroy(Disassembler* dis);
void error_printer(int error);
const char* error_parser(int error);

#endif