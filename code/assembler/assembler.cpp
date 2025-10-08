#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "assembler_read.h"
#include "../language.h"
#include "assembler_func.h"

#define ASS_MODE
#include "../commands.h"


int assemble        (Line* text, size_t count, FILE* out_file);
void error_parser   (int error);

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
    Line* text = NULL;

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

int assemble(Line* text, size_t count, FILE* out_file)
{
    if(text == NULL) return ASS_NULL_TEXT_POINTER;
    if(out_file == NULL) return ASS_NULL_OUTPUT_FILE;
    if(count == 0) return ASS_EMPTY_PROGRAMM;
    
    for(size_t i = 0; i < count; i++)
    {
        int found = 0;
        for(int j = 0; j < COMMANDS_COUNT; j++)
        {
            if(!strcmp(text[i].line, COMMANDS[j].name))
            {
                found = 1;
                int error = COMMANDS[j].ass_func(text, (int)i, out_file, j);
                if(error != ASS_CORRECT) return error;
            }
        }
        if(!found) return ASS_SYNTAX_ERROR;
    }

    if(text[count-1].line != NULL && strcmp(text[count-1].line, "HLT")) return ASS_HLT_NOT_FOUND;

    return ASS_CORRECT;
}

void error_parser(int error)
{
    switch (error)
    {
        case ASS_HLT_NOT_FOUND:{    fprintf(stderr, "HLT not found\n");             break;}
        case ASS_ARGUMENT_INVALID:{ fprintf(stderr, "Argument invalid\n");          break;}
        case ASS_EMPTY_PROGRAMM:{   fprintf(stderr, "Empty programm\n");            break;}
        case ASS_NULL_INPUT_FILE:{  fprintf(stderr, "Input file NULL\n");           break;}
        case ASS_NULL_TEXT_POINTER:{fprintf(stderr, "Text NULL\n");                 break;}
        case ASS_NULL_OUTPUT_FILE:{ fprintf(stderr, "Output file NULL\n");          break;}
        case ASS_SYNTAX_ERROR:{     fprintf(stderr, "Syntax error\n");              break;}
        default:{                   fprintf(stderr, "Unknown error: %d\n", error);  break;}
    }
}