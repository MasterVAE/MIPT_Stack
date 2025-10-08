#ifndef ASSEMBLER_FUNC_H
#define ASSEMBLER_FUNC_H

#include "assembler_read.h"

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