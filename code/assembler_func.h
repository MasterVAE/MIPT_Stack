#ifndef ASSEMBLER_FUNC_H
#define ASSEMBLER_FUNC_H

#include "assembler_read.h"

int ASS_HALT    (Line* text, int list_ind, FILE* out_file, int my_ind);
int ASS_ADD     (Line* text, int list_ind, FILE* out_file, int my_ind);
int ASS_SUB     (Line* text, int list_ind, FILE* out_file, int my_ind);
int ASS_MUL     (Line* text, int list_ind, FILE* out_file, int my_ind);
int ASS_DIV     (Line* text, int list_ind, FILE* out_file, int my_ind);
int ASS_SQRT    (Line* text, int list_ind, FILE* out_file, int my_ind);
int ASS_PUSH    (Line* text, int list_ind, FILE* out_file, int my_ind);
int ASS_OUT     (Line* text, int list_ind, FILE* out_file, int my_ind);
int ASS_IN      (Line* text, int list_ind, FILE* out_file, int my_ind);
int ASS_PUSHR   (Line* text, int list_ind, FILE* out_file, int my_ind);
int ASS_POPR    (Line* text, int list_ind, FILE* out_file, int my_ind);

#endif