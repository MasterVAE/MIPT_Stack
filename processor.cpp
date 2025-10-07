#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "code/stack.h"
#include "code/language.h"
#include "code/assembler_read.h"
#include "code/processor_functions.h"

#define SPU_MODE
#include "code/commands.h"

#define POP_ERR(stack, err) StackPop(stack, err); if(*err != 0) {stack->err_code = *err; return SPU_STACK_ERROR;}
#define INIT_ERR(stack, num) int err = StackInit(stack, num); if(err != 0){stack->err_code = err; return SPU_STACK_ERROR;}
#define PUSH_ERR(stack, value) {int error = StackPush(stack, value); if(error != 0) {stack->err_code = error; return SPU_STACK_ERROR;}}

int run(SPU* processor);
int IsError(int error, int check);
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
    spu_main.err_code = error;
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
        if(error == SPU_HALT_STATE) break;
        else if(error != SPU_CORRECT)
        { 
            spu_main.err_code = error;
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

    for(int i = 0; i < COMMANDS_COUNT; i++)
    {
        if(inp == COMMANDS[i].num)  return COMMANDS[i].spu_func(processor);
    }
    return SPU_INVALID_COMMAND;
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
    if(processor->buffer == NULL)
    {
        fprintf(ERROR_STREAM, "Buffer NULL\n");
    }
    else
    {
        fprintf(ERROR_STREAM, "Buffer:\n\n%s\n\n", processor->buffer);
    }
        fprintf(ERROR_STREAM, "Offcet: %lu\n", processor->offcet);
        StackDump(&processor->stack);
    if(processor->reg == NULL)
    {
        fprintf(ERROR_STREAM, RED "Register NULL\n" CLEAN);
        fprintf(ERROR_STREAM, MAGENTA "\n- - - SPU dumping end - - -\n\n" CLEAN);
        return;
    }
        fprintf(ERROR_STREAM, "Register:\n\n");
    for(size_t i = 0; i < register_size; i++)
    {
        fprintf(ERROR_STREAM, "  " PINK "[%lu]" CYAN " %d\n" CLEAN, i, processor->reg[i]);
    }
        fprintf(ERROR_STREAM, MAGENTA "\n- - - SPU dumping end - - -\n\n" CLEAN);
}

int SPUVerify(SPU* processor)
{
    processor->err_code = SPU_CORRECT;

    if(processor == NULL)                    return processor->err_code |= SPU_PROCESSOR_NULL;
    if(StackVerify(&processor->stack) != Verified)  processor->err_code |= SPU_STACK_ERROR;
    if(processor->buffer == NULL)                   processor->err_code |= SPU_BUFFER_NULL;
    if(processor->reg == NULL)                      processor->err_code |= SPU_REG_NULL;
    return                                          processor->err_code;
}

void SPUDestroy(SPU* processor)
{
    if(processor == NULL) return;
    StackDestroy(&processor->stack);
    free(processor->reg);
    free(processor->buffer);
    memset(processor, 0, sizeof(SPU));
}

int IsError(int error, int check)
{
    return error & check;
}

void SPUErrorParser(int error)
{   
    if(IsError(error, SPU_HALT_STATE))          fprintf(ERROR_STREAM, "Error: SPU halted\n");
    if(IsError(error, SPU_DIVISION_BY_ZERO))    fprintf(ERROR_STREAM, "Error: division by zero\n");
    if(IsError(error, SPU_INVALID_COMMAND))     fprintf(ERROR_STREAM, "Error: invalid command\n");
    if(IsError(error, SPU_STACK_ERROR))         fprintf(ERROR_STREAM, "Error: stack error\n");
    if(IsError(error, SPU_PROCESSOR_NULL))      fprintf(ERROR_STREAM, "Error: processor NULL\n");
    if(IsError(error, SPU_REG_NULL))            fprintf(ERROR_STREAM, "Error: register NULL\n");
    if(IsError(error, SPU_BUFFER_NULL))         fprintf(ERROR_STREAM, "Error: buffer null\n");
    if(IsError(error, SPU_INVALID_REGISTER))    fprintf(ERROR_STREAM, "Error: invalid register\n");
}
