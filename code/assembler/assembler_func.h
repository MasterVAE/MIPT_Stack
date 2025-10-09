#ifndef ASSEMBLER_FUNC_H
#define ASSEMBLER_FUNC_H

#include "assembler_read.h"

enum assembler_errors
{
    ASS_CORRECT, 
    ASS_ASSEMBLER_NULL,
    ASS_NULL_TEXT_POINTER,
    ASS_NULL_BUFFER_POINTER,
    ASS_EMPTY_PROGRAMM,
    ASS_UNKNOWN_COMMAND,
    ASS_ARGUMENT_INVALID,
    ASS_NULL_INPUT_FILE,
    ASS_NULL_OUTPUT_FILE,
    ASS_SYNTAX_ERROR,
    ASS_USED_LABEL,
    ASS_LABEL_INVALID,
    ASS_TOO_MANY_JUMPS
};

int bytecode_comm(Assembler* asm_ptr, int command);
int bytecode_value(Assembler* asm_ptr, int command);

int ass_halt    (Assembler* asm_ptr, size_t my_ind);

int ass_add     (Assembler* asm_ptr, size_t my_ind);
int ass_sub     (Assembler* asm_ptr, size_t my_ind);
int ass_mul     (Assembler* asm_ptr, size_t my_ind);
int ass_div     (Assembler* asm_ptr, size_t my_ind);
int ass_sqrt    (Assembler* asm_ptr, size_t my_ind);

int ass_push    (Assembler* asm_ptr, size_t my_ind);
int ass_out     (Assembler* asm_ptr, size_t my_ind);
int ass_in      (Assembler* asm_ptr, size_t my_ind);
int ass_pushr   (Assembler* asm_ptr, size_t my_ind);
int ass_popr    (Assembler* asm_ptr, size_t my_ind);

int ass_jump    (Assembler* asm_ptr, size_t my_ind);

int ass_label   (Assembler* asm_ptr, size_t my_ind);

#endif