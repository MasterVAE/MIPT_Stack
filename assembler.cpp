#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "code/assembler_read.h"
#include "code/language.h"

int assemble(char** text, size_t count, FILE* out_file);
void error_parser(int error);
int bytecode_comm(FILE* output_file, int command);
int bytecode_value(FILE* output_file, int value);
int command_parse(char** line, size_t line_ind, FILE* out_file);


int main(int argc, char *argv[])
{
    const char* input_file_name = "files/code.asm";
    const char* output_file_name = "files/code.bcode";
    if(argc >= 2)
    {
        input_file_name = argv[1];
        if(argc >= 3)
        {
            output_file_name = argv[2];
        }
    }

    printf("Start compiling: %s -> %s\n", input_file_name, output_file_name);

    FILE* input_file = fopen(input_file_name, "r");
    if(input_file == NULL) 
    {
        error_parser(ASS_NULL_INPUT_FILE);
        return 1;
    }

    char* buffer = NULL;
    char** text = NULL;

    size_t size = 0;
    size_t count = 0;

    initialize_buffer(&buffer, &size, input_file);
    count = initialize_text(&text, buffer, size);

    fclose(input_file);
    
    FILE* output_file = fopen(output_file_name, "w");
    if(output_file == NULL)
    {
        error_parser(ASS_NULL_OUTPUT_FILE);
        return 1;
    }

    int error = assemble(text, count, output_file);

    fclose(output_file);

    free(buffer);
    free(text);

    if(error != ASS_CORRECT)
    { 
        error_parser(error);
        return 1;
    }
    printf("Success compiling: %s -> %s\n", input_file_name, output_file_name);
}


int assemble(char** text, size_t count, FILE* out_file)
{
    if(text == NULL) return ASS_NULL_TEXT_POINTER;
    if(out_file == NULL) return ASS_NULL_OUTPUT_FILE;
    if(count == 0) return ASS_EMPTY_PROGRAMM;

    
    for(size_t i = 0; i < count; i++)
    {
        int error = command_parse(text, i, out_file);
        if(error != ASS_CORRECT) return error;
    }

    if(text[(count-1) * (arg_limit + 1)] != NULL && strcmp(text[(count-1) * (arg_limit + 1)], "HLT")) return ASS_HLT_NOT_FOUND;

    return ASS_CORRECT;
}

int command_parse(char** line, size_t line_ind, FILE* out_file)
{
    if(line == NULL) return ASS_NULL_TEXT_POINTER;
    if(out_file == NULL) return ASS_NULL_OUTPUT_FILE;

    size_t commands = 0;

    char* comm = NULL;
    while((comm = line[line_ind * (arg_limit + 1) + commands]) != NULL)
    {
        if(commands > 0) return ASS_TO_MUCH_ARGUMENT;

        if(!strcmp(comm, "HLT")) bytecode_comm(out_file, HLT);
        else if(!strcmp(comm, "ADD")) bytecode_comm(out_file, ADD);
        else if(!strcmp(comm, "SUB")) bytecode_comm(out_file, SUB);
        else if(!strcmp(comm, "MUL")) bytecode_comm(out_file, MUL);
        else if(!strcmp(comm, "DIV")) bytecode_comm(out_file, DIV);
        else if(!strcmp(comm, "SQRT")) bytecode_comm(out_file, SQRT);
        else if(!strcmp(comm, "OUT")) bytecode_comm(out_file, OUT);
        else if(!strcmp(comm, "PUSH"))
        {
            int value = 0;
            char* arg = NULL;
            while((arg = line[line_ind * (arg_limit + 1) + 1 + commands]) != NULL)
            {   
                if(sscanf(arg, "%10d", &value))
                { 
                    bytecode_comm(out_file, PUSH); 
                    bytecode_value(out_file, value); 
                    commands++;
                }
                else
                {
                    return ASS_PUSH_ARGUMENT_INVALID;
                }
            }
            if(commands == 0)
            {
                return ASS_PUSH_ARGUMENT_INVALID;
            }    
        }
        else return ASS_SYNTAX_ERROR;
        commands++;
    }  
    return ASS_CORRECT;
}

void error_parser(int error)
{
    switch (error)
    {
        case ASS_HLT_NOT_FOUND:
        {
            fprintf(stderr, "HLT not found\n");
            break;
        }
        case ASS_PUSH_ARGUMENT_INVALID:
        {
            fprintf(stderr, "PUSH argument invalid\n");
            break;
        }
        case ASS_EMPTY_PROGRAMM:
        {
            fprintf(stderr, "Empty programm\n");
            break;
        }
        case ASS_NULL_INPUT_FILE:
        {
            fprintf(stderr, "Input file NULL\n");
            break;
        }
        case ASS_NULL_TEXT_POINTER:
        {
            fprintf(stderr, "Text NULL\n");
            break;
        }
        case ASS_NULL_OUTPUT_FILE:
        {
            fprintf(stderr, "Output file NULL\n");
            break;
        }
        case ASS_SYNTAX_ERROR:
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


int bytecode_comm(FILE* output_file, int command)
{
    if(output_file == NULL) return ASS_NULL_OUTPUT_FILE;

    char bytecode[command_size + 1] = {};
    for(int i = command_size - 1; i >= 0; i--)
    {
        bytecode[i] = '0'+ (char)command%2;
        command/=2;
    }
    fprintf(output_file, "%s", bytecode);
    
    return ASS_CORRECT;
}

int bytecode_value(FILE* output_file, int value)
{
    if(output_file == NULL) return ASS_NULL_OUTPUT_FILE;

    char bytecode[value_size + 1] = {};
    for(int i = value_size - 1; i >= 0; i--)
    {
        bytecode[i] = '0'+ (char)value%2;
        value/=2;
    }
    fprintf(output_file, "%s", bytecode);
    
    return ASS_CORRECT;
}
