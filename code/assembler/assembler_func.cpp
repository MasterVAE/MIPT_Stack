#include <stdio.h>
#include <string.h>

#include "assembler_func.h"
#include "assembler_read.h"
#include "../language.h"

#define ASS_MODE
#include "../commands.h"

#define REG_CMP(reg, num) if(!strcmp(arg, reg)) return num;

int bytecode_comm(Assembler* asm_ptr, int command);
int bytecode_value(Assembler* asm_ptr, int command);
int reg_cmp(const char* arg);

int ass_halt(Line*, size_t, Assembler* asm_ptr, size_t my_ind) 
{
    bytecode_comm(asm_ptr, COMMANDS[my_ind].num);
    return ASS_CORRECT;
}

int ass_add(Line*, size_t, Assembler* asm_ptr, size_t my_ind)
{
    bytecode_comm(asm_ptr, COMMANDS[my_ind].num);
    return ASS_CORRECT;
}

int ass_sub(Line*, size_t, Assembler* asm_ptr, size_t my_ind)
{
    bytecode_comm(asm_ptr, COMMANDS[my_ind].num);
    return ASS_CORRECT;
}

int ass_mul(Line*, size_t, Assembler* asm_ptr, size_t my_ind)
{
    bytecode_comm(asm_ptr, COMMANDS[my_ind].num);
    return ASS_CORRECT;
}

int ass_div(Line*, size_t, Assembler* asm_ptr, size_t my_ind)
{
    bytecode_comm(asm_ptr, COMMANDS[my_ind].num);
    return ASS_CORRECT;
}

int ass_sqrt(Line*, size_t, Assembler* asm_ptr, size_t my_ind)
{
    bytecode_comm(asm_ptr, COMMANDS[my_ind].num);
    return ASS_CORRECT;
}

int ass_out(Line*, size_t, Assembler* asm_ptr, size_t my_ind)
{
    bytecode_comm(asm_ptr, COMMANDS[my_ind].num);
    return ASS_CORRECT;
}

int ass_in(Line*, size_t, Assembler* asm_ptr, size_t my_ind)
{
    bytecode_comm(asm_ptr, COMMANDS[my_ind].num);
    return ASS_CORRECT;
}

int ass_pushr(Line* text, size_t list_ind, Assembler* asm_ptr, size_t my_ind)
{
    char* arg = text[list_ind].args[0];
    if(arg == NULL) return ASS_ARGUMENT_INVALID;
    bytecode_comm(asm_ptr, COMMANDS[my_ind].num);
    int reg = reg_cmp(arg);
    if(reg == -1) return ASS_ARGUMENT_INVALID;
    bytecode_value(asm_ptr, reg);
    return ASS_CORRECT;
}

int ass_popr(Line* text, size_t list_ind, Assembler* asm_ptr, size_t my_ind)
{
    char* arg = text[list_ind].args[0];
    if(arg == NULL) return ASS_ARGUMENT_INVALID;

    int reg = reg_cmp(arg);
    if(reg == -1) return ASS_ARGUMENT_INVALID;

    bytecode_comm(asm_ptr, COMMANDS[my_ind].num);
    bytecode_value(asm_ptr, reg);
    return ASS_CORRECT;
}

int ass_push(Line* text, size_t line_ind, Assembler* asm_ptr, size_t my_ind)
{
    char* arg = NULL;
    if((arg = text[line_ind].args[0]) == NULL) return ASS_ARGUMENT_INVALID;;
    
    int value = 0;
    if(!sscanf(arg, "%10d", &value)) return ASS_ARGUMENT_INVALID;

    bytecode_comm(asm_ptr, COMMANDS[my_ind].num); 
    bytecode_value(asm_ptr, value);
    return ASS_CORRECT;
}

int ass_jump(Line* text, size_t list_ind, Assembler* asm_ptr, size_t my_ind)
{
    char* arg = NULL;
    if((arg = text[list_ind].args[0]) == NULL) return ASS_ARGUMENT_INVALID;;
    int value = 0;
    if(!sscanf(arg, "%10d", &value)) return ASS_ARGUMENT_INVALID;
    if(asm_ptr->labels[value] != -1)
    {
        bytecode_comm(asm_ptr, COMMANDS[my_ind].num); 
        bytecode_value(asm_ptr, -1);
        return ASS_CORRECT; 
    }
    //FIXME структура послекомпиляционного прогона
    return ASS_ARGUMENT_INVALID;
}

int ass_label(Line* text, size_t list_ind, Assembler* asm_ptr, size_t)
{
    char* arg = NULL;
    if((arg = text[list_ind].args[0]) == NULL) return ASS_ARGUMENT_INVALID;;
    size_t value = 0;
    if(!sscanf(arg, "%10lu", &value)) return ASS_ARGUMENT_INVALID;
    if(value >= 10) return ASS_ARGUMENT_INVALID;
    if(asm_ptr->labels[value] != -1) return ASS_ARGUMENT_INVALID;
    asm_ptr->labels[value] = (int)asm_ptr->offset;
    return ASS_CORRECT; 
}

int bytecode_comm(Assembler* asm_ptr, int command)
{
    if(asm_ptr == NULL) return ASS_NULL_OUTPUT_FILE;
    memcpy(asm_ptr->buffer + asm_ptr->offset, &command, sizeof(COMMAND_TYPE));
    asm_ptr->offset += sizeof(COMMAND_TYPE);
    return ASS_CORRECT;
}

int bytecode_value(Assembler* asm_ptr, int value)
{
    if(asm_ptr == NULL) return ASS_NULL_OUTPUT_FILE;
    memcpy(asm_ptr->buffer + asm_ptr->offset, &value, sizeof(VALUE_TYPE));
    asm_ptr->offset += sizeof(VALUE_TYPE);
    return ASS_CORRECT;
}

int reg_cmp(const char* arg)
{
    REG_CMP("SR1", 0);
    REG_CMP("SR2", 1);
    REG_CMP("SR3", 2);
    REG_CMP("SR4", 3);
    REG_CMP("SR5", 4);
    REG_CMP("SR6", 5);
    REG_CMP("SR7", 6);
    REG_CMP("SR8", 7);
    return -1;
}