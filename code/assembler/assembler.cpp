#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "assembler_read.h"
#include "../language.h"
#include "../lib.h"
#include "assembler_func.h"

#define ASS_MODE
#include "../commands.h"


int assemble            (Assembler* ass);
void error_printer      (int error);
const char* error_parser(int error);

const char* input_file_name = "files/code.asm";
const char* output_file_name = "files/code.bcode";

int main(int argc, char *argv[])
{
    if(argc > 1)
    {
        input_file_name = argv[1];
        if(argc > 2)
        {
            output_file_name = argv[2];
        }
    }

    printf("Start compiling: %s -> %s\n", input_file_name, output_file_name);

    Assembler ass = {};
    ASSInit(&ass);
    
    FILE* input_file = fopen(input_file_name, "r");
    if(input_file == NULL) 
    {
        error_parser(ASS_NULL_INPUT_FILE);
        free(ass.buffer);
        return 1;
    }

    char* buffer = NULL;
    size_t size = 0;

    initialize_buffer(&buffer, &size, input_file);
    fclose(input_file);   
    ass.lines_count = initialize_text(&ass.text, buffer, size);
    
    int error = assemble(&ass);
    free(buffer);
    if(error != ASS_CORRECT)
    { 
        error_parser(error);
        ASSDestroy(&ass);
        free(buffer);
        return 1;
    }

    FILE* output_file = fopen(output_file_name, "w");
    if(output_file == NULL)
    {
        error_parser(ASS_NULL_OUTPUT_FILE);
        ASSDestroy(&ass);
        return 1;
    }
    fwrite(ass.buffer, sizeof(char), ass.offset, output_file);
    fclose(output_file);

    
    ASSDestroy(&ass);
    printf("Success compiling: %s -> %s\n", input_file_name, output_file_name);
    return 0;
}

int assemble(Assembler* ass)
{
    if(ass == NULL) return ASS_ASSEMBLER_NULL;
    if(ass->text == NULL) return ASS_NULL_TEXT_POINTER;
    if(ass->buffer == NULL) return ASS_NULL_BUFFER_POINTER;
    if(ass->lines_count == 0) return ASS_EMPTY_PROGRAMM;
    
    for(ass->line_offset = 0; ass->line_offset < ass->lines_count; ass->line_offset++)
    {
        if(!strcmp(ass->text[ass->line_offset].line, "")) continue;

        if(ass->buffer_size - ass->offset < buffer_start_size)
        {
            ass->buffer_size *= buffer_size_mult;
            ass->buffer = (char*)realloc(ass->buffer, ass->buffer_size);
        }

        int found = 0;
        for(size_t j = 0; j < COMMANDS_COUNT; j++)
        {
            if(!strcmp(ass->text[ass->line_offset].line, COMMANDS[j].name))
            {
                found = 1;
                int error = COMMANDS[j].ass_func(ass, j);
                if(error != ASS_CORRECT)
                {
                    printf("ERROR AT LINE %s:%lu    %s\n",
                        input_file_name, ass->line_offset, ass->text[ass->line_offset].line);
                    return error;
                }
            }
        }
        if(!found ) return ASS_UNKNOWN_COMMAND;
    }
    return ASS_CORRECT;
}
//FIXME в функцию

void error_printer(int error)
{
    fprintf(stderr, "%s\n", error_parser(error));
}

const char* error_parser(int error)
{
    switch (error)
    {
        case ASS_CORRECT:               return "Correct";
        case ASS_ASSEMBLER_NULL:        return "Assembler NULL";
        case ASS_NULL_TEXT_POINTER:     return "NULL text pointet";
        case ASS_NULL_BUFFER_POINTER:   return "NULL buffer pointer";
        case ASS_EMPTY_PROGRAMM:        return "Empty programm";
        case ASS_UNKNOWN_COMMAND:       return "Unknown command";
        case ASS_ARGUMENT_INVALID:      return "Argument invalid";
        case ASS_NULL_INPUT_FILE:       return "Error opening input file";
        case ASS_NULL_OUTPUT_FILE:      return "Error opening output file";
        case ASS_SYNTAX_ERROR:          return "Syntax error";
        case ASS_USED_LABEL:            return "Label overriding";
        default:                        return "Unknown error";
    }
}