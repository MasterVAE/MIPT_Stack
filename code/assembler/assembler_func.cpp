#include <stdio.h>
#include <string.h>

#include "assembler_func.h"
#include "assembler_read.h"
#include "../language.h"

#define ASS_MODE
#include "../commands.h"

#define REG_CMP(reg, num) if(!strcmp(arg, reg)) return num;

int reg_cmp(const char* arg);

int ass_halt(Assembler* asm_ptr, size_t my_ind) 
{
    bytecode_comm(asm_ptr, COMMANDS[my_ind].num);
    return ASS_CORRECT;
}

int ass_add(Assembler* asm_ptr, size_t my_ind)
{
    bytecode_comm(asm_ptr, COMMANDS[my_ind].num);
    return ASS_CORRECT;
}

int ass_sub(Assembler* asm_ptr, size_t my_ind)
{
    bytecode_comm(asm_ptr, COMMANDS[my_ind].num);
    return ASS_CORRECT;
}

int ass_mul(Assembler* asm_ptr, size_t my_ind)
{
    bytecode_comm(asm_ptr, COMMANDS[my_ind].num);
    return ASS_CORRECT;
}

int ass_div(Assembler* asm_ptr, size_t my_ind)
{
    bytecode_comm(asm_ptr, COMMANDS[my_ind].num);
    return ASS_CORRECT;
}

int ass_sqrt(Assembler* asm_ptr, size_t my_ind)
{
    bytecode_comm(asm_ptr, COMMANDS[my_ind].num);
    return ASS_CORRECT;
}

int ass_out(Assembler* asm_ptr, size_t my_ind)
{
    bytecode_comm(asm_ptr, COMMANDS[my_ind].num);
    return ASS_CORRECT;
}

int ass_in(Assembler* asm_ptr, size_t my_ind)
{
    bytecode_comm(asm_ptr, COMMANDS[my_ind].num);
    return ASS_CORRECT;
}

int ass_pushr(Assembler* asm_ptr, size_t my_ind)
{
    char* arg = asm_ptr->text[asm_ptr->line_offset].args[0];
    if(arg == NULL) return ASS_ARGUMENT_INVALID;
    
    int reg = reg_cmp(arg);
    if(reg == -1) return ASS_ARGUMENT_INVALID;

    bytecode_comm(asm_ptr, COMMANDS[my_ind].num);
    bytecode_value(asm_ptr, reg);
    return ASS_CORRECT;
}

int ass_popr(Assembler* asm_ptr, size_t my_ind)
{
    char* arg = asm_ptr->text[asm_ptr->line_offset].args[0];
    if(arg == NULL) return ASS_ARGUMENT_INVALID;

    int reg = reg_cmp(arg);
    if(reg == -1) return ASS_ARGUMENT_INVALID;

    bytecode_comm(asm_ptr, COMMANDS[my_ind].num);
    bytecode_value(asm_ptr, reg);
    return ASS_CORRECT;
}

int ass_push(Assembler* asm_ptr, size_t my_ind)
{
    char* arg = NULL;
    if((arg = asm_ptr->text[asm_ptr->line_offset].args[0]) == NULL) return ASS_ARGUMENT_INVALID;
    
    int value = 0;
    if(!sscanf(arg, "%10d", &value)) return ASS_ARGUMENT_INVALID;

    bytecode_comm(asm_ptr, COMMANDS[my_ind].num); 
    bytecode_value(asm_ptr, value);
    return ASS_CORRECT;
}

int ass_jump(Assembler* asm_ptr, size_t my_ind)
{
    char* arg = NULL;
    int value = 0;
    if((arg = asm_ptr->text[asm_ptr->line_offset].args[0]) == NULL) return ASS_ARGUMENT_INVALID;
    if(!sscanf(arg, "%10d", &value)) return ASS_ARGUMENT_INVALID;
    if(asm_ptr->current_jump_memory >= MAX_JUMPS) return ASS_TOO_MANY_JUMPS;
    if(asm_ptr->labels[value] != -1)
    {
        bytecode_comm(asm_ptr, COMMANDS[my_ind].num); 
        bytecode_value(asm_ptr, asm_ptr->labels[value]);
        return ASS_CORRECT; 
    }
    asm_ptr->jumps[asm_ptr->current_jump_memory].offcet = asm_ptr->offset+sizeof(COMMAND_TYPE);
    asm_ptr->jumps[asm_ptr->current_jump_memory].label = value;
    asm_ptr->current_jump_memory++;
    
    bytecode_comm(asm_ptr, COMMANDS[my_ind].num); 
    bytecode_value(asm_ptr, -1);
    return ASS_CORRECT;
}

int ass_label(Assembler* asm_ptr, size_t)
{
    char* arg = NULL;
    int value = 0;
    if((arg = asm_ptr->text[asm_ptr->line_offset].args[0]) == NULL) 
                                                    return ASS_ARGUMENT_INVALID;;
    if(!sscanf(arg, "%10d", &value))                return ASS_ARGUMENT_INVALID;
    if(!correct_label(value))                       return ASS_LABEL_INVALID;
    if(asm_ptr->labels[value] != -1)                return ASS_USED_LABEL;
    asm_ptr->labels[value] = (int)asm_ptr->offset;  return ASS_CORRECT; 
}

int bytecode_comm(Assembler* asm_ptr, int command)
{
    if(asm_ptr == NULL)         return ASS_ASSEMBLER_NULL;
    if(asm_ptr->buffer == NULL) return ASS_NULL_BUFFER_POINTER;

    memcpy(asm_ptr->buffer + asm_ptr->offset, &command, sizeof(COMMAND_TYPE));
    asm_ptr->offset += sizeof(COMMAND_TYPE);
    return ASS_CORRECT;
}

int bytecode_value(Assembler* asm_ptr, int value)
{
    if(asm_ptr == NULL)         return ASS_ASSEMBLER_NULL;
    if(asm_ptr->buffer == NULL) return ASS_NULL_BUFFER_POINTER;

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