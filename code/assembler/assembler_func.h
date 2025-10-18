#ifndef ASSEMBLER_FUNC_H
#define ASSEMBLER_FUNC_H

#include "assembler_manager.h"

int BytecodeComm   (Assembler* asm_ptr, int command);
int BytecodeValue  (Assembler* asm_ptr, int command);

ASSErr_t AssDef    (Assembler* asm_ptr, size_t my_ind);

ASSErr_t AssPush    (Assembler* asm_ptr, size_t my_ind);
ASSErr_t AssPopr    (Assembler* asm_ptr, size_t my_ind);
ASSErr_t AssPopm    (Assembler* asm_ptr, size_t my_ind);

ASSErr_t AssJump    (Assembler* asm_ptr, size_t my_ind);

ASSErr_t AssLabel   (Assembler* asm_ptr, size_t my_ind);

#endif // ASSEMBLER_FUNC_H