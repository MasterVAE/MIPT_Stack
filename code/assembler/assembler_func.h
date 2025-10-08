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

int ass_halt    (Line* text, int list_ind, FILE* out_file, int my_ind);
int ass_add     (Line* text, int list_ind, FILE* out_file, int my_ind);
int ass_sub     (Line* text, int list_ind, FILE* out_file, int my_ind);
int ass_mul     (Line* text, int list_ind, FILE* out_file, int my_ind);
int ass_div     (Line* text, int list_ind, FILE* out_file, int my_ind);
int ass_sqrt    (Line* text, int list_ind, FILE* out_file, int my_ind);
int ass_push    (Line* text, int list_ind, FILE* out_file, int my_ind);
int ass_out     (Line* text, int list_ind, FILE* out_file, int my_ind);
int ass_in      (Line* text, int list_ind, FILE* out_file, int my_ind);
int ass_pushr   (Line* text, int list_ind, FILE* out_file, int my_ind);
int ass_popr    (Line* text, int list_ind, FILE* out_file, int my_ind);

#endif