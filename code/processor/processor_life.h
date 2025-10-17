#ifndef PROCESSOR_LIFE_H_
#define PROCESSOR_LIFE_H_

#include "stack.h"

int SPUInit         (SPU* processor);
void SPUDestroy     (SPU* processor);
void SPUDump        (SPU* processor);
int SPUVerify       (SPU* processor);
void SPUErrorParser (int error);

#endif //PROCESSOR_LIFE_H_