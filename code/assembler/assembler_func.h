#ifndef ASSEMBLER_FUNC_H
#define ASSEMBLER_FUNC_H

#include "assembler_read.h"

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

int ass_halt    (Line* text, size_t list_ind, Assembler* asm_ptr, size_t my_ind);

int ass_add     (Line* text, size_t list_ind, Assembler* asm_ptr, size_t my_ind);
int ass_sub     (Line* text, size_t list_ind, Assembler* asm_ptr, size_t my_ind);
int ass_mul     (Line* text, size_t list_ind, Assembler* asm_ptr, size_t my_ind);
int ass_div     (Line* text, size_t list_ind, Assembler* asm_ptr, size_t my_ind);
int ass_sqrt    (Line* text, size_t list_ind, Assembler* asm_ptr, size_t my_ind);

int ass_push    (Line* text, size_t list_ind, Assembler* asm_ptr, size_t my_ind);
int ass_out     (Line* text, size_t list_ind, Assembler* asm_ptr, size_t my_ind);
int ass_in      (Line* text, size_t list_ind, Assembler* asm_ptr, size_t my_ind);
int ass_pushr   (Line* text, size_t list_ind, Assembler* asm_ptr, size_t my_ind);
int ass_popr    (Line* text, size_t list_ind, Assembler* asm_ptr, size_t my_ind);

int ass_jump    (Line* text, size_t list_ind, Assembler* asm_ptr, size_t my_ind);

int ass_label   (Line* text, size_t list_ind, Assembler* asm_ptr, size_t my_ind);

#endif