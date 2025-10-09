#ifndef COMMANDS_H_
#define COMMANDS_H_

#ifdef ASS_MODE
    #include "assembler/assembler_func.h"
    #include "assembler/assembler_read.h"
#endif

#ifdef SPU_MODE
    #include "processor/processor_functions.h"
#endif

typedef struct instruction
{
    const char* name;
    const int num;
    
    #ifdef ASS_MODE
    int (*ass_func) (Line*, size_t, Assembler*, size_t);
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
    #ifdef ASS_MODE
    CMD("LABEL", 0, ass_label, NULL),
    #endif

    CMD("HLT",  0b1000000, ass_halt,  spu_halt),

    CMD("ADD",  0b0000001, ass_add,   spu_add),
    CMD("SUB",  0b0000010, ass_sub,   spu_sub),   
    CMD("MUL",  0b0000011, ass_mul,   spu_mul),
    CMD("DIV",  0b0000100, ass_div,   spu_div),
    CMD("SQRT", 0b0000101, ass_sqrt,  spu_sqrt),
    
    CMD("PUSH", 0b0000110, ass_push,  spu_push),
    CMD("IN",   0b0001000, ass_in,    spu_in),
    CMD("OUT",  0b0001001, ass_out,   spu_out), 
    CMD("POPR", 0b0010000, ass_popr,  spu_popr),
    CMD("PUSHR",0b0010001, ass_pushr, spu_pushr), 
    
    CMD("JMP",  0b0100000, ass_jump,  spu_jmp),
    CMD("JB",   0b0100001, ass_jump,  spu_jb),
    CMD("JBE",  0b0100010, ass_jump,  spu_jbe),
    CMD("JA",   0b0100011, ass_jump,  spu_ja),
    CMD("JAE",  0b0100100, ass_jump,  spu_jae),
    CMD("JE",   0b0100101, ass_jump,  spu_je),
    CMD("JNE",  0b0100110, ass_jump,  spu_jne)
};

static const size_t COMMANDS_COUNT = sizeof(COMMANDS) / sizeof(COMMANDS[0]);

#endif