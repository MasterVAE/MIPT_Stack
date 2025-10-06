#ifndef ASSEMBLER_FUNC_H
#define ASSEMBLER_FUNC_H


int ASS_HALT    (char** text, int list_ind, FILE* out_file, int my_ind);
int ASS_ADD     (char** text, int list_ind, FILE* out_file, int my_ind);
int ASS_SUB     (char** text, int list_ind, FILE* out_file, int my_ind);
int ASS_MUL     (char** text, int list_ind, FILE* out_file, int my_ind);
int ASS_DIV     (char** text, int list_ind, FILE* out_file, int my_ind);
int ASS_SQRT    (char** text, int list_ind, FILE* out_file, int my_ind);
int ASS_PUSH    (char** text, int list_ind, FILE* out_file, int my_ind);
int ASS_OUT     (char** text, int list_ind, FILE* out_file, int my_ind);
int ASS_IN      (char** text, int list_ind, FILE* out_file, int my_ind);
int ASS_PUSHR   (char** text, int list_ind, FILE* out_file, int my_ind);
int ASS_POPR    (char** text, int list_ind, FILE* out_file, int my_ind);

#endif