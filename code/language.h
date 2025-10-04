#ifndef LANGUAGE_H_
#define LANGUAGE_H_


enum funcs
{
    HLT,
    PUSH,
    ADD,
    SUB,
    MUL,
    DIV,
    SQRT,
    OUT,
    IN,
    PUSHR,
    POPR
};

enum assembler_errors
{
    ASS_CORRECT = 0, 
    ASS_HLT_NOT_FOUND,
    ASS_PUSH_ARGUMENT_INVALID,
    ASS_EMPTY_PROGRAMM,
    ASS_NULL_INPUT_FILE,
    ASS_NULL_TEXT_POINTER,
    ASS_NULL_OUTPUT_FILE,
    ASS_SYNTAX_ERROR,
    ASS_TO_MUCH_ARGUMENT,
};

const int command_size = 6;
const int value_size = 32;

const int regiser_size = 16;

typedef int stack_type;



typedef enum processor_errors
{
    SPU_CORRECT = 0,
    SPU_HALT,
    SPU_DIVISION_BY_ZERO,
    SPU_INVALID_COMMAND,
    SPU_STACK_ERROR
} spu_err;


#endif