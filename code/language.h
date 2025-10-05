#ifndef LANGUAGE_H_
#define LANGUAGE_H_

#define RED "\033[31m"
#define CLEAN "\033[0m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"

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
    ASS_ARGUMENT_INVALID,
    ASS_EMPTY_PROGRAMM,
    ASS_NULL_INPUT_FILE,
    ASS_NULL_TEXT_POINTER,
    ASS_NULL_OUTPUT_FILE,
    ASS_SYNTAX_ERROR,
    ASS_TO_MUCH_ARGUMENT
};

const int command_size = 6;
const int value_size = 32;

const int register_size = 8;

typedef int stack_type;



typedef enum processor_errors
{
    SPU_CORRECT = 0,
    SPU_HALT = 1 << 1,
    SPU_DIVISION_BY_ZERO = 1 << 2,
    SPU_INVALID_COMMAND = 1 << 3,
    SPU_STACK_ERROR = 1 << 4,
    SPU_PROCESSOR_NULL = 1 << 5,
    SPU_REG_NULL = 1 << 6,
    SPU_BUFFER_NULL = 1 << 7
} spu_err;


#endif