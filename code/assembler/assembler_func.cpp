#include <stdio.h>
#include <string.h>

#include "assembler_func.h"
#include "../language.h"
#include "../commands.h"

#define REG_CMP(reg, num) if(!strcmp(arg, reg)) {bytecode_value(out_file, num); return SPU_CORRECT;}

int bytecode_comm(FILE* output_file, int command);
int bytecode_value(FILE* output_file, int command);

int ass_halt(Line*, int, FILE* out_file, int my_ind) 
{
    bytecode_comm(out_file, COMMANDS[my_ind].num);
    return ASS_CORRECT;
}

int ass_add(Line*, int, FILE* out_file, int my_ind)
{
    bytecode_comm(out_file, COMMANDS[my_ind].num);
    return ASS_CORRECT;
}

int ass_sub(Line*, int, FILE* out_file, int my_ind)
{
    bytecode_comm(out_file, COMMANDS[my_ind].num);
    return ASS_CORRECT;
}

int ass_mul(Line*, int, FILE* out_file, int my_ind)
{
    bytecode_comm(out_file, COMMANDS[my_ind].num);
    return ASS_CORRECT;
}

int ass_div(Line*, int, FILE* out_file, int my_ind)
{
    bytecode_comm(out_file, COMMANDS[my_ind].num);
    return ASS_CORRECT;
}

int ass_sqrt(Line*, int, FILE* out_file, int my_ind)
{
    bytecode_comm(out_file, COMMANDS[my_ind].num);
    return ASS_CORRECT;
}

int ass_out(Line*, int, FILE* out_file, int my_ind)
{
    bytecode_comm(out_file, COMMANDS[my_ind].num);
    return ASS_CORRECT;
}
int ass_in(Line*, int, FILE* out_file, int my_ind)
{
    bytecode_comm(out_file, COMMANDS[my_ind].num);
    return ASS_CORRECT;
}
int ass_pushr(Line* text, int list_ind, FILE* out_file, int my_ind)
{
    char* arg = text[list_ind].args[0];
    if(arg == NULL) return ASS_ARGUMENT_INVALID;

    bytecode_comm(out_file, COMMANDS[my_ind].num);
    REG_CMP("SR1", 0);
    REG_CMP("SR2", 1);
    REG_CMP("SR3", 2);
    REG_CMP("SR4", 3);
    REG_CMP("SR5", 4);
    REG_CMP("SR6", 5);
    REG_CMP("SR7", 6);
    REG_CMP("SR8", 7);
    return ASS_ARGUMENT_INVALID;
}
int ass_popr(Line* text, int list_ind, FILE* out_file, int my_ind)
{
    char* arg = text[list_ind].args[0];
    if(arg == NULL) return ASS_ARGUMENT_INVALID;

    bytecode_comm(out_file, COMMANDS[my_ind].num);
    REG_CMP("SR1", 0);
    REG_CMP("SR2", 1);
    REG_CMP("SR3", 2);
    REG_CMP("SR4", 3);
    REG_CMP("SR5", 4);
    REG_CMP("SR6", 5);
    REG_CMP("SR7", 6);
    REG_CMP("SR8", 7);
    return ASS_ARGUMENT_INVALID;
}
int ass_push(Line* text, int line_ind, FILE* out_file, int my_ind)
{
    int value = 0;
    char* arg = NULL;
    int commands = 0;
    while((arg = text[line_ind].args[commands]) != NULL)
    {   
        if(sscanf(arg, "%10d", &value))
        { 
            bytecode_comm(out_file, COMMANDS[my_ind].num); 
            bytecode_value(out_file, value); 
            commands++;
        }
        else
        {
            return ASS_ARGUMENT_INVALID;
        }
    }
    if(commands == 0)
    {
        return ASS_ARGUMENT_INVALID;
    } 
    return ASS_CORRECT;
}


int bytecode_comm(FILE* output_file, int command)
{
    if(output_file == NULL) return ASS_NULL_OUTPUT_FILE;
    fwrite(&command, sizeof(__uint8_t), 1, output_file);
    return ASS_CORRECT;
}

int bytecode_value(FILE* output_file, int value)
{
    if(output_file == NULL) return ASS_NULL_OUTPUT_FILE;
    fwrite(&value, sizeof(__uint32_t), 1, output_file);
    return ASS_CORRECT;
}


