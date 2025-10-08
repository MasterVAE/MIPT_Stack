#ifndef COMMANDS_H_
#define COMMANDS_H_

#ifdef ASS_MODE
    #include "assembler/assembler_func.h"
#endif

#ifdef SPU_MODE
    #include "processor/processor_functions.h"
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
    #define CMD(name, code, ass, spu) {name, code, ass}
#else
    #ifdef SPU_MODE
        #define CMD(name, code, ass, spu) {name, code, spu}
    #else
        #define CMD(name, code, ass, spu) {name, code}
    #endif
#endif

static const instruction COMMANDS[] = 
{
    CMD("HLT",   0b100000, ass_halt,  spu_halt),
    CMD("ADD",   0b000001, ass_add,   spu_add),
    CMD("SUB",   0b000010, ass_sub,   spu_sub),   
    CMD("MUL",   0b000011, ass_mul,   spu_mul),
    CMD("DIV",   0b000100, ass_div,   spu_div),
    CMD("SQRT",  0b000101, ass_sqrt,  spu_sqrt),
    CMD("PUSH",  0b000110, ass_push,  spu_push),
    CMD("IN",    0b001000, ass_in,    spu_in),
    CMD("OUT",   0b001001, ass_out,   spu_out), 
    CMD("POPR",  0b010000, ass_popr,  spu_popr),
    CMD("PUSHR", 0b010001, ass_pushr, spu_pushr)
};

static const int COMMANDS_COUNT = sizeof(COMMANDS) / sizeof(COMMANDS[0]);

#endif