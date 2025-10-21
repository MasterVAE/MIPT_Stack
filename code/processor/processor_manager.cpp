#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "stack.h"
#include "processor_manager.h"

#include "../colors.h"
#include "../constants.h"
#include "../lib.h"
#include "processor_functions.h"

#define FREE(var) free(var); var = NULL;

const char RAM_DEFAULT_VALUE = '_';

void PrintBuffer(FILE* stream, SPU* processor);
void RegisterDump(SPU* processor);
void BufferDump(SPU* processor);

int SPUInit(SPU* processor)
{
    if(processor == NULL) return 0;
    
    int error = StackInit(&processor->stack, 0);
    if(error != Verified)
    {
        processor->stack.err_code = error;
        return SPU_STACK_ERROR;
    }

    error = StackInit(&processor->return_stack, 0);
    if(error != Verified)
    {
        processor->return_stack.err_code = error;
        return SPU_STACK_ERROR;
    }

    processor->command_pointer = 0;
    processor->command_buffer_size = 0;
    processor->err_code = 0;
    processor->command_buffer = NULL;

    processor->ram = (char*)calloc(RAM_SIZE, sizeof(char));

    for(size_t i = 0; i < REG_SIZE; i++)   processor->reg[i] = 0;
    for(size_t i = 0; i < RAM_SIZE; i++)   processor->ram[i] = RAM_DEFAULT_VALUE;

    return SPU_CORRECT;
}

void BufferDump(SPU* processor)
{
        fprintf(ERROR_STREAM, RED "\n\n- - - BUFFER dumping - - -\n" CLEAN);
        fprintf(ERROR_STREAM, "OFFSET --> %lu\n\n", processor->command_pointer);
    if(processor->command_buffer == NULL)
    {
        fprintf(ERROR_STREAM, "Buffer NULL\n");
    }
    else
    {
        PrintBuffer(ERROR_STREAM, processor);
        fprintf(ERROR_STREAM, "\n\n");
    }
}

void RegisterDump(SPU* processor)
{
        fprintf(ERROR_STREAM, RED "\n\n- - - REGISTER dumping - - -\n" CLEAN);
    for(size_t i = 0; i < REG_SIZE; i++)
    {
        fprintf(ERROR_STREAM, PINK "[%s]" CYAN " %d " CLEAN, regs[i], processor->reg[i]);
    }
}

void SPUDump(SPU* processor)
{
        fprintf(ERROR_STREAM, RED "\n\n= = = SPU dumping start = = =\n" CLEAN);
    SPUStateorParser(processor->err_code);
    if(processor == NULL)
    {
        fprintf(ERROR_STREAM, RED "Processor NULL\n" CLEAN); 
        fprintf(ERROR_STREAM, RED "\n- - - SPU dumping end - - -\n\n" CLEAN);
        return;
    }
    BufferDump(processor);
    StackDump(&processor->stack);
    RegisterDump(processor);
    StackDump(&processor->return_stack);
        fprintf(ERROR_STREAM, RED "\n\n= = = SPU dumping end = = =\n\n" CLEAN);
}

int SPUVerify(SPU* processor)
{
    processor->err_code = SPU_CORRECT;

    if(processor == NULL)                    return processor->err_code |= SPU_PROCESSOR_NULL;
    if(StackVerify(&processor->stack) != Verified)  processor->err_code |= SPU_STACK_ERROR;
    if(processor->command_buffer == NULL)                   processor->err_code |= SPU_BUFFER_NULL;
    return                                          processor->err_code;
}

void SPUDestroy(SPU* processor)
{
    if(processor == NULL) return;
    StackDestroy(&processor->stack);
    StackDestroy(&processor->return_stack);

    FREE(processor->command_buffer)
    FREE(processor->ram)

    memset(processor, 0, sizeof(SPU));
}

void SPUStateorParser(int error)
{   
    if(IsError(error, SPU_HALT_STATE))          fprintf(ERROR_STREAM, "Error: SPU halted\n");
    if(IsError(error, SPU_DIVISION_BY_ZERO))    fprintf(ERROR_STREAM, "Error: division by zero\n");
    if(IsError(error, SPU_INVALID_COMMAND))     fprintf(ERROR_STREAM, "Error: invalid command\n");
    if(IsError(error, SPU_STACK_ERROR))         fprintf(ERROR_STREAM, "Error: stack error\n");
    if(IsError(error, SPU_PROCESSOR_NULL))      fprintf(ERROR_STREAM, "Error: processor NULL\n");
    if(IsError(error, SPU_REG_NULL))            fprintf(ERROR_STREAM, "Error: register NULL\n");
    if(IsError(error, SPU_BUFFER_NULL))         fprintf(ERROR_STREAM, "Error: buffer null\n");
    if(IsError(error, SPU_INVALID_REGISTER))    fprintf(ERROR_STREAM, "Error: invalid register\n");
    if(IsError(error, SPU_INVALID_RAM))         fprintf(ERROR_STREAM, "Error: invalid RAM adress\n");
}

void PrintBuffer(FILE* stream, SPU* processor)
{   
    for(size_t i = 0; i < processor->command_buffer_size; i++)
    {
        if(i < processor->command_pointer)
            fprintf(stream, BLUE "%s " CLEAN, itos((unsigned)*(processor->command_buffer + i), 16, 2).str);
        else if(i == processor->command_pointer)
            fprintf(stream, PINK "%s " CLEAN, itos((unsigned)*(processor->command_buffer + i), 16, 2).str);
        else
            fprintf(stream, "%s ", itos((unsigned)*(processor->command_buffer + i), 16, 2).str);
        if(i % BUFFER_CHARS_BY_LINE == BUFFER_CHARS_BY_LINE - 1)
            fprintf(stream, "\n");
    }
}