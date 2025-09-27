#ifndef STACK_H_
#define STACK_H_

#include <stdio.h>

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