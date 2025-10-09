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

int     disassemble         (Disassembler* dis, FILE* out_file);
void    error_parser        (int error);

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
        error_parser(DIS_NULL_INPUT_FILE);
        DISDestroy(&dis);
        return 1;
    }

    initialize_buffer(&dis.buffer, &dis.buffer_size, input_file);

    fclose(input_file);

    FILE* output_file = fopen(output_file_name, "w");
    if(output_file == NULL)
    {
        error_parser(DIS_NULL_OUTPUT_FILE);
        DISDestroy(&dis);
        return 1;
    }

    int error = disassemble(&dis, output_file);

    fclose(output_file);
    DISDestroy(&dis);

    if(error != DIS_CORRECT)
    { 
        error_parser(error);
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
            return DIS_SYNTAX_ERROR;
        }
    }
    return DIS_CORRECT;
}

void error_parser(int error)
{
    switch (error)
    {
        case DIS_PUSH_ARGUMENT_INVALID:
        {
            fprintf(stderr, "PUSH argument invalid\n");
            break;
        }
        case DIS_EMPTY_PROGRAMM:
        {
            fprintf(stderr, "Empty programm\n");
            break;
        }
        case DIS_NULL_INPUT_FILE:
        {
            fprintf(stderr, "Input file NULL\n");
            break;
        }
        case DIS_NULL_BUFFER:
        {
            fprintf(stderr, "Buffer NULL\n");
            break;
        }
        case DIS_NULL_OUTPUT_FILE:
        {
            fprintf(stderr, "Output file NULL\n");
            break;
        }
        case DIS_SYNTAX_ERROR:
        {
            fprintf(stderr, "Syntax error\n");
            break;
        }
        default:
        {
            fprintf(stderr, "Unknown error: %d\n", error);
            break;
        }
    }
}

