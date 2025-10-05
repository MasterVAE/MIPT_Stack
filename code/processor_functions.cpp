#include <math.h>

#include "processor_functions.h"
#include "stack.h"

#define POP_ERR(stack, err) StackPop(stack, err); if(*err != 0) return SPU_STACK_ERROR;
#define PUSH_ERR(stack, value) {int error = StackPush(stack, value); if(error != 0) return SPU_STACK_ERROR;}


int get_int(char* buffer, size_t len)
{
    unsigned int ans = 0;
    for(size_t i = 0; i < len; i++)
    {
        ans *= 2;
        ans += (unsigned int)(buffer[i] - '0');
    }
    return (int)ans;
}

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

int SPU_PUSH(SPU* processor)
{
    stack_type value = get_int(processor->buffer + processor->offcet, value_size);
    processor->offcet += value_size;
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

int SPU_IN(SPU* processor)
{
    printf("SPU IN: ");  
    stack_type value = 0;
    scanf("%d", &value);
    PUSH_ERR(&processor->stack, value);
    
    return SPU_CORRECT;
}

int SPU_PUSHR(SPU* processor)
{
    stack_type reg = get_int(processor->buffer + processor->offcet, value_size);
    if(reg < 0 || reg >= register_size) return SPU_INVALID_REGISTER;
    processor->offcet += value_size;
    PUSH_ERR(&processor->stack, processor->reg[reg]); 
    return SPU_CORRECT;
}

int SPU_POPR(SPU* processor)
{
    stack_type reg = get_int(processor->buffer + processor->offcet, value_size);
    if(reg < 0 || reg >= register_size) return SPU_INVALID_REGISTER;
    processor->offcet += value_size;
    int err = 0;
    processor->reg[reg] = POP_ERR(&processor->stack, &err);
   
    return SPU_CORRECT;
}