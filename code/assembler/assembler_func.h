#ifndef ASSEMBLER_FUNC_H
#define ASSEMBLER_FUNC_H

#include "assembler_read.h"

int bytecode_comm   (Assembler* asm_ptr, int command);
int bytecode_value  (Assembler* asm_ptr, int command);

ass_err ass_def    (Assembler* asm_ptr, size_t my_ind);

ass_err ass_push    (Assembler* asm_ptr, size_t my_ind);
ass_err ass_pushr   (Assembler* asm_ptr, size_t my_ind);
ass_err ass_popr    (Assembler* asm_ptr, size_t my_ind);

ass_err ass_jump    (Assembler* asm_ptr, size_t my_ind);

ass_err ass_label   (Assembler* asm_ptr, size_t my_ind);

#endif