#ifndef PROCESSOR_FUNC_H_
#define PROCESSOR_FUNC_H_

#include "processor_manager.h"

typedef enum SPUErr
{
    SPU_CORRECT = 0,
    SPU_HALT_STATE = 1 << 1,
    SPU_DIVISION_BY_ZERO = 1 << 2,
    SPU_INVALID_COMMAND = 1 << 3,
    SPU_STACK_ERROR = 1 << 4,
    SPU_PROCESSOR_NULL = 1 << 5,
    SPU_REG_NULL = 1 << 6,
    SPU_BUFFER_NULL = 1 << 7,
    SPU_INVALID_REGISTER = 1 << 8,
    SPU_INVALID_RAM = 1 << 9
} SPUErr_t;

SPUErr_t spu_halt    (SPU* processor);
SPUErr_t spu_draw    (SPU* processor);

SPUErr_t spu_add     (SPU* processor);
SPUErr_t spu_sub     (SPU* processor);
SPUErr_t spu_mul     (SPU* processor);
SPUErr_t spu_div     (SPU* processor);
SPUErr_t spu_sqrt    (SPU* processor);

SPUErr_t spu_push    (SPU* processor);
SPUErr_t spu_out     (SPU* processor);
SPUErr_t spu_in      (SPU* processor);
SPUErr_t spu_pushr   (SPU* processor);
SPUErr_t spu_popr    (SPU* processor);
SPUErr_t spu_pushm   (SPU* processor);
SPUErr_t spu_popm    (SPU* processor);

SPUErr_t spu_jmp     (SPU* processor);
SPUErr_t spu_jb      (SPU* processor);
SPUErr_t spu_jbe     (SPU* processor);
SPUErr_t spu_ja      (SPU* processor);
SPUErr_t spu_jae     (SPU* processor);
SPUErr_t spu_je      (SPU* processor);
SPUErr_t spu_jne     (SPU* processor);
SPUErr_t spu_call    (SPU* processor);
SPUErr_t spu_ret     (SPU* processor);

#endif // PROCESSOR_FUNC_H_