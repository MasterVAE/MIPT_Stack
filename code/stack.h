#ifndef STACK_H_
#define STACK_H_

#include <stdio.h>

typedef struct Stack_t
{
    int* data;
    size_t size;
    size_t capacity;
} Stack_t;

typedef enum StackErr_t
{
    NO_ERROR = 0,
    STACK_POINTER_NULL = 1,
    STACK_DATA_NULL = 2,
    STACK_CAPACITY_INCORRECT = 3,
    STACK_SIZE_INCORECT = 4,
    STACK_OVERFLOW = 5
} StackErr_t;

void StackDump(Stack_t* stack);
StackErr_t StackVerify(Stack_t* stack);
StackErr_t StackInit(Stack_t* stack, size_t capacity);
StackErr_t StackPush(Stack_t* stack, int value);
int StackPop(Stack_t* stack, StackErr_t* err = NULL);
void StackDestroy(Stack_t* stack);

#endif