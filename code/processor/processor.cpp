#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "stack.h"
#include "../language.h"
#include "../lib.h"
#include "../colors.h"
#include "../assembler/assembler_read.h"
#include "processor_functions.h"

#define SPU_MODE
#include "../commands.h"

int run             (SPU* processor);
int IsError         (int error, int check);
int SPUInit         (SPU* processor);
void SPUDestroy     (SPU* processor);
void SPUDump        (SPU* processor);
int SPUVerify       (SPU* processor);
void SPUErrorParser (int error);
void print_buffer(FILE* stream, SPU* processor);

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

    initialize_buffer(&spu_main.buffer, &spu_main.buffer_size, input_file);
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
        //SPUDump(&spu_main);
    }
    SPUDestroy(&spu_main);
    printf("SPU TURNING OFF...\n");
    return 0;
}

int run(SPU* processor)
{
    int inp = get_int(processor->buffer + processor->offcet, sizeof(COMMAND_TYPE));
    processor->offcet += sizeof(COMMAND_TYPE);

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
    processor->reg = (int*)calloc(REG_SIZE, sizeof(int));
    processor->offcet = 0;
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
        fprintf(ERROR_STREAM, "Offcet: %lu\n", processor->offcet);
        StackDump(&processor->stack);
    if(processor->reg == NULL)
    {
        fprintf(ERROR_STREAM, RED "Register NULL\n" CLEAN);
        fprintf(ERROR_STREAM, RED "\n- - - SPU dumping end - - -\n\n" CLEAN);
        return;
    }
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


void print_buffer(FILE* stream, SPU* processor)
{   
    for(size_t i = 0; i < processor->buffer_size; i++)
    {
        if(i != processor->offcet-1)  fprintf(stream, "%x ", (__uint8_t)*(processor->buffer+i));
        else    fprintf(stream, GREEN "%x " CLEAN, (__uint8_t)*(processor->buffer+i));
    }
}