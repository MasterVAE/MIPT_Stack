#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h> 

#include "stack.h"
#include "../constants.h"
#include "../lib.h"
#include "../colors.h"
#include "processor_functions.h"

#include "processor_manager.h"

#define SPU_MODE
#include "../commands.h"

static int run(SPU* processor); 

const char* input_filename = "files/code.bcode";

int main()
{
    printf("SPU STARTING...\n");
    SPU spu_main = {};
    int error = SPUInit(&spu_main);
    spu_main.err_code = error;
    if(error != SPU_CORRECT) SPUDump(&spu_main);

    FILE* input_file = fopen(input_filename, "r");
    if(input_file == NULL)
    {
        StackDestroy(&spu_main.stack);
        return 1;
    }

    spu_main.command_buffer = InitializeBuffer(&spu_main.command_buffer_size, input_file);
    fclose(input_file);
    error = SPU_CORRECT;

    while(error != SPU_HALT_STATE)
    {
        error = run(&spu_main);
        if(error == SPU_HALT_STATE) break;
        error |= SPUVerify(&spu_main);
        if(error != SPU_CORRECT)
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

static int run(SPU* processor)
{
    int error = SPUVerify(processor);
    if(error != SPU_CORRECT) return error;
    
    int inp = DebytecodeInt(processor->command_buffer + processor->command_pointer, sizeof(command_type));

    for(size_t i = 0; i < COMMANDS_COUNT; i++)
    {
        if(inp == COMMANDS[i].num)
        {    
            processor->command_pointer += sizeof(command_type);
            return COMMANDS[i].SpuFunc(processor);
        }
    }
    return SPU_INVALID_COMMAND;
}