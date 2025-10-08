#ifndef STACK_H_
#define STACK_H_

#include <stdio.h>
#include "../language.h"


const int SHIELD_START = 0x6BADF00D;
const int SHIELD_END = 0x7BADF00D;
#define ERROR_STREAM stderr
const int STACK_MULTIPLIER = 2;

enum StackError
{
    Verified = 0,
    StackNull = 1 << 1,
    StackOverflow = 1 << 2,
    DataNull = 1 << 3,
    DataCorrupted = 1 << 4,
    CapacityInvalid = 1 << 5,
    StackUnderflow = 1 << 6
};

bool IsError(int error, StackError check);

typedef struct Stack_t
{
    stack_type* data;
    size_t size;
    size_t capacity;
    int err_code;
} Stack_t;

typedef struct processor
{
    Stack_t stack;
    char* buffer;
    size_t offcet;

    int* reg;

    int err_code;
    
} SPU;

void StackDump(Stack_t* stack);
int StackVerify(Stack_t* stack);
int StackInit(Stack_t* stack, size_t capacity);
int StackPush(Stack_t* stack, stack_type value);
int StackPop(Stack_t* stack, int* err = NULL);
void StackDestroy(Stack_t* stack);

const int shield_size = 1;

#endif