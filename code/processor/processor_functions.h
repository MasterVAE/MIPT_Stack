#ifndef PROCESSOR_FUNC_H
#define PROCESSOR_FUNC_H

#include "stack.h"

int get_int(char* buffer, size_t len);

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

#endif