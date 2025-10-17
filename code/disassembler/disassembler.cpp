#include <stdio.h>
#include <stdlib.h>

#include "../language.h"
#include "../lib.h"
#include "../colors.h"

#define DIS_MODE
#include "../commands.h"

#include "disassembler_life.h"
#include "disassembler_func.h"

#define OPEN(file_ptr, file_name, type) FILE* file_ptr = fopen(file_name, type); \
    if(file_ptr == NULL) {  error_printer(DIS_NULL_FILE); DISDestroy(&dis);   return 1;}

#define ARG_PARSE if(argc >= 2) {   input_file_name = argv[1];if(argc >= 3) \
    output_file_name = argv[2];}

#define CHECK(error) if(error != DIS_CORRECT){ error_printer(error);DISDestroy(&dis);return 1;}

dis_err disassemble(Disassembler* dis, FILE* out_file);

const char* input_file_name = "files/code.bcode";
const char* output_file_name = "files/code_disassembled.asm";

int main(int argc, char *argv[])
{
    ARG_PARSE

    Disassembler dis = {};
    DISInit(&dis);

    printf("Start disassembling: %s -> %s\n", input_file_name, output_file_name);

    OPEN(input_file, input_file_name, "r")

    initialize_buffer(&dis.buffer, &dis.buffer_size, input_file);

    fclose(input_file);

    OPEN(output_file, output_file_name, "w+")

    dis_err error = label_search(&dis);
    CHECK(error)
    
    error = disassemble(&dis, output_file);
    CHECK(error)

    fclose(output_file);
    DISDestroy(&dis);


    printf(GREEN "Success disassembling: " CLEAN "%s -> %s\n", input_file_name, output_file_name);
    return 0;
}

dis_err disassemble(Disassembler* dis, FILE* out_file)
{
    if(dis->buffer == NULL) return DIS_NULL_BUFFER;
    if(out_file == NULL) return DIS_NULL_FILE;
    if(dis->buffer_size == 0) return DIS_EMPTY_PROGRAMM;

    for(dis->offset = 0; dis->offset < dis->buffer_size; dis->offset+=sizeof(command_type))
    {
        insert_label(dis, out_file);
        if(dis->offset + sizeof(command_type) > dis->buffer_size)   return DIS_SYNTAX_ERROR;
        int comm = debytecode_int(dis->buffer + dis->offset, sizeof(command_type));
        
        bool found = 0;
        for(size_t j = 0; j < COMMANDS_COUNT; j++)
        {    
            if(COMMANDS[j].num == comm)
            {
                COMMANDS[j].dis_func(dis, j, out_file);
                found = 1;
                break;
            }
        }
        if(!found)  return DIS_UNKNOWN_COMMAND;
    }
    return DIS_CORRECT;
}

