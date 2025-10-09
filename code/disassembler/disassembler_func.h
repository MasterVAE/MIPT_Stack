#ifndef DISASSEMBLER_FUNC_H
#define DISASSEMBLER_FUNC_H

#include "disassembler_life.h"

int dis_def(Disassembler* dis, size_t my_ind, FILE* output_file);
int dis_push(Disassembler* dis, size_t my_ind, FILE* output_file);
int dis_popr(Disassembler* dis, size_t my_ind, FILE* output_file);
int dis_jump(Disassembler* dis, size_t my_ind, FILE* output_file);

const char* const regs[] = 
{
    "SR1",
    "SR2",
    "SR3",
    "SR4",
    "SR5",
    "SR6",
    "SR7",
    "SR8"
};

int debytecode(char* code, size_t size);

#endif