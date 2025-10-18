#ifndef DISASSEMLER_MANAGER_H_
#define DISASSEMLER_MANAGER_H_

#include <stdio.h>

#include "../constants.h"

typedef enum dis_err
{
    DIS_CORRECT = 0,
    DIS_NULL_DISASSEMBLER,
    DIS_NULL_BUFFER,
    DIS_NULL_FILE,
    DIS_EMPTY_PROGRAMM,
    DIS_UNKNOWN_COMMAND,
    DIS_SYNTAX_ERROR,
    DIS_ARGUMENT_INVALID
} dis_err;

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
void ErrorPrinter(dis_err error);
const char* ErrorParser(dis_err error);
int FindLabel(Disassembler* dis, int label_value);
dis_err LabelSearch(Disassembler* dis);
void InsertLabel(Disassembler* dis, FILE* output_file);

#endif // DISASSEMLER_MANAGER_H_