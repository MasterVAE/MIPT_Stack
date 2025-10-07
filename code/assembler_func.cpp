#include <stdio.h>
#include <string.h>

#include "assembler_func.h"
#include "language.h"
#include "commands.h"
#include "processor_functions.h"

#define REG_CMP(reg, num) if(!strcmp(arg, reg)) {bytecode_value(out_file, num); return SPU_CORRECT;}

int bytecode_comm(FILE* output_file, int command);
int bytecode_value(FILE* output_file, int command);

int ASS_HALT    (Line*, int, FILE* out_file, int my_ind) 
{
    bytecode_comm(out_file, COMMANDS[my_ind].num);
    return ASS_CORRECT;
}

int ASS_ADD     (Line*, int, FILE* out_file, int my_ind)
{
    bytecode_comm(out_file, COMMANDS[my_ind].num);
    return ASS_CORRECT;
}

int ASS_SUB     (Line*, int, FILE* out_file, int my_ind)
{
    bytecode_comm(out_file, COMMANDS[my_ind].num);
    return ASS_CORRECT;
}

int ASS_MUL     (Line*, int, FILE* out_file, int my_ind)
{
    bytecode_comm(out_file, COMMANDS[my_ind].num);
    return ASS_CORRECT;
}

int ASS_DIV     (Line*, int, FILE* out_file, int my_ind)
{
    bytecode_comm(out_file, COMMANDS[my_ind].num);
    return ASS_CORRECT;
}

int ASS_SQRT    (Line*, int, FILE* out_file, int my_ind)
{
    bytecode_comm(out_file, COMMANDS[my_ind].num);
    return ASS_CORRECT;
}

int ASS_OUT     (Line*, int, FILE* out_file, int my_ind)
{
    bytecode_comm(out_file, COMMANDS[my_ind].num);
    return ASS_CORRECT;
}
int ASS_IN      (Line*, int, FILE* out_file, int my_ind)
{
    bytecode_comm(out_file, COMMANDS[my_ind].num);
    return ASS_CORRECT;
}
int ASS_PUSHR   (Line* text, int list_ind, FILE* out_file, int my_ind)
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
int ASS_POPR    (Line* text, int list_ind, FILE* out_file, int my_ind)
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
int ASS_PUSH    (Line* text, int line_ind, FILE* out_file, int my_ind)
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

    char bytecode[command_size + 2] = {};
    for(int i = command_size - 1; i >= 0; i--)
    {
        bytecode[i] = '0'+ (char)command%2;
        command/=2;
    }
    //bytecode[command_size] = ' ';
    fprintf(output_file, "%s", bytecode);
    
    return ASS_CORRECT;
}

int bytecode_value(FILE* output_file, int value)
{
    if(output_file == NULL) return ASS_NULL_OUTPUT_FILE;

    unsigned int value_u = (unsigned)value;
    char bytecode[value_size + 2] = {};
    for(int i = value_size - 1; i >= 0; i--)
    {
        bytecode[i] = '0'+ (unsigned char)value_u%2;
        value_u/=2;
    }
    //bytecode[value_size] = ' ';
    fprintf(output_file, "%s", bytecode);
    
    return ASS_CORRECT;
}


