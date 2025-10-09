#include <math.h>
#include <string.h>

#include "processor_functions.h"
#include "stack.h"
#include "../language.h"

#define POP_ERR(stack, err) StackPop(stack, err); \
if(*err != 0) {(stack)->err_code = *err; return SPU_STACK_ERROR;}
#define INIT_ERR(stack, num) int err = StackInit(stack, num); \
if(err != 0) {(stack)->err_code = err; return SPU_STACK_ERROR;}
#define PUSH_ERR(stack, value) {int error = StackPush(stack, value); \
if(error != 0) {(stack)->err_code = error; return SPU_STACK_ERROR;}}

int get_int(char* code, size_t size)
{
    int value;
    memcpy(&value, code, size);
    return value;
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
    stack_type value = get_int(processor->buffer + processor->offset, sizeof(VALUE_TYPE));
    processor->offset += sizeof(VALUE_TYPE);
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
    int reg = get_int(processor->buffer + processor->offset, sizeof(VALUE_TYPE));
    if(reg < 0 || reg >= (int)REG_COUNT) return SPU_INVALID_REGISTER;
    processor->offset += sizeof(VALUE_TYPE);
    PUSH_ERR(&processor->stack, processor->reg[reg]); 
    return SPU_CORRECT;
}

int spu_popr(SPU* processor)
{
    size_t reg = (size_t)get_int(processor->buffer + processor->offset, sizeof(VALUE_TYPE));
    if(reg >= REG_COUNT) return SPU_INVALID_REGISTER;
    processor->offset += sizeof(VALUE_TYPE);
    int err = 0;
    processor->reg[reg] = POP_ERR(&processor->stack, &err);
   
    return SPU_CORRECT;
} 

int spu_jmp(SPU* processor)
{
    size_t offset = (size_t)get_int(processor->buffer + processor->offset, sizeof(VALUE_TYPE));
    if(offset >= processor->buffer_size) return SPU_INVALID_COMMAND;
    processor->offset = offset;
    return SPU_CORRECT;
}

int spu_jb(SPU* processor)
{
    int err = 0;
    stack_type b = POP_ERR(&processor->stack, &err);
    stack_type a = POP_ERR(&processor->stack, &err);

    if(a >= b)
    {
        processor->offset += sizeof(VALUE_TYPE);
        return SPU_CORRECT;
    }

    size_t offset = (size_t)get_int(processor->buffer + processor->offset, sizeof(VALUE_TYPE));
    if(offset >= processor->buffer_size) return SPU_INVALID_COMMAND;
    processor->offset = offset;
    return SPU_CORRECT;
}

int spu_jbe(SPU* processor)
{
    int err = 0;
    stack_type b = POP_ERR(&processor->stack, &err);
    stack_type a = POP_ERR(&processor->stack, &err);

    if(a > b)
    {
        processor->offset += sizeof(VALUE_TYPE);
        return SPU_CORRECT;
    }

    size_t offset = (size_t)get_int(processor->buffer + processor->offset, sizeof(VALUE_TYPE));
    if(offset >= processor->buffer_size) return SPU_INVALID_COMMAND;
    processor->offset = offset;
    return SPU_CORRECT;
}

int spu_ja(SPU* processor)
{
    int err = 0;
    stack_type b = POP_ERR(&processor->stack, &err);
    stack_type a = POP_ERR(&processor->stack, &err);

    if(a <= b)
    {
        processor->offset += sizeof(VALUE_TYPE);
        return SPU_CORRECT;
    }

    size_t offset = (size_t)get_int(processor->buffer + processor->offset, sizeof(VALUE_TYPE));
    if(offset >= processor->buffer_size) return SPU_INVALID_COMMAND;
    processor->offset = offset;
    return SPU_CORRECT;
}

int spu_jae(SPU* processor)
{
    int err = 0;
    stack_type b = POP_ERR(&processor->stack, &err);
    stack_type a = POP_ERR(&processor->stack, &err);

    if(a < b)
    {
        processor->offset += sizeof(VALUE_TYPE);
        return SPU_CORRECT;
    }

    size_t offset = (size_t)get_int(processor->buffer + processor->offset, sizeof(VALUE_TYPE));
    if(offset >= processor->buffer_size) return SPU_INVALID_COMMAND;
    processor->offset = offset;
    return SPU_CORRECT;
}

int spu_je(SPU* processor)
{
    int err = 0;
    stack_type b = POP_ERR(&processor->stack, &err);
    stack_type a = POP_ERR(&processor->stack, &err);

    if(a != b)
    {
        processor->offset += sizeof(VALUE_TYPE);
        return SPU_CORRECT;
    }


    size_t offset = (size_t)get_int(processor->buffer + processor->offset, sizeof(VALUE_TYPE));
    if(offset >= processor->buffer_size) return SPU_INVALID_COMMAND;
    processor->offset = offset;
    return SPU_CORRECT;
}

int spu_jne(SPU* processor)
{
    int err = 0;
    stack_type b = POP_ERR(&processor->stack, &err);
    stack_type a = POP_ERR(&processor->stack, &err);

    if(a == b)
    {
        processor->offset += sizeof(VALUE_TYPE);
        return SPU_CORRECT;
    }

    size_t offset = (size_t)get_int(processor->buffer + processor->offset, sizeof(VALUE_TYPE));
    if(offset >= processor->buffer_size) return SPU_INVALID_COMMAND;
    processor->offset = offset;
    return SPU_CORRECT;
}


