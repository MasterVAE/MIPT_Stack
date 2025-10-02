#ifndef STACK_H_
#define STACK_H_

#include <stdio.h>

#define RED "\033[31m"
#define CLEAN "\033[0m"
#define GREEN "\033[32m"

// FIXME const
#define SHIELD_START 0x6BADF00D
#define SHIELD_END 0x7BADF00D
#define ERROR_STREAM stderr
#define STACK_MULTIPLIER 2

// FIXME в порядке правильном
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
typedef int stack_type;

typedef struct Stack_t
{
    stack_type* data;
    size_t size;
    size_t capacity;
} Stack_t;

void ErrorParser(int error);
void StackDump(Stack_t* stack, int error = 0);
int StackVerify(Stack_t* stack);
int StackInit(Stack_t* stack, size_t capacity);
int StackPush(Stack_t* stack, stack_type value);
int StackPop(Stack_t* stack, int* err = NULL);
void StackDestroy(Stack_t* stack);


#endif