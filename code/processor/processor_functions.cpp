#include <math.h>
#include <string.h>
#include <unistd.h>

#include "processor_functions.h"
#include "stack.h"
#include "../constants.h"
#include "../lib.h"
#include "../colors.h"

const size_t FRAME_DELAY = 33300;

#define INIT_ERR(stack, num) int err = StackInit(stack, num); \
    if(err != 0) {(stack)->err_code = err; return SPU_STACK_ERROR;}

#define POP_ERR(stack, err) StackPop(stack, err); \
    if(*err != 0) {(stack)->err_code = *err; return SPU_STACK_ERROR;}

#define PUSH_ERR(stack, value) {int error = StackPush(stack, value); \
    if(error != 0) {(stack)->err_code = error; return SPU_STACK_ERROR;}}

static void Draw(SPU* processor);

static void Draw(SPU* processor)
{
    usleep(FRAME_DELAY);
    printf(ESCAPE "VRAM\n");

    size_t vbuf_size = 2 * RAM_SIZE + RAM_SIZE/VRAM_BY_LINE + 1;
    char* vbuf = (char*)calloc(vbuf_size, sizeof(char));
    size_t vbuf_counter = 0;
    
    for(size_t i = 0; i < RAM_SIZE; i++)
    {
        if(i % VRAM_BY_LINE == 0) vbuf[vbuf_counter++] = '\n';
        
        vbuf[vbuf_counter++] = processor->ram[i];
        vbuf[vbuf_counter++] = ' ';
    }
    vbuf[vbuf_counter++] = '\n';

    fwrite(vbuf, vbuf_size, 1, stdout);
    free(vbuf);
}

SPUErr_t SpuHalt(SPU*)
{
    return SPU_HALT_STATE;
}

SPUErr_t SpuDraw(SPU* processor)
{
    Draw(processor);
    
    return SPU_CORRECT;
}

SPUErr_t SpuAdd(SPU* processor)
{
    int err = 0;
    stack_type a = POP_ERR(&processor->stack, &err);
    stack_type b = POP_ERR(&processor->stack, &err);

    PUSH_ERR(&processor->stack, a + b);

    return SPU_CORRECT;
}

SPUErr_t SpuSub(SPU* processor)
{
    int err = 0;
    stack_type b = POP_ERR(&processor->stack, &err);
    stack_type a = POP_ERR(&processor->stack, &err);

    PUSH_ERR(&processor->stack, a - b);
    
    return SPU_CORRECT;
}

SPUErr_t SpuMul(SPU* processor)
{
    int err = 0;
    stack_type a = POP_ERR(&processor->stack, &err);
    stack_type b = POP_ERR(&processor->stack, &err);

    PUSH_ERR(&processor->stack, a * b);

    return SPU_CORRECT;
}

SPUErr_t SpuDiv(SPU* processor)
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

SPUErr_t SpuSqrt(SPU* processor)
{
    int err = 0;
    stack_type value = POP_ERR(&processor->stack, &err);
    PUSH_ERR(&processor->stack, (int)sqrt(value));

    return SPU_CORRECT;
}

SPUErr_t SpuPush(SPU* processor)
{
    stack_type value = DebytecodeInt(processor->command_buffer + processor->command_pointer, sizeof(value_type));
    processor->command_pointer += sizeof(value_type);
    PUSH_ERR(&processor->stack, value);

    return SPU_CORRECT;
}

SPUErr_t SpuOut(SPU* processor)
{
    int err = 0;
    stack_type value = POP_ERR(&processor->stack, &err);
    printf("SPU OUT: %d\n", value);  
    return SPU_CORRECT;
}

SPUErr_t SpuIn(SPU* processor)
{
    printf("SPU IN: ");  
    stack_type value = 0;
    scanf("%d", &value);
    PUSH_ERR(&processor->stack, value);
    
    return SPU_CORRECT;
}

SPUErr_t SpuPushr(SPU* processor)
{
    int reg = DebytecodeInt(processor->command_buffer + processor->command_pointer, sizeof(value_type));
    if(reg < 0 || reg >= (int)REG_SIZE) return SPU_INVALID_REGISTER;
    processor->command_pointer += sizeof(value_type);
    PUSH_ERR(&processor->stack, processor->reg[reg]); 
    return SPU_CORRECT;
}

SPUErr_t SpuPopr(SPU* processor)
{
    size_t reg = (size_t)DebytecodeInt(processor->command_buffer + processor->command_pointer, sizeof(value_type));
    if(reg >= REG_SIZE) return SPU_INVALID_REGISTER;
    processor->command_pointer += sizeof(value_type);
    int err = 0;
    processor->reg[reg] = POP_ERR(&processor->stack, &err);
   
    return SPU_CORRECT;
} 

SPUErr_t SpuPushm(SPU* processor)
{
    int reg = DebytecodeInt(processor->command_buffer + processor->command_pointer, sizeof(value_type));
    if(reg < 0 || reg >= (int)REG_SIZE) return SPU_INVALID_REGISTER;
    processor->command_pointer += sizeof(value_type);

    int ram = processor->reg[reg];
    if(ram < 0 || ram >= (int)RAM_SIZE) return SPU_INVALID_RAM;

    PUSH_ERR(&processor->stack, processor->ram[ram]); 
    return SPU_CORRECT;
}

SPUErr_t SpuPopm(SPU* processor)
{
    int reg = DebytecodeInt(processor->command_buffer + processor->command_pointer, sizeof(value_type));
    if(reg < 0 || reg >= (int)REG_SIZE) return SPU_INVALID_REGISTER;

    int ram = processor->reg[reg];
    if(ram < 0 || ram >= (int)RAM_SIZE) return SPU_INVALID_RAM;
    
    processor->command_pointer += sizeof(value_type);
    int err = 0;
    processor->ram[processor->reg[reg]] = (char)POP_ERR(&processor->stack, &err);
   
    return SPU_CORRECT;
}

SPUErr_t SpuJmp(SPU* processor)
{
    size_t offset = (size_t)DebytecodeInt(processor->command_buffer + processor->command_pointer, sizeof(value_type));
    if(offset >= processor->command_buffer_size) return SPU_INVALID_COMMAND;
    processor->command_pointer = offset;

    return SPU_CORRECT;
}

SPUErr_t SpuJb(SPU* processor)
{
    int err = 0;
    stack_type b = POP_ERR(&processor->stack, &err);
    stack_type a = POP_ERR(&processor->stack, &err);

    if(a >= b)
    {
        processor->command_pointer += sizeof(value_type);

        return SPU_CORRECT;
    }

    return SpuJmp(processor);
}

SPUErr_t SpuJbe(SPU* processor)
{
    int err = 0;
    stack_type b = POP_ERR(&processor->stack, &err);
    stack_type a = POP_ERR(&processor->stack, &err);

    if(a > b)
    {
        processor->command_pointer += sizeof(value_type);

        return SPU_CORRECT;
    }

    return SpuJmp(processor);
}

SPUErr_t SpuJa(SPU* processor)
{
    int err = 0;
    stack_type b = POP_ERR(&processor->stack, &err);
    stack_type a = POP_ERR(&processor->stack, &err);

    if(a <= b)
    {
        processor->command_pointer += sizeof(value_type);

        return SPU_CORRECT;
    }

    return SpuJmp(processor);
}

SPUErr_t SpuJae(SPU* processor)
{
    int err = 0;
    stack_type b = POP_ERR(&processor->stack, &err);
    stack_type a = POP_ERR(&processor->stack, &err);

    if(a < b)
    {
        processor->command_pointer += sizeof(value_type);

        return SPU_CORRECT;
    }

    return SpuJmp(processor);
}

SPUErr_t SpuJe(SPU* processor)
{
    int err = 0;
    stack_type b = POP_ERR(&processor->stack, &err);
    stack_type a = POP_ERR(&processor->stack, &err);

    if(a != b)
    {
        processor->command_pointer += sizeof(value_type);

        return SPU_CORRECT;
    }

    return SpuJmp(processor);
}

SPUErr_t SpuJne(SPU* processor)
{
    int err = 0;
    stack_type b = POP_ERR(&processor->stack, &err);
    stack_type a = POP_ERR(&processor->stack, &err);

    if(a == b)
    {
        processor->command_pointer += sizeof(value_type);

        return SPU_CORRECT;
    }

    return SpuJmp(processor);
}

SPUErr_t SpuCall(SPU* processor)
{
    PUSH_ERR(&processor->return_stack, (stack_type)(processor->command_pointer + sizeof(value_type)));
    return SpuJmp(processor);
}

SPUErr_t SpuRet(SPU* processor)
{
    int err = 0;
    size_t offset = (size_t)POP_ERR(&processor->return_stack, &err);
    if(offset >= processor->command_buffer_size) return SPU_INVALID_COMMAND;
    processor->command_pointer = offset;

    return SPU_CORRECT;
}
