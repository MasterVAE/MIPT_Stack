#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "code/assembler_read.h"
#include "language.h"

int assemble(char** text, size_t count, FILE* out_file);
void error_parser(int error);
int bytecode_comm(FILE* output_file, int command);
int bytecode_value(FILE* output_file, int value);

enum assembler_errors
{
    ASS_CORRECT = 0, 
    ASS_HLT_NOT_FOUND,
    ASS_PUSH_ARGUMENT_INVALID,
    ASS_EMPTY_PROGRAMM,
    ASS_NULL_INPUT_FILE,
    ASS_NULL_TEXT_POINTER,
    ASS_NULL_OUTPUT_FILE,
    ASS_SYNTAX_ERROR 
};

int main()
{
    FILE* input_file = fopen("files/code.asm", "r");
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

    FILE* output_file = fopen("files/code.bcode", "w");
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
        exit(EXIT_FAILURE);
    }
}


int assemble(char** text, size_t count, FILE* out_file)
{
    if(text == NULL) return ASS_NULL_TEXT_POINTER;
    if(out_file == NULL) return ASS_NULL_OUTPUT_FILE;
    if(count == 0) return ASS_EMPTY_PROGRAMM;

    char buff[11] = {};
    for(size_t i = 0; i < count - 1; i++)
    {
        sscanf(text[i], "%10s", buff);
        if(!strcmp(buff, "HLT")) bytecode_comm(out_file, HLT);
        else if(!strcmp(buff, "ADD")) bytecode_comm(out_file, ADD);
        else if(!strcmp(buff, "SUB")) bytecode_comm(out_file, SUB);
        else if(!strcmp(buff, "MUL")) bytecode_comm(out_file, MUL);
        else if(!strcmp(buff, "DIV")) bytecode_comm(out_file, DIV);
        else if(!strcmp(buff, "SQRT")) bytecode_comm(out_file, SQRT);
        else if(!strcmp(buff, "OUT")) bytecode_comm(out_file, OUT);
        else if(!strcmp(buff, "PUSH"))
        {
            int value = 0;
            if(sscanf(text[i] + 4, "%10d", &value))
            { 
                bytecode_comm(out_file, PUSH); 
                bytecode_value(out_file, value); 
            }
            else
            {
                return ASS_PUSH_ARGUMENT_INVALID;
            }
        }
        else return ASS_SYNTAX_ERROR;
    }

    sscanf(text[count -  1], "%10s", buff);
    if(!strcmp(buff, "HLT")) bytecode_comm(out_file, HLT);
    else return ASS_HLT_NOT_FOUND;

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
            fprintf(stderr, "Inpout file NULL\n");
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

    char bytecode[9] = {};
    bytecode[0] = 'A' + (char)command/16;
    bytecode[1] = 'A' + (char)command%16;
    fprintf(output_file, "%s", bytecode);
    
    return ASS_CORRECT;
}

int bytecode_value(FILE* output_file, int value)
{
    if(output_file == NULL) return ASS_NULL_OUTPUT_FILE;

    char bytecode[9] = {};
    for(int i = 7; i >= 0; i--)
    {
        bytecode[i] = 'A'+ (char)value%16;
        value/=16;
    }
    fprintf(output_file, "%s", bytecode);
    
    return ASS_CORRECT;
}
