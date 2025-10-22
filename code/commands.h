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
    DisErr_t (*DisFunc) (Disassembler*, size_t);
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

enum CommandCodes
{
    LABEL = 0,
    POPM  = 0b0010010,
    PUSH  = 0b0000110,
    POPR  = 0b0010000,
    HLT   = 0b1000000,
    DRAW  = 0b1000001,
    ADD   = 0b0000001,
    SUB   = 0b0000010,
    MUL   = 0b0000011,
    DIV   = 0b0000100,
    SQRT  = 0b0000101,
    IN    = 0b0001000,
    OUT   = 0b0001001,
    PUSHR = 0b0010001,
    PUSHM = 0b0010011,
    JMP   = 0b0100000,
    JB    = 0b0100001,
    JBE   = 0b0100010,
    JA    = 0b0100011,
    JAE   = 0b0100100,
    JE    = 0b0100101,
    JNE   = 0b0100110,
    CALL  = 0b0101000,
    RET   = 0b0101001
};

static const instruction COMMANDS[] = 
{
    #ifdef ASS_MODE
    CMD("LABEL", LABEL, AssLabel, NULL, DisDef, 0),
    #endif

    CMD("POPM", POPM,  AssPopm, SpuPopm, DisPopr, sizeof(command_type) + sizeof(value_type)),
    CMD("PUSH", PUSH,  AssPush, SpuPush, DisPush, sizeof(command_type) + sizeof(value_type)),
    CMD("POPR", POPR,  AssPopr, SpuPopr, DisPopr, sizeof(command_type) + sizeof(value_type)),
    
    CMD("HLT",  HLT,   AssDef,  SpuHalt, DisDef,  sizeof(command_type)),
    CMD("DRAW", DRAW,  AssDef,  SpuDraw, DisDef,  sizeof(command_type)),

    CMD("ADD",  ADD,   AssDef,  SpuAdd,  DisDef,  sizeof(command_type)),
    CMD("SUB",  SUB,   AssDef,  SpuSub,  DisDef,  sizeof(command_type)),  
    CMD("MUL",  MUL,   AssDef,  SpuMul,  DisDef,  sizeof(command_type)),
    CMD("DIV",  DIV,   AssDef,  SpuDiv,  DisDef,  sizeof(command_type)),
    CMD("SQRT", SQRT,  AssDef,  SpuSqrt, DisDef,  sizeof(command_type)),
    

    CMD("IN",   IN,    AssDef,  SpuIn,   DisDef,  sizeof(command_type)),
    CMD("OUT",  OUT,   AssDef,  SpuOut,  DisDef,  sizeof(command_type)),
    
    CMD("PUSHR",PUSHR, AssPopr, SpuPushr,DisPopr, sizeof(command_type) + sizeof(value_type)),
    
    CMD("PUSHM",PUSHM, AssPopm, SpuPushm,DisPopr, sizeof(command_type) + sizeof(value_type)),
    

    CMD("JMP",  JMP,   AssJump, SpuJmp,  DisJump, sizeof(command_type) + sizeof(value_type)),
    CMD("JB",   JB,    AssJump, SpuJb,   DisJump, sizeof(command_type) + sizeof(value_type)),
    CMD("JBE",  JBE,   AssJump, SpuJbe,  DisJump, sizeof(command_type) + sizeof(value_type)),
    CMD("JA",   JA,    AssJump, SpuJa,   DisJump, sizeof(command_type) + sizeof(value_type)),
    CMD("JAE",  JAE,   AssJump, SpuJae,  DisJump, sizeof(command_type) + sizeof(value_type)),
    CMD("JE",   JE,    AssJump, SpuJe,   DisJump, sizeof(command_type) + sizeof(value_type)),
    CMD("JNE",  JNE,   AssJump, SpuJne,  DisJump, sizeof(command_type) + sizeof(value_type)),
    CMD("CALL", CALL,  AssJump, SpuCall, DisJump, sizeof(command_type) + sizeof(value_type)),
    CMD("RET",  RET,   AssDef,  SpuRet,  DisDef,  sizeof(command_type))
};

static const size_t COMMANDS_COUNT = sizeof(COMMANDS) / sizeof(COMMANDS[0]);

#endif // COMMANDS_H_