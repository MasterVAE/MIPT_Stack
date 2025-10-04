#ifndef PROCESSOR_FUNC_H
#define PROCESSOR_FUNC_H

#include "stack.h"

int SPU_ADD(SPU* processor);
int SPU_SUB(SPU* processor);
int SPU_MUL(SPU* processor);
int SPU_DIV(SPU* processor);
int SPU_SQRT(SPU* processor);
int SPU_PUSH(SPU* processor, stack_type value);
int SPU_OUT(SPU* processor);

#endif