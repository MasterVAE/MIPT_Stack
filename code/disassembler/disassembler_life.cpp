#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "disassembler_life.h"

int DISInit(Disassembler* dis)
{
    if(dis == NULL) return 1;
    dis->buffer_size = 0;
    dis->offset = 0;
    for(size_t i = 0; i < MAX_LABELS; dis->labels[i++] = -1);
    return 0;
}

void DISDestroy(Disassembler* dis)
{
    free(dis->buffer);
    memset(dis, 0, sizeof(Disassembler));
}

void error_printer(dis_err error)
{
    fprintf(stderr, "%s\n", error_parser(error));
}

const char* error_parser(dis_err error)
{
    switch (error)
    {
        case DIS_CORRECT:           return "Correct";
        case DIS_NULL_DISASSEMBLER: return "Disassembler NULL";
        case DIS_NULL_BUFFER:       return "Buffer NULL";
        case DIS_NULL_FILE:         return "Error opening file";
        case DIS_EMPTY_PROGRAMM:    return "Empty programm";
        case DIS_UNKNOWN_COMMAND:   return "Unknown command";
        case DIS_SYNTAX_ERROR:      return "Syntax error";
        case DIS_ARGUMENT_INVALID:  return "Argument invalid";
        default:                    return "Unknown error";
    }
}

int find_label(Disassembler* dis, int label_value)
{
    for(size_t i = 0; i < MAX_LABELS; i++)
    {
        if(dis->labels[i] == label_value) return (int)i;
    }

    return -1;
}