#ifndef PROCESSOR_FUNC_H
#define PROCESSOR_FUNC_H

#include "stack.h"

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

int spu_halt    (SPU* processor);

int spu_add     (SPU* processor);
int spu_sub     (SPU* processor);
int spu_mul     (SPU* processor);
int spu_div     (SPU* processor);
int spu_sqrt    (SPU* processor);

int spu_push    (SPU* processor);
int spu_out     (SPU* processor);
int spu_in      (SPU* processor);
int spu_pushr   (SPU* processor);
int spu_popr    (SPU* processor);

int spu_jmp     (SPU* processor);
int spu_jb      (SPU* processor);
int spu_jbe     (SPU* processor);
int spu_ja      (SPU* processor);
int spu_jae     (SPU* processor);
int spu_je      (SPU* processor);
int spu_jne     (SPU* processor);

#endif