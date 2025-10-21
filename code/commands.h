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
    ASSErr_t (*AssFunc) (Assembler*, size_t);
    #endif
    
    #ifdef SPU_MODE
    SPUState_t (*SpuFunc) (SPU*);
    #endif

    #ifdef DIS_MODE
    DisErr_t (*DisFunc) (Disassembler*, size_t, FILE*);
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
    CMD("LABEL", 0, AssLabel, NULL, DisDef, 0),
    #endif

    CMD("POPM", 0b0010010, AssPopm, SpuPopm, DisPopr, sizeof(command_type) + sizeof(value_type)),
    CMD("PUSH", 0b0000110, AssPush, SpuPush, DisPush, sizeof(command_type) + sizeof(value_type)),
    CMD("POPR", 0b0010000, AssPopr, SpuPopr, DisPopr, sizeof(command_type) + sizeof(value_type)),
    
    CMD("HLT",  0b1000000, AssDef,  SpuHalt, DisDef,  sizeof(command_type)),
    CMD("DRAW", 0b1000001, AssDef,  SpuDraw, DisDef,  sizeof(command_type)),

    CMD("ADD",  0b0000001, AssDef,  SpuAdd,  DisDef,  sizeof(command_type)),
    CMD("SUB",  0b0000010, AssDef,  SpuSub,  DisDef,  sizeof(command_type)),  
    CMD("MUL",  0b0000011, AssDef,  SpuMul,  DisDef,  sizeof(command_type)),
    CMD("DIV",  0b0000100, AssDef,  SpuDiv,  DisDef,  sizeof(command_type)),
    CMD("SQRT", 0b0000101, AssDef,  SpuSqrt, DisDef,  sizeof(command_type)),
    

    CMD("IN",   0b0001000, AssDef,  SpuIn,   DisDef,  sizeof(command_type)),
    CMD("OUT",  0b0001001, AssDef,  SpuOut,  DisDef,  sizeof(command_type)),
    
    CMD("PUSHR",0b0010001, AssPopr, SpuPushr,DisPopr, sizeof(command_type) + sizeof(value_type)),
    
    CMD("PUSHM",0b0010011, AssPopm, SpuPushm,DisPopr, sizeof(command_type) + sizeof(value_type)),
    

    CMD("JMP",  0b0100000, AssJump, SpuJmp,  DisJump, sizeof(command_type) + sizeof(value_type)),
    CMD("JB",   0b0100001, AssJump, SpuJb,   DisJump, sizeof(command_type) + sizeof(value_type)),
    CMD("JBE",  0b0100010, AssJump, SpuJbe,  DisJump, sizeof(command_type) + sizeof(value_type)),
    CMD("JA",   0b0100011, AssJump, SpuJa,   DisJump, sizeof(command_type) + sizeof(value_type)),
    CMD("JAE",  0b0100100, AssJump, SpuJae,  DisJump, sizeof(command_type) + sizeof(value_type)),
    CMD("JE",   0b0100101, AssJump, SpuJe,   DisJump, sizeof(command_type) + sizeof(value_type)),
    CMD("JNE",  0b0100110, AssJump, SpuJne,  DisJump, sizeof(command_type) + sizeof(value_type)),
    CMD("CALL", 0b0101000, AssJump, SpuCall, DisJump, sizeof(command_type) + sizeof(value_type)),
    CMD("RET",  0b0101001, AssDef,  SpuRet,  DisDef,  sizeof(command_type))
};

static const size_t COMMANDS_COUNT = sizeof(COMMANDS) / sizeof(COMMANDS[0]);

#endif // COMMANDS_H_