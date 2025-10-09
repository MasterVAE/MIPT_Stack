#ifndef PROCESSOR_LIFE_H
#define PROCESSOR_LIFE_H

#include "stack.h"

int SPUInit         (SPU* processor);
void SPUDestroy     (SPU* processor);
void SPUDump        (SPU* processor);
int SPUVerify       (SPU* processor);
void SPUErrorParser (int error);

#endif