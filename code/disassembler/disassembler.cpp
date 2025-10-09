#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "../language.h"
#include "../commands.h"

int     disassemble         (char* buffer, size_t size, FILE* out_file);
void    error_parser        (int error);
int     debytecode          (char* code, size_t size);
void    initialize_buffer   (char** buffer, size_t* size, FILE* input_file);
size_t  file_len            (FILE* file);

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

    printf("Start disassembling: %s -> %s\n", input_file_name, output_file_name);

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

    printf("End disassembling: %s -> %s\n", input_file_name, output_file_name);
    return 0;
}

const char* regs[] = 
{
    "SR1",
    "SR2",
    "SR3",
    "SR4",
    "SR5",
    "SR6",
    "SR7",
    "SR8"
};

int disassemble(char* buffer, size_t size, FILE* out_file)
{
    if(buffer == NULL) return DIS_NULL_BUFFER;
    if(out_file == NULL) return DIS_NULL_OUTPUT_FILE;
    if(size == 0) return DIS_EMPTY_PROGRAMM;

    size_t i = 0;
    while(i < size)
    {
        if(i + sizeof(COMMAND_TYPE) > size)
        {
            return DIS_SYNTAX_ERROR;
        }
        int comm = debytecode(buffer + i, sizeof(COMMAND_TYPE));
        int found = 0;
        for(size_t j = 0; j < COMMANDS_COUNT; j++)
        {    
            if(COMMANDS[j].num == comm)
            {
                if(!strcmp(COMMANDS[j].name, "PUSH"))
                {
                    if(i + sizeof(COMMAND_TYPE) + sizeof(VALUE_TYPE) > size)
                    {
                        return DIS_PUSH_ARGUMENT_INVALID;
                    }
                    int value = debytecode(buffer + i + sizeof(COMMAND_TYPE), sizeof(VALUE_TYPE));
                    i += sizeof(VALUE_TYPE);
                    fprintf(out_file, "PUSH %d\n", value);
                }
                else if(!strcmp(COMMANDS[j].name, "PUSHR"))
                {
                    if(i + sizeof(COMMAND_TYPE) + sizeof(VALUE_TYPE) > size)
                    {
                        return DIS_PUSH_ARGUMENT_INVALID;
                    }
                    int value = debytecode(buffer + i + sizeof(COMMAND_TYPE), sizeof(VALUE_TYPE));
                    i += sizeof(VALUE_TYPE);
                    fprintf(out_file, "PUSHR %s\n", regs[value]);
                }
                else if(!strcmp(COMMANDS[j].name, "POPR"))
                {
                    if(i + sizeof(COMMAND_TYPE) + sizeof(VALUE_TYPE) > size)
                    {
                        return DIS_PUSH_ARGUMENT_INVALID;
                    }
                    int value = debytecode(buffer + i + sizeof(COMMAND_TYPE), sizeof(VALUE_TYPE));
                    i += sizeof(VALUE_TYPE);
                    fprintf(out_file, "POPR %s\n", regs[value]);
                }
                else fprintf(out_file, "%s\n", COMMANDS[j].name);
                found = 1;
                break;
            }
        }
        if(!found)
        {
            return DIS_SYNTAX_ERROR;
        }
        i += sizeof(COMMAND_TYPE);
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

int debytecode(char* code, size_t size)
{
    int value;
    memcpy(&value, code, size);
    return value;
}

size_t file_len(FILE* file)
{
    struct stat file_info;
    fstat(fileno(file), &file_info);
    return (size_t)file_info.st_size;
}


void initialize_buffer(char** buffer, size_t* size, FILE* input_file)
{
    assert(buffer != NULL);
    assert(size != NULL);
    assert(input_file != NULL);

    *size = file_len(input_file);

    char* buff = (char*)calloc(*size + 1, sizeof(char));
    size_t len = fread(buff, sizeof(char), *size, input_file);

    buff[len] = '\0';

    *size = len;
    *buffer = buff;
}
