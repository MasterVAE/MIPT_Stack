#ifndef COMMANDS_H_
#define COMMANDS_H_

#include "processor_functions.h"
#include "assembler_func.h"

typedef struct instruction
{
    const char* name;
    const int num;
    int (*ass_func) (char** text, int line_ind, FILE* out_file, int my_ind);
    int (*spu_func) (SPU* processor);
} instruction;

static const instruction COMMANDS[] = 
{
    {"HLT",   0b100000, ASS_HALT,  SPU_HALT},
    {"ADD",   0b000001, ASS_ADD,   SPU_ADD},
    {"SUB",   0b000010, ASS_SUB,   SPU_SUB},   
    {"MUL",   0b000011, ASS_MUL,   SPU_MUL},
    {"DIV",   0b000100, ASS_DIV,   SPU_DIV},
    {"SQRT",  0b000101, ASS_SQRT,  SPU_SQRT},
    {"PUSH",  0b000110, ASS_PUSH,  SPU_PUSH},
    {"IN",    0b001000, ASS_IN,    SPU_IN},
    {"OUT",   0b001001, ASS_OUT,   SPU_OUT}, 
    {"POPR",  0b010000, ASS_POPR,  SPU_POPR},
    {"PUSHR", 0b010001, ASS_PUSHR, SPU_PUSHR}
};

static const int COMMANDS_COUNT = sizeof(COMMANDS) / sizeof(COMMANDS[0]);

#endif