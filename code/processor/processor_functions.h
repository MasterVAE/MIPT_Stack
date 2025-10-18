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

SPUErr_t SpuHalt    (SPU* processor);
SPUErr_t SpuDraw    (SPU* processor);

SPUErr_t SpuAdd     (SPU* processor);
SPUErr_t SpuSub     (SPU* processor);
SPUErr_t SpuMul     (SPU* processor);
SPUErr_t SpuDiv     (SPU* processor);
SPUErr_t SpuSqrt    (SPU* processor);

SPUErr_t SpuPush    (SPU* processor);
SPUErr_t SpuOut     (SPU* processor);
SPUErr_t SpuIn      (SPU* processor);
SPUErr_t SpuPushr   (SPU* processor);
SPUErr_t SpuPopr    (SPU* processor);
SPUErr_t SpuPushm   (SPU* processor);
SPUErr_t SpuPopm    (SPU* processor);

SPUErr_t SpuJmp     (SPU* processor);
SPUErr_t SpuJb      (SPU* processor);
SPUErr_t SpuJbe     (SPU* processor);
SPUErr_t SpuJa      (SPU* processor);
SPUErr_t SpuJae     (SPU* processor);
SPUErr_t SpuJe      (SPU* processor);
SPUErr_t SpuJne      (SPU* processor);
SPUErr_t SpuCall    (SPU* processor);
SPUErr_t SpuRet     (SPU* processor);

#endif // PROCESSOR_FUNC_H_