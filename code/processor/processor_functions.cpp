#include <math.h>
#include <string.h>
#include <unistd.h>

#include "processor_functions.h"
#include "stack.h"
#include "../language.h"
#include "../lib.h"
#include "../colors.h"

#define POP_ERR(stack, err) StackPop(stack, err); \
if(*err != 0) {(stack)->err_code = *err; return SPU_STACK_ERROR;}
#define INIT_ERR(stack, num) int err = StackInit(stack, num); \
if(err != 0) {(stack)->err_code = err; return SPU_STACK_ERROR;}
#define PUSH_ERR(stack, value) {int error = StackPush(stack, value); \
if(error != 0) {(stack)->err_code = error; return SPU_STACK_ERROR;}}

void draw(SPU* processor);


void draw(SPU* processor)
{
    usleep(20000);
    size_t vbuf_size = 2 * RAM_COUNT + RAM_COUNT/VRAM_BY_LINE + 1;
    char* vbuf = (char*)calloc(vbuf_size, sizeof(char));
    size_t vbuf_counter = 0;
    printf("\033[2J\033[fVRAM\n");
    for(size_t i = 0; i < RAM_COUNT; i++)
    {
        if(i % VRAM_BY_LINE == 0) vbuf[vbuf_counter++] = '\n';
        vbuf[vbuf_counter++] = processor->ram[i];
        vbuf[vbuf_counter++] = ' ';
    }
    vbuf[vbuf_counter++] = '\n';
    write(STDOUT_FILENO, vbuf, vbuf_size);
    free(vbuf);
}
spu_err spu_halt(SPU*)
{
    return SPU_HALT_STATE;
}

spu_err spu_draw(SPU* processor)
{
    draw(processor);
    return SPU_CORRECT;
}

spu_err spu_add(SPU* processor)
{
    int err = 0;
    stack_type a = POP_ERR(&processor->stack, &err);
    stack_type b = POP_ERR(&processor->stack, &err);

    PUSH_ERR(&processor->stack, a + b);
    return SPU_CORRECT;
}

spu_err spu_sub(SPU* processor)
{
    int err = 0;
    stack_type b = POP_ERR(&processor->stack, &err);
    stack_type a = POP_ERR(&processor->stack, &err);

    PUSH_ERR(&processor->stack, a - b);
    return SPU_CORRECT;
}

spu_err spu_mul(SPU* processor)
{
    int err = 0;
    stack_type a = POP_ERR(&processor->stack, &err);
    stack_type b = POP_ERR(&processor->stack, &err);

    PUSH_ERR(&processor->stack, a * b);
    return SPU_CORRECT;
}

spu_err spu_div(SPU* processor)
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

spu_err spu_sqrt(SPU* processor)
{
    int err = 0;
    stack_type value = POP_ERR(&processor->stack, &err);
    PUSH_ERR(&processor->stack, (int)sqrt(value));
    return SPU_CORRECT;
}

spu_err spu_push(SPU* processor)
{
    stack_type value = debytecode_int(processor->buffer + processor->offset, sizeof(VALUE_TYPE));
    processor->offset += sizeof(VALUE_TYPE);
    PUSH_ERR(&processor->stack, value);
    return SPU_CORRECT;
}

spu_err spu_out(SPU* processor)
{
    int err = 0;
    stack_type value = POP_ERR(&processor->stack, &err);
    printf("SPU OUT: %d\n", value);  
    return SPU_CORRECT;
}

spu_err spu_in(SPU* processor)
{
    printf("SPU IN: ");  
    stack_type value = 0;
    scanf("%d", &value);
    PUSH_ERR(&processor->stack, value);
    
    return SPU_CORRECT;
}

spu_err spu_pushr(SPU* processor)
{
    int reg = debytecode_int(processor->buffer + processor->offset, sizeof(VALUE_TYPE));
    if(reg < 0 || reg >= (int)REG_COUNT) return SPU_INVALID_REGISTER;
    processor->offset += sizeof(VALUE_TYPE);
    PUSH_ERR(&processor->stack, processor->reg[reg]); 
    return SPU_CORRECT;
}

spu_err spu_popr(SPU* processor)
{
    size_t reg = (size_t)debytecode_int(processor->buffer + processor->offset, sizeof(VALUE_TYPE));
    if(reg >= REG_COUNT) return SPU_INVALID_REGISTER;
    processor->offset += sizeof(VALUE_TYPE);
    int err = 0;
    processor->reg[reg] = POP_ERR(&processor->stack, &err);
   
    return SPU_CORRECT;
} 

spu_err spu_pushm(SPU* processor)
{
    int reg = debytecode_int(processor->buffer + processor->offset, sizeof(VALUE_TYPE));
    if(reg < 0 || reg >= (int)REG_COUNT) return SPU_INVALID_REGISTER;
    processor->offset += sizeof(VALUE_TYPE);

    int ram = processor->reg[reg];
    if(ram < 0 || ram >= (int)RAM_COUNT) return SPU_INVALID_RAM;

    PUSH_ERR(&processor->stack, processor->ram[ram]); 
    return SPU_CORRECT;
}

spu_err spu_popm(SPU* processor)
{
    int reg = debytecode_int(processor->buffer + processor->offset, sizeof(VALUE_TYPE));
    if(reg < 0 || reg >= (int)REG_COUNT) return SPU_INVALID_REGISTER;

    int ram = processor->reg[reg];
    if(ram < 0 || ram >= (int)RAM_COUNT) return SPU_INVALID_RAM;
    
    processor->offset += sizeof(VALUE_TYPE);
    int err = 0;
    processor->ram[processor->reg[reg]] = (char)POP_ERR(&processor->stack, &err);
   
    return SPU_CORRECT;
}

spu_err spu_jmp(SPU* processor)
{
    size_t offset = (size_t)debytecode_int(processor->buffer + processor->offset, sizeof(VALUE_TYPE));
    if(offset >= processor->buffer_size) return SPU_INVALID_COMMAND;
    processor->offset = offset;
    return SPU_CORRECT;
}

spu_err spu_jb(SPU* processor)
{
    int err = 0;
    stack_type b = POP_ERR(&processor->stack, &err);
    stack_type a = POP_ERR(&processor->stack, &err);

    if(a >= b)
    {
        processor->offset += sizeof(VALUE_TYPE);
        return SPU_CORRECT;
    }

    size_t offset = (size_t)debytecode_int(processor->buffer + processor->offset, sizeof(VALUE_TYPE));
    if(offset >= processor->buffer_size) return SPU_INVALID_COMMAND;
    processor->offset = offset;
    return SPU_CORRECT;
}

spu_err spu_jbe(SPU* processor)
{
    int err = 0;
    stack_type b = POP_ERR(&processor->stack, &err);
    stack_type a = POP_ERR(&processor->stack, &err);

    if(a > b)
    {
        processor->offset += sizeof(VALUE_TYPE);
        return SPU_CORRECT;
    }

    size_t offset = (size_t)debytecode_int(processor->buffer + processor->offset, sizeof(VALUE_TYPE));
    if(offset >= processor->buffer_size) return SPU_INVALID_COMMAND;
    processor->offset = offset;
    return SPU_CORRECT;
}

spu_err spu_ja(SPU* processor)
{
    int err = 0;
    stack_type b = POP_ERR(&processor->stack, &err);
    stack_type a = POP_ERR(&processor->stack, &err);

    if(a <= b)
    {
        processor->offset += sizeof(VALUE_TYPE);
        return SPU_CORRECT;
    }

    size_t offset = (size_t)debytecode_int(processor->buffer + processor->offset, sizeof(VALUE_TYPE));
    if(offset >= processor->buffer_size) return SPU_INVALID_COMMAND;
    processor->offset = offset;
    return SPU_CORRECT;
}

spu_err spu_jae(SPU* processor)
{
    int err = 0;
    stack_type b = POP_ERR(&processor->stack, &err);
    stack_type a = POP_ERR(&processor->stack, &err);

    if(a < b)
    {
        processor->offset += sizeof(VALUE_TYPE);
        return SPU_CORRECT;
    }

    size_t offset = (size_t)debytecode_int(processor->buffer + processor->offset, sizeof(VALUE_TYPE));
    if(offset >= processor->buffer_size) return SPU_INVALID_COMMAND;
    processor->offset = offset;
    return SPU_CORRECT;
}

spu_err spu_je(SPU* processor)
{
    int err = 0;
    stack_type b = POP_ERR(&processor->stack, &err);
    stack_type a = POP_ERR(&processor->stack, &err);

    if(a != b)
    {
        processor->offset += sizeof(VALUE_TYPE);
        return SPU_CORRECT;
    }

    size_t offset = (size_t)debytecode_int(processor->buffer + processor->offset, sizeof(VALUE_TYPE));
    if(offset >= processor->buffer_size) return SPU_INVALID_COMMAND;
    processor->offset = offset;
    return SPU_CORRECT;
}

spu_err spu_jne(SPU* processor)
{
    int err = 0;
    stack_type b = POP_ERR(&processor->stack, &err);
    stack_type a = POP_ERR(&processor->stack, &err);

    if(a == b)
    {
        processor->offset += sizeof(VALUE_TYPE);
        return SPU_CORRECT;
    }

    size_t offset = (size_t)debytecode_int(processor->buffer + processor->offset, sizeof(VALUE_TYPE));
    if(offset >= processor->buffer_size) return SPU_INVALID_COMMAND;
    processor->offset = offset;
    return SPU_CORRECT;
}

spu_err spu_call(SPU* processor)
{
    PUSH_ERR(&processor->return_stack, (stack_type)(processor->offset + sizeof(VALUE_TYPE)));
    size_t offset = (size_t)debytecode_int(processor->buffer + processor->offset, sizeof(VALUE_TYPE));
    if(offset >= processor->buffer_size) return SPU_INVALID_COMMAND;
    processor->offset = offset;
    return SPU_CORRECT;
}

spu_err spu_ret(SPU* processor)
{
    int err = 0;
    size_t offset = (size_t)POP_ERR(&processor->return_stack, &err);
    if(offset >= processor->buffer_size) return SPU_INVALID_COMMAND;
    processor->offset = offset;
    return SPU_CORRECT;
}
