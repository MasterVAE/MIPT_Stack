#ifndef PROCESSOR_LIFE_H_
#define PROCESSOR_LIFE_H_

#include "stack.h"

typedef struct SPU
{
    Stack_t stack;
    char* buffer;
    size_t offset;
    size_t buffer_size;
    Stack_t return_stack;

    int reg[REG_SIZE];
    char ram[RAM_SIZE]; //ВЫНЕСТИ В КУЧУ

    int err_code;
    
} SPU;

int  SPUInit        (SPU* processor);
void SPUDestroy     (SPU* processor);
void SPUDump        (SPU* processor);
int  SPUVerify      (SPU* processor);
void SPUErrorParser (int error);

const int BUFFER_CHARS_BY_LINE = 32;

#endif //PROCESSOR_LIFE_H_