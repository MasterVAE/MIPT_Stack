#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "code/stack.h"
#include "code/language.h"
#include "code/assembler_read.h"
#include "code/processor_functions.h"

#define POP_ERR(stack, err) StackPop(stack, err); if(*err != 0) StackDump(stack, *err);
#define INIT_ERR(stack, num) int err = StackInit(stack, num); if(err != 0) StackDump(stack, err);
#define PUSH_ERR(stack, value) {int error = StackPush(stack, value); if(error != 0) StackDump(stack, error);}

int run(SPU* processor);
int get_int(char* buffer, size_t len);


int main()
{
    printf("SPU STARTING...\n");
    SPU spu_main = {};
    INIT_ERR(&spu_main.stack, 0);
    
    

    FILE* input_file = fopen("files/code.bcode", "r");
    if(input_file == NULL)
    {
        StackDestroy(&spu_main.stack);
        return 1;
    }
    size_t size = 0;
    
    initialize_buffer(&spu_main.buffer, &size, input_file);
    fclose(input_file);

    //BREAKING STACK
    //((char*)stack.data)[2] = '7';

    size_t offcet = 0;
    while (1)
    {
        int error = run(&spu_main);
        if(error == SPU_HALT) break;
        else if(error == SPU_DIVISION_BY_ZERO) fprintf(stderr, "Interpretator error: DIVISION BY ZERO\n"); 
        else if(error == SPU_INVALID_COMMAND) fprintf(stderr, "Interpretator error: INVALID COMMAND\n"); 
    }
    StackDestroy(&spu_main.stack);
    free(spu_main.buffer);
    return 0;
}

int run(SPU* processor)
{
    int inp = get_int(processor->buffer + processor->offcet, command_size);
    processor->offcet += command_size;
    int err = 0;

    switch (inp)
    {
        case HLT: return SPU_HALT;
        case ADD: return SPU_ADD(processor);
        case SUB: return SPU_SUB(processor);
        case MUL: return SPU_MUL(processor);
        case DIV: return SPU_DIV(processor);
        case SQRT: return SPU_SQRT(processor);
        case PUSH: 
        {
            stack_type value = get_int(processor->buffer + processor->offcet, value_size);
            processor->offcet += value_size;
            return SPU_PUSH(processor, value);
        }
        case OUT: return SPU_OUT(processor);
        default: return SPU_INVALID_COMMAND;
    }
    return SPU_CORRECT;
}


int get_int(char* buffer, size_t len)
{
    int ans = 0;
    for(size_t i = 0; i < len; i++)
    {
        ans *= 2;
        ans += buffer[i] - '0';
    }
    return ans;
}