#ifndef PROCESSOR_FUNC_H_
#define PROCESSOR_FUNC_H_

#include "processor_manager.h"

typedef enum SPUState
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
} SPUState_t;

SPUState_t SpuHalt    (SPU* processor);
SPUState_t SpuDraw    (SPU* processor);

SPUState_t SpuAdd     (SPU* processor);
SPUState_t SpuSub     (SPU* processor);
SPUState_t SpuMul     (SPU* processor);
SPUState_t SpuDiv     (SPU* processor);
SPUState_t SpuSqrt    (SPU* processor);

SPUState_t SpuPush    (SPU* processor);
SPUState_t SpuOut     (SPU* processor);
SPUState_t SpuIn      (SPU* processor);
SPUState_t SpuPushr   (SPU* processor);
SPUState_t SpuPopr    (SPU* processor);
SPUState_t SpuPushm   (SPU* processor);
SPUState_t SpuPopm    (SPU* processor);

SPUState_t SpuJmp     (SPU* processor);
SPUState_t SpuJb      (SPU* processor);
SPUState_t SpuJbe     (SPU* processor);
SPUState_t SpuJa      (SPU* processor);
SPUState_t SpuJae     (SPU* processor);
SPUState_t SpuJe      (SPU* processor);
SPUState_t SpuJne      (SPU* processor);
SPUState_t SpuCall    (SPU* processor);
SPUState_t SpuRet     (SPU* processor);

#endif // PROCESSOR_FUNC_H_