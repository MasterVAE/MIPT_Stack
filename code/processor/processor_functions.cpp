#include <math.h>

#include "processor_functions.h"
#include "stack.h"
#include "../language.h"

#define POP_ERR(stack, err) StackPop(stack, err); if(*err != 0) return SPU_STACK_ERROR;
#define PUSH_ERR(stack, value) {int error = StackPush(stack, value); if(error != 0) return SPU_STACK_ERROR;}


int get_int(char* code, size_t size)
{
    int ans = 0;
    for(int i = (int)size-1; i >= 0; i--)
    {
        ans *= 256;
        ans += *(code+i);
    }
    return ans;
}


int spu_halt(SPU*)
{
    return SPU_HALT_STATE;
}

int spu_add(SPU* processor)
{
    int err = 0;
    stack_type a = POP_ERR(&processor->stack, &err);
    stack_type b = POP_ERR(&processor->stack, &err);

    PUSH_ERR(&processor->stack, a + b);
    return SPU_CORRECT;
}

int spu_sub(SPU* processor)
{
    int err = 0;
    stack_type b = POP_ERR(&processor->stack, &err);
    stack_type a = POP_ERR(&processor->stack, &err);

    PUSH_ERR(&processor->stack, a - b);
    return SPU_CORRECT;
}

int spu_mul(SPU* processor)
{
    int err = 0;
    stack_type a = POP_ERR(&processor->stack, &err);
    stack_type b = POP_ERR(&processor->stack, &err);

    PUSH_ERR(&processor->stack, a * b);
    return SPU_CORRECT;
}

int spu_div(SPU* processor)
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

int spu_sqrt(SPU* processor)
{
    int err = 0;
    stack_type value = POP_ERR(&processor->stack, &err);
    PUSH_ERR(&processor->stack, (int)sqrt(value));
    return SPU_CORRECT;
}

int spu_push(SPU* processor)
{
    stack_type value = get_int(processor->buffer + processor->offcet, sizeof(VALUE_TYPE));
    processor->offcet += sizeof(VALUE_TYPE);
    PUSH_ERR(&processor->stack, value);
    return SPU_CORRECT;
}

int spu_out(SPU* processor)
{
    int err = 0;
    stack_type value = POP_ERR(&processor->stack, &err);
    printf("SPU OUT: %d\n", value);  
    return SPU_CORRECT;
}

int spu_in(SPU* processor)
{
    printf("SPU IN: ");  
    stack_type value = 0;
    scanf("%d", &value);
    PUSH_ERR(&processor->stack, value);
    
    return SPU_CORRECT;
}

int spu_pushr(SPU* processor)
{
    int reg = get_int(processor->buffer + processor->offcet, sizeof(VALUE_TYPE));
    if(reg < 0 || reg >= (int)REG_SIZE) return SPU_INVALID_REGISTER;
    processor->offcet += sizeof(VALUE_TYPE);
    PUSH_ERR(&processor->stack, processor->reg[reg]); 
    return SPU_CORRECT;
}

int spu_popr(SPU* processor)
{
    int reg = get_int(processor->buffer + processor->offcet, sizeof(VALUE_TYPE));
    if(reg < 0 || reg >= (int)REG_SIZE) return SPU_INVALID_REGISTER;
    processor->offcet += sizeof(VALUE_TYPE);
    int err = 0;
    processor->reg[reg] = POP_ERR(&processor->stack, &err);
   
    return SPU_CORRECT;
}