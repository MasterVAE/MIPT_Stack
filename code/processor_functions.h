#ifndef PROCESSOR_FUNC_H
#define PROCESSOR_FUNC_H

#include "stack.h"

int get_int(char* buffer, size_t len);

int SPU_ADD     (SPU* processor);
int SPU_SUB     (SPU* processor);
int SPU_MUL     (SPU* processor);
int SPU_DIV     (SPU* processor);
int SPU_SQRT    (SPU* processor);
int SPU_PUSH    (SPU* processor);
int SPU_OUT     (SPU* processor);
int SPU_IN      (SPU* processor);
int SPU_PUSHR   (SPU* processor);
int SPU_POPR    (SPU* processor);

#endif