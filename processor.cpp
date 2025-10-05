#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "code/stack.h"
#include "code/language.h"
#include "code/assembler_read.h"
#include "code/processor_functions.h"

#define POP_ERR(stack, err) StackPop(stack, err); if(*err != 0) {stack->err_code = *err; return SPU_STACK_ERROR;}
#define INIT_ERR(stack, num) int err = StackInit(stack, num); if(err != 0){stack->err_code = err; return SPU_STACK_ERROR;}
#define PUSH_ERR(stack, value) {int error = StackPush(stack, value); if(error != 0) {stack->err_code = error; return SPU_STACK_ERROR;}}

int run(SPU* processor);
int SPUInit(SPU* processor);
void SPUDestroy(SPU* processor);
void SPUDump(SPU* processor);
int SPUVerify(SPU* processor);
void SPUErrorParser(int error);

int main()
{
    printf("SPU STARTING...\n");
    SPU spu_main = {};
    int error = SPUInit(&spu_main);
    if(error != SPU_CORRECT) SPUDump(&spu_main);

    FILE* input_file = fopen("files/code.bcode", "r");
    if(input_file == NULL)
    {
        StackDestroy(&spu_main.stack);
        return 1;
    }
    size_t size = 0;
    
    initialize_buffer(&spu_main.buffer, &size, input_file);
    fclose(input_file);

    while (1)
    {
        error = run(&spu_main);
        if(error == SPU_HALT) break;
        else if(error != SPU_CORRECT)
        { 
            SPUDump(&spu_main);
            break;
        }
    }
    //SPUDump(&spu_main);
    SPUDestroy(&spu_main);
    printf("SPU TURNING OFF...\n");
    return 0;
}

int run(SPU* processor)
{
    int inp = get_int(processor->buffer + processor->offcet, command_size);
    processor->offcet += command_size;

    switch (inp)
    {
        case HLT: return SPU_HALT;
        case ADD: return SPU_ADD(processor);
        case SUB: return SPU_SUB(processor);
        case MUL: return SPU_MUL(processor);
        case DIV: return SPU_DIV(processor);
        case SQRT: return SPU_SQRT(processor);
        case PUSH: return SPU_PUSH(processor);
        case OUT: return SPU_OUT(processor);
        case PUSHR: return SPU_PUSHR(processor);
        case IN: return SPU_IN(processor);
        case POPR: return SPU_POPR(processor);
        default: return SPU_INVALID_COMMAND;
    }
    return SPU_CORRECT;
}

int SPUInit(SPU* processor)
{
    if(processor == NULL) return 0;
    
    int error = StackInit(&processor->stack, 0);
    if(error != Verified)
    {
        processor->stack.err_code = error;
        return SPU_STACK_ERROR;
    }
    processor->reg = (int*)calloc(register_size, sizeof(int));
    processor->err_code = 0;
    return SPU_CORRECT;
}

void SPUDump(SPU* processor)
{
        fprintf(ERROR_STREAM, MAGENTA "\n\n- - - SPU dumping start - - -\n" CLEAN);
    SPUErrorParser(processor->err_code);
    if(processor == NULL)
    {
        fprintf(ERROR_STREAM, RED "Processor NULL\n" CLEAN); 
        fprintf(ERROR_STREAM, MAGENTA "\n- - - SPU dumping end - - -\n\n" CLEAN);
        return;
    }
        fprintf(ERROR_STREAM, "Offcet: %lu\n", processor->offcet);
        StackDump(&processor->stack);
    if(processor->reg == NULL)
    {
        fprintf(ERROR_STREAM, RED "Register NULL\n" CLEAN);
        fprintf(ERROR_STREAM, MAGENTA "\n- - - SPU dumping end - - -\n\n" CLEAN);
        return;
    }
        fprintf(ERROR_STREAM, "Register: \n");
    for(int i = 0; i < register_size; i++)
    {
        fprintf(ERROR_STREAM, "    [%d] %d\n", i, processor->reg[i]);
    }
        fprintf(ERROR_STREAM, MAGENTA "\n- - - SPU dumping end - - -\n\n" CLEAN);
}

int SPUVerify(SPU* processor)
{
    processor->err_code = Verified;

    if(processor == NULL) return processor->err_code |= SPU_PROCESSOR_NULL;
    if(StackVerify(&processor->stack) != Verified) processor->err_code |= SPU_STACK_ERROR;
    if(processor->buffer == NULL) processor->err_code |= SPU_BUFFER_NULL;
    if(processor->reg == NULL) processor->err_code |= SPU_REG_NULL;

    return processor->err_code;
}

void SPUDestroy(SPU* processor)
{
    if(processor == NULL) return;
    StackDestroy(&processor->stack);
    free(processor->reg);
    processor->reg = NULL;
    free(processor->buffer);
    processor->buffer = NULL;
}

void SPUErrorParser(int error)
{

    //FIXME PROCESSOR
    
    if(IsError(error, StackNull)) fprintf(ERROR_STREAM, "Error: stack NULL\n");
    if(IsError(error, CapacityInvalid)) fprintf(ERROR_STREAM, "Error: capacity invalid\n");
    if(IsError(error, DataNull)) fprintf(ERROR_STREAM, "Error: data NULL\n");
    if(IsError(error, StackOverflow)) fprintf(ERROR_STREAM, "Error: stack overflow\n");
    if(IsError(error, StackUnderflow)) fprintf(ERROR_STREAM, "Error: stack underflow\n");
    if(IsError(error, DataCorrupted)) fprintf(ERROR_STREAM, "Error: data corrupted\n");
}
