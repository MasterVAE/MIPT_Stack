#ifndef ASSEMBLER_FUNC_H
#define ASSEMBLER_FUNC_H

#include "assembler_life.h"

int bytecode_comm   (Assembler* asm_ptr, int command);
int bytecode_value  (Assembler* asm_ptr, int command);

ASSErr_t ass_def    (Assembler* asm_ptr, size_t my_ind);

ASSErr_t ass_push    (Assembler* asm_ptr, size_t my_ind);
ASSErr_t ass_popr    (Assembler* asm_ptr, size_t my_ind);
ASSErr_t ass_popm    (Assembler* asm_ptr, size_t my_ind);

ASSErr_t ass_jump    (Assembler* asm_ptr, size_t my_ind);

ASSErr_t ass_label   (Assembler* asm_ptr, size_t my_ind);

#endif //ASSEMBLER_FUNC_H