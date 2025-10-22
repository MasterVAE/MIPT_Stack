#include <stdio.h>
#include <stdlib.h>

#include "../constants.h"
#include "../lib.h"
#include "../colors.h"

#define DIS_MODE
#include "../commands.h"

#include "disassembler_manager.h"
#include "disassembler_func.h"

#define OPEN(file_ptr, file_name, type) FILE* file_ptr = fopen(file_name, type); \
    if(file_ptr == NULL) {  ErrorPrinter(DIS_NULL_FILE); DISDestroy(&dis);   return 1;}

#define CLOSE(file_ptr) fclose(file_ptr);

#define CHECK(error) if(error != DIS_CORRECT){ ErrorPrinter(error);DISDestroy(&dis);return 1;}

const char* input_file_name = "files/code.bcode";
const char* output_file_name = "files/code_disassembled.asm";

int main(int argc, char *argv[])
{
    if(argc >= 2) input_file_name = argv[1];
    if(argc >= 3) output_file_name = argv[2];

    Disassembler dis = {};
    DISInit(&dis);

    printf("Start disassembling: %s -> %s\n", input_file_name, output_file_name);

    OPEN(input_file, input_file_name, "r")

    dis.buffer = InitializeBuffer(&dis.buffer_size, input_file);

    fclose(input_file);

    DisErr_t error = LabelSearch(&dis);
    CHECK(error)
    
    error = Disassemble(&dis);
    CHECK(error)

    OPEN(output_file, output_file_name, "w+");
    fwrite(dis.output_buffer, sizeof(dis.output_buffer[0]), dis.output_buffer_size, output_file);
    CLOSE(output_file);

    DISDestroy(&dis);

    printf(GREEN "Success disassembling: " CLEAN "%s -> %s\n", input_file_name, output_file_name);

    return 0;
}