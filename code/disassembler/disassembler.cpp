#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "../language.h"
#include "../lib.h"

#define DIS_MODE
#include "../commands.h"

#include "disassembler_life.h"
#include "disassembler_func.h"

int disassemble(Disassembler* dis, FILE* out_file);
void error_printer(int error);
const char* error_parser(int error);

int main(int argc, char *argv[])
{
    const char* input_file_name = "files/code.bcode";
    const char* output_file_name = "files/code_disassembled.asm";
    if(argc >= 2)
    {
        input_file_name = argv[1];
        if(argc >= 3)
        {
            output_file_name = argv[2];
        }
    }

    Disassembler dis = {};
    DISInit(&dis);

    printf("Start disassembling: %s -> %s\n", input_file_name, output_file_name);

    FILE* input_file = fopen(input_file_name, "r");
    if(input_file == NULL) 
    {
        error_printer(DIS_NULL_INPUT_FILE);
        DISDestroy(&dis);
        return 1;
    }

    initialize_buffer(&dis.buffer, &dis.buffer_size, input_file);

    fclose(input_file);

    FILE* output_file = fopen(output_file_name, "w");
    if(output_file == NULL)
    {
        error_printer(DIS_NULL_OUTPUT_FILE);
        DISDestroy(&dis);
        return 1;
    }

    int error = disassemble(&dis, output_file);

    fclose(output_file);
    DISDestroy(&dis);

    if(error != DIS_CORRECT)
    { 
        error_printer(error);
        return 1;
    }

    printf("End disassembling: %s -> %s\n", input_file_name, output_file_name);
    return 0;
}

int disassemble(Disassembler* dis, FILE* out_file)
{
    if(dis->buffer == NULL) return DIS_NULL_BUFFER;
    if(out_file == NULL) return DIS_NULL_OUTPUT_FILE;
    if(dis->buffer_size == 0) return DIS_EMPTY_PROGRAMM;

    for(dis->offset = 0; dis->offset < dis->buffer_size; dis->offset+=sizeof(COMMAND_TYPE))
    {
        if(dis->offset + sizeof(COMMAND_TYPE) > dis->buffer_size)
        {
            return DIS_SYNTAX_ERROR;
        }
        int comm = debytecode(dis->buffer + dis->offset, sizeof(COMMAND_TYPE));
        int found = 0;
        for(size_t j = 0; j < COMMANDS_COUNT; j++)
        {    
            if(COMMANDS[j].num == comm)
            {
                COMMANDS[j].dis_func(dis, j, out_file);
                found = 1;
                break;
            }
        }
        if(!found)
        {
            return DIS_UNKNOWN_COMMAND;
        }
    }
    return DIS_CORRECT;
}

void error_printer(int error)
{
    fprintf(stderr, "%s\n", error_parser(error));
}

const char* error_parser(int error)
{
    switch (error)
    {
        case DIS_CORRECT:           return "Correct";
        case DIS_NULL_DISASSEMBLER: return "Disassembler NULL";
        case DIS_NULL_BUFFER:       return "Buffer NULL";
        case DIS_NULL_INPUT_FILE:   return "Error opening input file";
        case DIS_NULL_OUTPUT_FILE:  return "Error opening outpur file";
        case DIS_EMPTY_PROGRAMM:    return "Empty programm";
        case DIS_UNKNOWN_COMMAND:   return "Unknown command";
        case DIS_SYNTAX_ERROR:      return "Syntax error";
        case DIS_ARGUMENT_INVALID:  return "Argument invalid";
        default:                    return "Unknown error";
    }
}

