#ifndef LANGUAGE_H_
#define LANGUAGE_H_

#define RED     "\033[91m"
#define CLEAN   "\033[0m"
#define GREEN   "\033[92m"
#define BLUE    "\033[94m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[96m"
#define PINK    "\033[95m"

enum assembler_errors
{
    ASS_CORRECT, 
    ASS_HLT_NOT_FOUND,
    ASS_ARGUMENT_INVALID,
    ASS_EMPTY_PROGRAMM,
    ASS_NULL_INPUT_FILE,
    ASS_NULL_TEXT_POINTER,
    ASS_NULL_OUTPUT_FILE,
    ASS_SYNTAX_ERROR,
    ASS_TO_MUCH_ARGUMENT
};

#define COMMAND_TYPE __uint8_t
#define VALUE_TYPE __uint32_t

const size_t REG_SIZE = 8;
const size_t ARG_LIMIT = 10;

typedef int stack_type;

typedef enum processor_errors
{
    SPU_CORRECT = 0,
    SPU_HALT_STATE = 1 << 1,
    SPU_DIVISION_BY_ZERO = 1 << 2,
    SPU_INVALID_COMMAND = 1 << 3,
    SPU_STACK_ERROR = 1 << 4,
    SPU_PROCESSOR_NULL = 1 << 5,
    SPU_REG_NULL = 1 << 6,
    SPU_BUFFER_NULL = 1 << 7,
    SPU_INVALID_REGISTER = 1 << 8
} spu_err;


#endif