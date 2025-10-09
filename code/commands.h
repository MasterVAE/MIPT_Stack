#ifndef COMMANDS_H_
#define COMMANDS_H_

#ifdef ASS_MODE
    #include "assembler/assembler_func.h"
    #include "assembler/assembler_read.h"
#endif

#ifdef SPU_MODE
    #include "processor/processor_functions.h"
#endif

#ifdef DIS_MODE
    #include "disassembler/disassembler_func.h"
#endif

typedef struct instruction
{
    const char* name;
    const int num;
    
    #ifdef ASS_MODE
    int (*ass_func) (Assembler*, size_t);
    #endif
    
    #ifdef SPU_MODE
    int (*spu_func) (SPU*);
    #endif

    #ifdef DIS_MODE
    int (*dis_func) (Disassembler*, size_t, FILE*);
    #endif
} instruction;

#ifdef ASS_MODE
    #define CMD(name, code, ass, spu, dis) {name, code, ass}
#else
    #ifdef SPU_MODE
        #define CMD(name, code, ass, spu, dis) {name, code, spu}
    #else
        #ifdef DIS_MODE
            #define CMD(name, code, ass, spu, dis) {name, code, dis}
        #else
            #define CMD(name, code, ass, spu, dis) {name, code}
        #endif
    #endif
#endif

static const instruction COMMANDS[] = 
{
    #ifdef ASS_MODE
    CMD("LABEL", 0, ass_label, NULL, dis_def),
    #endif

    CMD("HLT",  0b1000000, ass_halt,  spu_halt,  dis_def),

    CMD("ADD",  0b0000001, ass_add,   spu_add,   dis_def),
    CMD("SUB",  0b0000010, ass_sub,   spu_sub,   dis_def),   
    CMD("MUL",  0b0000011, ass_mul,   spu_mul,   dis_def),
    CMD("DIV",  0b0000100, ass_div,   spu_div,   dis_def),
    CMD("SQRT", 0b0000101, ass_sqrt,  spu_sqrt,  dis_def),
    
    CMD("PUSH", 0b0000110, ass_push,  spu_push,  dis_push),
    CMD("IN",   0b0001000, ass_in,    spu_in,    dis_def),
    CMD("OUT",  0b0001001, ass_out,   spu_out,   dis_def), 
    CMD("POPR", 0b0010000, ass_popr,  spu_popr,  dis_popr),
    CMD("PUSHR",0b0010001, ass_pushr, spu_pushr, dis_popr), 
    
    CMD("JMP",  0b0100000, ass_jump,  spu_jmp,   dis_jump),
    CMD("JB",   0b0100001, ass_jump,  spu_jb,    dis_jump),
    CMD("JBE",  0b0100010, ass_jump,  spu_jbe,   dis_jump),
    CMD("JA",   0b0100011, ass_jump,  spu_ja,    dis_jump),
    CMD("JAE",  0b0100100, ass_jump,  spu_jae,   dis_jump),
    CMD("JE",   0b0100101, ass_jump,  spu_je,    dis_jump),
    CMD("JNE",  0b0100110, ass_jump,  spu_jne,   dis_jump)
};

static const size_t COMMANDS_COUNT = sizeof(COMMANDS) / sizeof(COMMANDS[0]);

#endif