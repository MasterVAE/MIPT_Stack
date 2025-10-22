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

#define OPEN(file_ptr, file_name, type) FILE* file_ptr = fopen(file_name, type); \
        if(file_ptr == NULL) {  SPUDestroy(&spu_main);  return 1;}
#define CLOSE(file) fclose(file);

const char* input_filename = "files/code.bcode";

int main()
{
    printf("SPU STARTING...\n");

    SPU spu_main = {};
    int error = SPUInit(&spu_main);
    spu_main.err_code = error;
    if(error != SPU_CORRECT) SPUDump(&spu_main);

    OPEN(input_file, input_filename, "r");
    spu_main.command_buffer = InitializeBuffer(&spu_main.command_buffer_size, input_file);
    CLOSE(input_file);

    SPUCycle(&spu_main);

    SPUDestroy(&spu_main);
    printf("SPU TURNING OFF...\n");

    return 0;
}