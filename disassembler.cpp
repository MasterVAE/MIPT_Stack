#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "code/assembler_read.h"
#include "code/language.h"

int disassemble(char* buffer, size_t size, FILE* out_file);
void error_parser(int error);
int disbytecode(char* buffer, size_t size);

enum disassembler_errors
{
    DIS_CORRECT = 0, 
    DIS_PUSH_ARGUMENT_INVALID,
    DIS_EMPTY_PROGRAMM,
    DIS_NULL_INPUT_FILE,
    DIS_NULL_OUTPUT_FILE,
    DIS_NULL_BUFFER,
    DIS_SYNTAX_ERROR 
};

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

    FILE* input_file = fopen(input_file_name, "r");
    if(input_file == NULL) 
    {
        error_parser(DIS_NULL_INPUT_FILE);
        return 1;
    }

    char* buffer = NULL;

    size_t size = 0;

    initialize_buffer(&buffer, &size, input_file);

    fclose(input_file);

    FILE* output_file = fopen(output_file_name, "w");
    if(output_file == NULL)
    {
        error_parser(DIS_NULL_OUTPUT_FILE);
        return 1;
    }

    int error = disassemble(buffer, size, output_file);

    fclose(output_file);

    free(buffer);

    if(error != DIS_CORRECT)
    { 
        error_parser(error);
        return 1;
    }
}


int disassemble(char* buffer, size_t size, FILE* out_file)
{
    if(buffer == NULL) return DIS_NULL_BUFFER;
    if(out_file == NULL) return DIS_NULL_OUTPUT_FILE;
    if(size == 0) return DIS_EMPTY_PROGRAMM;

    size_t i = 0;
    while(i < size)
    {
        if(i + command_size > size)
        {
            return DIS_SYNTAX_ERROR;
        }
        int comm = disbytecode(buffer + i, command_size);
        switch (comm)
        {
            case HLT:
            {
                fprintf(out_file, "HLT\n");
                break;
            }
            case ADD:
            {
                fprintf(out_file, "ADD\n");
                break;
            }
            case SUB:
            {
                fprintf(out_file, "SUB\n");
                break;
            }
            case MUL:
            {
                fprintf(out_file, "MUL\n");
                break;
            }
            case DIV:
            {
                fprintf(out_file, "DIV\n");
                break;
            }
            case SQRT:
            {
                fprintf(out_file, "SQRT\n");
                break;
            }
            case OUT:
            {
                fprintf(out_file, "OUT\n");
                break;
            }
            case PUSH:
            {
                if(i + command_size + value_size > size)
                {
                    return DIS_PUSH_ARGUMENT_INVALID;
                }
                int value = disbytecode(buffer + i + command_size, value_size);
                i += value_size;
                fprintf(out_file, "PUSH %d\n", value);

                break;
            }
            default:
            {
                return DIS_SYNTAX_ERROR;
            }
        }
        i += command_size;
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
            fprintf(stderr, "Inpout file NULL\n");
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


int disbytecode(char* buffer, size_t size)
{
    int ans = 0;
    for(size_t i = 0; i < size; i++)
    {
        ans *= 16;
        ans += buffer[i] - 'A';
    }
    return ans;
}
