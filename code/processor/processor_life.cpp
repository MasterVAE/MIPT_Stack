#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "processor_life.h"
#include "stack.h"
#include "../colors.h"
#include "../language.h"
#include "../lib.h"
#include "processor_functions.h"

void print_buffer(FILE* stream, SPU* processor);
int IsError(int error, int check);

int SPUInit(SPU* processor)
{
    if(processor == NULL) return 0;
    
    int error = StackInit(&processor->stack, 0);
    if(error != Verified)
    {
        processor->stack.err_code = error;
        return SPU_STACK_ERROR;
    }
    processor->offset = 0;
    processor->buffer_size = 0;
    processor->err_code = 0;
    return SPU_CORRECT;
}

void SPUDump(SPU* processor)
{
        fprintf(ERROR_STREAM, RED "\n\n- - - SPU dumping start - - -\n" CLEAN);
    SPUErrorParser(processor->err_code);
    if(processor == NULL)
    {
        fprintf(ERROR_STREAM, RED "Processor NULL\n" CLEAN); 
        fprintf(ERROR_STREAM, RED "\n- - - SPU dumping end - - -\n\n" CLEAN);
        return;
    }
    if(processor->buffer == NULL)
    {
        fprintf(ERROR_STREAM, "Buffer NULL\n");
    }
    else
    {
        fprintf(ERROR_STREAM, "Buffer:\n\n");
        print_buffer(ERROR_STREAM, processor);
        fprintf(ERROR_STREAM, "\n\n");
    }
        fprintf(ERROR_STREAM, "Offcet: %lu\n", processor->offset);
    StackDump(&processor->stack);
    
        fprintf(ERROR_STREAM, "Register:    ");
    for(size_t i = 0; i < REG_SIZE; i++)
    {
        fprintf(ERROR_STREAM, PINK "[%lu]" CYAN " %d " CLEAN, i, processor->reg[i]);
    }
        fprintf(ERROR_STREAM, RED "\n\n- - - SPU dumping end - - -\n\n" CLEAN);
}

int SPUVerify(SPU* processor)
{
    processor->err_code = SPU_CORRECT;

    if(processor == NULL)                    return processor->err_code |= SPU_PROCESSOR_NULL;
    if(StackVerify(&processor->stack) != Verified)  processor->err_code |= SPU_STACK_ERROR;
    if(processor->buffer == NULL)                   processor->err_code |= SPU_BUFFER_NULL;
    return                                          processor->err_code;
}

void SPUDestroy(SPU* processor)
{
    if(processor == NULL) return;
    StackDestroy(&processor->stack);
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

void print_buffer(FILE* stream, SPU* processor)
{   
    for(size_t i = 0; i < processor->buffer_size; i++)
    {//FIXME __uint8_t
        if(i != processor->offset)  fprintf(stream, "%x ", (__uint8_t)*(processor->buffer+i));
        else    fprintf(stream, GREEN "%x " CLEAN, (__uint8_t)*(processor->buffer+i));
    }
}