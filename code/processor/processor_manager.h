#ifndef PROCESSOR_MANAGER_H_
#define PROCESSOR_MANAGER_H_

#include "stack.h"

typedef struct SPU
{
    Stack_t stack;
    char* command_buffer;
    size_t command_buffer_size;
    size_t command_pointer;
    Stack_t return_stack;

    int reg[REG_SIZE];
    char* ram;

    int err_code;
    
} SPU;

int  SPUInit        (SPU* processor);
void SPUDestroy     (SPU* processor);
void SPUDump        (SPU* processor);
int  SPUVerify      (SPU* processor);
void SPUErrorParser (int error);

const int BUFFER_CHARS_BY_LINE = 32;

#endif // PROCESSOR_MANAGER_H_