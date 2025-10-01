#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>


#include "code/assembler_read.h"


void assemble(char** text, size_t count, FILE* out_file);

enum func_bycodes
{
    HLT = 0,
    PUSH = 1,
    ADD = 2,
    SUB = 3,
    MUL = 4,
    DIV = 5, 
    SQRT = 6,
    OUT = 7
};

int main()
{
    FILE* input_file = fopen("files/code.asm", "r");
    assert(input_file != NULL);

    char* buffer = NULL;
    char** text = NULL;

    size_t size = file_len(input_file);
    size_t count = 0;

    initialize_buffer(&buffer, &size, input_file);
    count = initialize_text(&text, buffer, size);

    fclose(input_file);

    FILE* output_file = fopen("files/code.bcode", "w");
    assert(output_file != NULL);

    assemble(text, count, output_file);

    fclose(output_file);

    free(buffer);
    free(text);
}


void assemble(char** text, size_t count, FILE* out_file)
{
    assert(text != NULL);
    assert(out_file != NULL);

    char buff[11] = {};
    for(size_t i = 0; i < count; i++)
    {
        sscanf(text[i], "%10s", buff); //ограничение на ввод
        if(!strcmp(buff, "HLT")) fprintf(out_file, "%d\n", HLT);
        else if(!strcmp(buff, "ADD")) fprintf(out_file, "%d\n", ADD);
        else if(!strcmp(buff, "SUB")) fprintf(out_file, "%d\n", SUB);
        else if(!strcmp(buff, "MUL")) fprintf(out_file, "%d\n", MUL);
        else if(!strcmp(buff, "DIV")) fprintf(out_file, "%d\n", DIV);
        else if(!strcmp(buff, "SQRT")) fprintf(out_file, "%d\n", SQRT);
        else if(!strcmp(buff, "OUT")) fprintf(out_file, "%d\n", OUT);
        else if(!strcmp(buff, "PUSH"))
        {
            int value = 0;
            if(sscanf(text[i] + 4, "%10d", &value)) 
            {
                fprintf(out_file, "%d %d\n", PUSH, value); 
            }
            else
            {
                printf("Compilation error, gay\n");
            }
        }
    }
}
