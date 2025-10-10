#ifndef PROCESSOR_FUNC_H
#define PROCESSOR_FUNC_H

#include "stack.h"

typedef enum spu_err
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

spu_err spu_halt    (SPU* processor);

spu_err spu_add     (SPU* processor);
spu_err spu_sub     (SPU* processor);
spu_err spu_mul     (SPU* processor);
spu_err spu_div     (SPU* processor);
spu_err spu_sqrt    (SPU* processor);

spu_err spu_push    (SPU* processor);
spu_err spu_out     (SPU* processor);
spu_err spu_in      (SPU* processor);
spu_err spu_pushr   (SPU* processor);
spu_err spu_popr    (SPU* processor);

spu_err spu_jmp     (SPU* processor);
spu_err spu_jb      (SPU* processor);
spu_err spu_jbe     (SPU* processor);
spu_err spu_ja      (SPU* processor);
spu_err spu_jae     (SPU* processor);
spu_err spu_je      (SPU* processor);
spu_err spu_jne     (SPU* processor);
spu_err spu_call    (SPU* processor);
spu_err spu_ret     (SPU* processor);

#endif