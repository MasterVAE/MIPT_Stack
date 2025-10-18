#ifndef COMMANDS_H_
#define COMMANDS_H_

#ifdef ASS_MODE
    #include "assembler/assembler_func.h"
    #include "assembler/assembler_manager.h"
#endif

#ifdef SPU_MODE
    #include "processor/processor_functions.h"
    #include "processor/processor_manager.h"
#endif

#ifdef DIS_MODE
    #include "disassembler/disassembler_func.h"
#endif

typedef struct instruction
{
    const char* name;
    const int num;
    
    #ifdef ASS_MODE
    ASSErr_t (*ass_func) (Assembler*, size_t);
    #endif
    
    #ifdef SPU_MODE
    SPUErr_t (*spu_func) (SPU*);
    #endif

    #ifdef DIS_MODE
    dis_err (*dis_func) (Disassembler*, size_t, FILE*);
    const size_t byte_size;
    #endif
} instruction;

#ifdef ASS_MODE
    #define CMD(name, code, ass, spu, dis, size) {name, code, ass}
#else
    #ifdef SPU_MODE
        #define CMD(name, code, ass, spu, dis, size) {name, code, spu}
    #else
        #ifdef DIS_MODE
            #define CMD(name, code, ass, spu, dis, size) {name, code, dis, size}
        #else
            #define CMD(name, code, ass, spu, dis, size) {name, code}
        #endif
    #endif
#endif

static const instruction COMMANDS[] = 
{
    #ifdef ASS_MODE
    CMD("LABEL", 0, AssLabel, NULL, dis_def, 0),
    #endif

    CMD("POPM", 0b0010010, ass_popm, spu_popm, dis_popr, sizeof(command_type) + sizeof(value_type)),
    CMD("PUSH", 0b0000110, ass_push, spu_push, dis_push, sizeof(command_type) + sizeof(value_type)),
    CMD("POPR", 0b0010000, ass_popr, spu_popr, dis_popr, sizeof(command_type) + sizeof(value_type)),
    
    CMD("HLT",  0b1000000, ass_def, spu_halt, dis_def,  sizeof(command_type)),
    CMD("DRAW", 0b1000001, ass_def, spu_draw, dis_def,  sizeof(command_type)),

    CMD("ADD",  0b0000001, ass_def,  spu_add,  dis_def,  sizeof(command_type)),
    CMD("SUB",  0b0000010, ass_def,  spu_sub,  dis_def,  sizeof(command_type)),  
    CMD("MUL",  0b0000011, ass_def,  spu_mul,  dis_def,  sizeof(command_type)),
    CMD("DIV",  0b0000100, ass_def,  spu_div,  dis_def,  sizeof(command_type)),
    CMD("SQRT", 0b0000101, ass_def,  spu_sqrt, dis_def,  sizeof(command_type)),
    

    CMD("IN",   0b0001000, ass_def,  spu_in,   dis_def,  sizeof(command_type)),
    CMD("OUT",  0b0001001, ass_def,  spu_out,  dis_def,  sizeof(command_type)),
    
    CMD("PUSHR",0b0010001, ass_popr,spu_pushr, dis_popr, sizeof(command_type) + sizeof(value_type)),
    
    CMD("PUSHM",0b0010011, ass_popm,spu_pushm, dis_popr, sizeof(command_type) + sizeof(value_type)),
    

    CMD("JMP",  0b0100000, ass_jump, spu_jmp,  dis_jump, sizeof(command_type) + sizeof(value_type)),
    CMD("JB",   0b0100001, ass_jump, spu_jb,   dis_jump, sizeof(command_type) + sizeof(value_type)),
    CMD("JBE",  0b0100010, ass_jump, spu_jbe,  dis_jump, sizeof(command_type) + sizeof(value_type)),
    CMD("JA",   0b0100011, ass_jump, spu_ja,   dis_jump, sizeof(command_type) + sizeof(value_type)),
    CMD("JAE",  0b0100100, ass_jump, spu_jae,  dis_jump, sizeof(command_type) + sizeof(value_type)),
    CMD("JE",   0b0100101, ass_jump, spu_je,   dis_jump, sizeof(command_type) + sizeof(value_type)),
    CMD("JNE",  0b0100110, ass_jump, spu_jne,  dis_jump, sizeof(command_type) + sizeof(value_type)),
    CMD("CALL", 0b0101000, ass_jump, spu_call, dis_jump, sizeof(command_type) + sizeof(value_type)),
    CMD("RET",  0b0101001, ass_def,  spu_ret,  dis_def,  sizeof(command_type))
};

static const size_t COMMANDS_COUNT = sizeof(COMMANDS) / sizeof(COMMANDS[0]);

#endif // COMMANDS_H_