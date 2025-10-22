#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "assembler_manager.h"
#include "../constants.h"
#include "../colors.h"
#include "../lib.h"
#include "assembler_func.h"

#define ASS_MODE
#include "../commands.h"

#define OPEN(file_ptr, file_name, type) FILE* file_ptr = fopen(file_name, type); \
        if(file_ptr == NULL) {  ErrorPrinter(ASS_NULL_FILE); ASSDestroy(&ass);  return 1;} \

#define CLOSE(file_ptr) fclose(file_ptr);

#define CHECK(error) if(error != ASS_CORRECT) {ErrorPrinter(error);ASSDestroy(&ass);return 1;}

const char* input_file_name = "files/code.asm";
const char* output_file_name = "files/code.bcode";

int main(int argc, char *argv[])
{
    if(argc >= 2) input_file_name = argv[1];
    if(argc >= 3) output_file_name = argv[2];

    printf("Start compiling: %s -> %s\n", input_file_name, output_file_name);
    Assembler ass = {};
    ASSErr_t error = ASSInit(&ass);
    CHECK(error);

    OPEN(input_file, input_file_name, "r");

    size_t input_buffer_size = 0;
    char* input_file_buffer = InitializeBuffer(&input_buffer_size, input_file);
    fclose(input_file);   
    ass.lines_count = InitializeText(&ass.text, input_file_buffer, input_buffer_size);
    
    error = Assemble(&ass);
    free(input_file_buffer);
    CHECK(error);

    error = ASSPostCompile(&ass);
    CHECK(error);

    OPEN(output_file, output_file_name, "w+");
    fwrite(ass.bin_buffer, sizeof(ass.bin_buffer[0]), ass.offset, output_file);
    CLOSE(output_file);
    
    ASSDestroy(&ass);
    printf(GREEN "Success compiling: " CLEAN "%s -> %s\n", input_file_name, output_file_name);

    return 0;
}