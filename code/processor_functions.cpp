#include <math.h>

#include "processor_functions.h"
#include "stack.h"

#define POP_ERR(stack, err) StackPop(stack, err); if(*err != 0) {StackDump(stack, *err); return SPU_STACK_ERROR;}
#define INIT_ERR(stack, num) int err = StackInit(stack, num); if(err != 0) {StackDump(stack, err); return SPU_STACK_ERROR;}
#define PUSH_ERR(stack, value) {int error = StackPush(stack, value); if(error != 0) {StackDump(stack, error); return SPU_STACK_ERROR;}}


int SPU_ADD(SPU* processor)
{
    int err = 0;
    stack_type a = POP_ERR(&processor->stack, &err);
    stack_type b = POP_ERR(&processor->stack, &err);

    PUSH_ERR(&processor->stack, a + b);
    return SPU_CORRECT;
}

int SPU_SUB(SPU* processor)
{
    int err = 0;
    stack_type b = POP_ERR(&processor->stack, &err);
    stack_type a = POP_ERR(&processor->stack, &err);

    PUSH_ERR(&processor->stack, a - b);
    return SPU_CORRECT;
}

int SPU_MUL(SPU* processor)
{
    int err = 0;
    stack_type a = POP_ERR(&processor->stack, &err);
    stack_type b = POP_ERR(&processor->stack, &err);

    PUSH_ERR(&processor->stack, a * b);
    return SPU_CORRECT;
}

int SPU_DIV(SPU* processor)
{
    int err = 0;
    stack_type b = POP_ERR(&processor->stack, &err);

    if(b == 0)
    {
        PUSH_ERR(&processor->stack, b);
        return SPU_DIVISION_BY_ZERO;
    }

    stack_type a = POP_ERR(&processor->stack, &err);
    PUSH_ERR(&processor->stack, a/b);
    return SPU_CORRECT;
}

int SPU_SQRT(SPU* processor)
{
    int err = 0;
    stack_type value = POP_ERR(&processor->stack, &err);
    PUSH_ERR(&processor->stack, (int)sqrt(value));
    return SPU_CORRECT;
}

int SPU_PUSH(SPU* processor, stack_type value)
{
    PUSH_ERR(&processor->stack, value);
    return SPU_CORRECT;
}

int SPU_OUT(SPU* processor)
{
    int err = 0;
    stack_type value = POP_ERR(&processor->stack, &err);
    printf("SPU OUT: %d\n", value);  
    return SPU_CORRECT;
}