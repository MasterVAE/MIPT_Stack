#ifndef COMMANDS_H_
#define COMMANDS_H_

#ifdef ASS_MODE
    #include "assembler_func.h"
#endif

#ifdef SPU_MODE
    #include "processor_functions.h"
#endif

typedef struct instruction
{
    const char* name;
    const int num;
    
    #ifdef ASS_MODE
    int (*ass_func) (Line* text, int line_ind, FILE* out_file, int my_ind);
    #endif
    
    #ifdef SPU_MODE
    int (*spu_func) (SPU* processor);
    #endif
} instruction;

#ifdef ASS_MODE
    #ifdef SPU_MODE
        #define CMD(name, code, ass, spu) {name, code, ass, spu}
    #else
        #define CMD(name, code, ass, spu) {name, code, ass}
    #endif
#else
    #ifdef SPU_MODE
        #define CMD(name, code, ass, spu) {name, code, spu}
    #else
        #define CMD(name, code, ass, spu) {name, code}
    #endif
#endif

static const instruction COMMANDS[] = 
{
    CMD("HLT",   0b100000, ASS_HALT,  SPU_HALT),
    CMD("ADD",   0b000001, ASS_ADD,   SPU_ADD),
    CMD("SUB",   0b000010, ASS_SUB,   SPU_SUB),   
    CMD("MUL",   0b000011, ASS_MUL,   SPU_MUL),
    CMD("DIV",   0b000100, ASS_DIV,   SPU_DIV),
    CMD("SQRT",  0b000101, ASS_SQRT,  SPU_SQRT),
    CMD("PUSH",  0b000110, ASS_PUSH,  SPU_PUSH),
    CMD("IN",    0b001000, ASS_IN,    SPU_IN),
    CMD("OUT",   0b001001, ASS_OUT,   SPU_OUT), 
    CMD("POPR",  0b010000, ASS_POPR,  SPU_POPR),
    CMD("PUSHR", 0b010001, ASS_PUSHR, SPU_PUSHR)
};

static const int COMMANDS_COUNT = sizeof(COMMANDS) / sizeof(COMMANDS[0]);

#endif