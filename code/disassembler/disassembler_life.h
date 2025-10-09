#ifndef DISASSEMLER_LIFE_H_
#define DISASSEMLER_LIFE_H_

#include <stdio.h>

#include "../language.h"

enum disassembler_errors
{
    DIS_CORRECT = 0, 
    DIS_PUSH_ARGUMENT_INVALID,
    DIS_EMPTY_PROGRAMM,
    DIS_NULL_INPUT_FILE,
    DIS_NULL_OUTPUT_FILE,
    DIS_NULL_BUFFER,
    DIS_SYNTAX_ERROR 
};

typedef struct Disassembler 
{
    char* buffer;
    size_t buffer_size;
    size_t offset;
    
} Disassembler;

int DISInit(Disassembler* dis);
void DISDestroy(Disassembler* dis);

#endif