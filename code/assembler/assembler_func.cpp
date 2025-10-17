#include <stdio.h>
#include <string.h>

#include "assembler_func.h"
#include "assembler_read.h"
#include "../language.h"

#define ASS_MODE
#include "../commands.h"

int reg_cmp(const char* arg);

ASSErr_t ass_def(Assembler* asm_ptr, size_t my_ind) 
{
    bytecode_comm(asm_ptr, COMMANDS[my_ind].num);
    return ASS_CORRECT;
}

ASSErr_t ass_popr(Assembler* asm_ptr, size_t my_ind)
{
    char* arg = asm_ptr->text[asm_ptr->line_offset].args[0];
    if(arg == NULL) return ASS_ARGUMENT_INVALID;

    int reg = reg_cmp(arg);
    if(reg == -1) return ASS_ARGUMENT_INVALID;

    bytecode_comm(asm_ptr, COMMANDS[my_ind].num);
    bytecode_value(asm_ptr, reg);
    return ASS_CORRECT;
}

ASSErr_t ass_popm(Assembler* asm_ptr, size_t my_ind)
{
    char* arg = asm_ptr->text[asm_ptr->line_offset].args[0];
    if(arg == NULL) return ASS_ARGUMENT_INVALID;

    char arg2[11] = {};
    if(!sscanf(arg, "[%3s]", arg2)) return ASS_ARGUMENT_INVALID;
    int reg = reg_cmp(arg2);
    if(reg == -1) return ASS_ARGUMENT_INVALID;

    bytecode_comm(asm_ptr, COMMANDS[my_ind].num);
    bytecode_value(asm_ptr, reg);
    return ASS_CORRECT;
}

ASSErr_t ass_push(Assembler* asm_ptr, size_t my_ind)
{
    char* arg = NULL;
    if((arg = asm_ptr->text[asm_ptr->line_offset].args[0]) == NULL) return ASS_ARGUMENT_INVALID;
    
    int value = 0;
    char data[11] = {};
    if(sscanf(arg, "%10d", &value))
    {
        bytecode_comm(asm_ptr, COMMANDS[my_ind].num); 
        bytecode_value(asm_ptr, value);
        return ASS_CORRECT;
    }
    if(sscanf(arg, "[%3s]", data))
    {
        for(size_t i = 0; i < COMMANDS_COUNT; i++)
        {
            if(!strcmp(COMMANDS[i].name, "PUSHM"))  return ass_popm(asm_ptr, i);
        }
    }
    if(sscanf(arg, "%10s", data))
    {
        for(size_t i = 0; i < COMMANDS_COUNT; i++)
        {
            if(!strcmp(COMMANDS[i].name, "PUSHR"))  return ass_popr(asm_ptr, i);
        }
    }
    return ASS_ARGUMENT_INVALID;
}

ASSErr_t ass_jump(Assembler* asm_ptr, size_t my_ind)
{
    char* arg = NULL;
    if((arg = asm_ptr->text[asm_ptr->line_offset].args[0]) == NULL) return ASS_ARGUMENT_INVALID;
    if(asm_ptr->current_jump_memory >= MAX_JUMPS) return ASS_TOO_MANY_JUMPS;
    label* lbl = get_label(asm_ptr, arg); 
    if(lbl)
    {
        bytecode_comm(asm_ptr, COMMANDS[my_ind].num); 
        bytecode_value(asm_ptr, lbl->value);
        return ASS_CORRECT; 
    }
    asm_ptr->jumps[asm_ptr->current_jump_memory].offcet = asm_ptr->offset+sizeof(command_type);
    memcpy(asm_ptr->jumps[asm_ptr->current_jump_memory].label, arg, MAX_COMMAND_LENGHT);
    asm_ptr->current_jump_memory++;
    
    bytecode_comm(asm_ptr, COMMANDS[my_ind].num); 
    bytecode_value(asm_ptr, -1);
    return ASS_CORRECT;
}

ASSErr_t ass_label(Assembler* asm_ptr, size_t)
{
    char* arg = NULL;
    if((arg = asm_ptr->text[asm_ptr->line_offset].args[0]) == NULL) 
                                                    return ASS_ARGUMENT_INVALID;;
    if(get_label(asm_ptr, arg))                     return ASS_USED_LABEL;
    add_label(asm_ptr, arg, (int)asm_ptr->offset);  return ASS_CORRECT; 
}

int bytecode_comm(Assembler* asm_ptr, int command)
{
    if(asm_ptr == NULL)         return ASS_ASSEMBLER_NULL;
    if(asm_ptr->buffer == NULL) return ASS_NULL_BUFFER_POINTER;

    memcpy(asm_ptr->buffer + asm_ptr->offset, &command, sizeof(command_type));
    asm_ptr->offset += sizeof(command_type);
    return ASS_CORRECT;
}

int bytecode_value(Assembler* asm_ptr, int value)
{
    if(asm_ptr == NULL)         return ASS_ASSEMBLER_NULL;
    if(asm_ptr->buffer == NULL) return ASS_NULL_BUFFER_POINTER;

    memcpy(asm_ptr->buffer + asm_ptr->offset, &value, sizeof(value_type));
    asm_ptr->offset += sizeof(value_type);
    return ASS_CORRECT;
}


//======= ПОИСК РЕГИСТРА ПО ИМЕНИ =======//
int reg_cmp(const char* arg)
{
    for(size_t i = 0; i < REG_SIZE; i++)
    {
        if(!strcmp(arg, regs[i]))
        {
            return (int)i;
        }
    }
    return -1;
}