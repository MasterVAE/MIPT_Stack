#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "assembler_read.h"
#include "../language.h"
#include "../colors.h"
#include "../lib.h"
#include "assembler_func.h"

#define ASS_MODE
#include "../commands.h"

#define ARG_PARSE if(argc >= 2) {   input_file_name = argv[1];if(argc >= 3) \
    output_file_name = argv[2];}

#define OPEN_R(file_ptr, file_name) FILE* file_ptr = fopen(file_name, "r"); \
    if(file_ptr == NULL) {  error_printer(ASS_NULL_FILE); ASSDestroy(&ass);  return 1;}

#define OPEN_W(file_ptr, file_name) FILE* file_ptr = fopen(file_name, "r"); \
    if(file_ptr == NULL) {  error_printer(ASS_NULL_FILE); ASSDestroy(&ass);  return 1;}

#define CHECK(error) if(error != ASS_CORRECT) {error_printer(error);ASSDestroy(&ass);return 1;}


int assemble(Assembler* ass);

const char* input_file_name = "files/code.asm";
const char* output_file_name = "files/code.bcode";

int main(int argc, char *argv[])
{
    ARG_PARSE

    printf("Start compiling: %s -> %s\n", input_file_name, output_file_name);
    //FIXME последняя команда с аргументом
    Assembler ass = {};
    ASSInit(&ass);

    OPEN_R(input_file, input_file_name)

    char* buffer = NULL;
    size_t size = 0;

    initialize_buffer(&buffer, &size, input_file);
    fclose(input_file);   
    ass.lines_count = initialize_text(&ass.text, buffer, size);
    
    int error = assemble(&ass);
    free(buffer);
    CHECK(error)

    error = ASSPostCompile(&ass);
    CHECK(error)

    OPEN_W(output_file, output_file_name)

    fwrite(ass.buffer, sizeof(char), ass.offset, output_file);
    fclose(output_file);
    
    ASSDestroy(&ass);
    printf(GREEN "Success compiling: " CLEAN "%s -> %s\n", input_file_name, output_file_name);
    return 0;
}

int assemble(Assembler* ass)
{
    if(ass == NULL)             return ASS_ASSEMBLER_NULL;
    if(ass->text == NULL)       return ASS_NULL_TEXT_POINTER;
    if(ass->buffer == NULL)     return ASS_NULL_BUFFER_POINTER;
    if(ass->lines_count == 0)   return ASS_EMPTY_PROGRAMM;
    
    for(ass->line_offset = 0; ass->line_offset < ass->lines_count; ass->line_offset++)
    {
        if(!strcmp(ass->text[ass->line_offset].line, "")) continue;

        if(ass->buffer_size - ass->offset < buffer_start_size)
        {
            ass->buffer_size *= buffer_size_mult;
            ass->buffer = (char*)realloc(ass->buffer, ass->buffer_size);
        }

        bool found = 0;
        for(size_t j = 0; j < COMMANDS_COUNT; j++)
        {
            if(!strcmp(ass->text[ass->line_offset].line, COMMANDS[j].name))
            {
                found = 1;
                int error = COMMANDS[j].ass_func(ass, j);
                if(error != ASS_CORRECT)
                {
                    printf(RED "ERROR " CLEAN "%s:%lu    %s\n",
                        input_file_name, ass->line_offset, ass->text[ass->line_offset].line);
                    return error;
                }
            }
        }
        if(!found) return ASS_UNKNOWN_COMMAND;
    }
    return ASS_CORRECT;
}

