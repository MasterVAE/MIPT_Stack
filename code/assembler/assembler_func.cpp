#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "assembler_func.h"
#include "assembler_manager.h"
#include "../constants.h"

#define ASS_MODE
#include "../commands.h"

#define NUM_TO_STR(arg) #arg
#define ITOS(arg) NUM_TO_STR(arg)

#define MAX_ARG_LEN 10
#define REG_LEN 3

static int RegCmp(const char* arg);

ASSErr_t AssDef(Assembler* asm_ptr, size_t my_ind) 
{
    assert(asm_ptr);

    BytecodeComm(asm_ptr, COMMANDS[my_ind].num);

    return ASS_CORRECT;
}

ASSErr_t AssPopr(Assembler* asm_ptr, size_t my_ind)
{
    assert(asm_ptr);

    char* arg = asm_ptr->text[asm_ptr->line_offset].arg;
    if(arg == NULL) return ASS_ARGUMENT_INVALID;

    int reg = RegCmp(arg);
    if(reg == -1)   return ASS_ARGUMENT_INVALID;

    BytecodeComm(asm_ptr, COMMANDS[my_ind].num);
    BytecodeValue(asm_ptr, reg);

    return ASS_CORRECT;
}

ASSErr_t AssPopm(Assembler* asm_ptr, size_t my_ind)
{
    assert(asm_ptr);

    char* arg = asm_ptr->text[asm_ptr->line_offset].arg;
    if(arg == NULL) return ASS_ARGUMENT_INVALID;

    char arg2[11] = {};
    if(!sscanf(arg, "[%3s]", arg2)) return ASS_ARGUMENT_INVALID;
    int reg = RegCmp(arg2);
    if(reg == -1) return ASS_ARGUMENT_INVALID;

    BytecodeComm(asm_ptr, COMMANDS[my_ind].num);
    BytecodeValue(asm_ptr, reg);

    return ASS_CORRECT;
}

const int PUSHR_INDEX = 13;
const int PUSHM_INDEX = 14;

ASSErr_t AssPush(Assembler* asm_ptr, size_t my_ind)
{
    assert(asm_ptr);

    char* arg = NULL;
    if((arg = asm_ptr->text[asm_ptr->line_offset].arg) == NULL) return ASS_ARGUMENT_INVALID;
    
    int value = 0;
    char data[MAX_ARG_LEN + 1] = {};
    if(sscanf(arg, "%" ITOS(MAX_ARG_LEN) "d", &value))
    {
        BytecodeComm(asm_ptr, COMMANDS[my_ind].num); 
        BytecodeValue(asm_ptr, value);
        return ASS_CORRECT;
    }
    if(sscanf(arg, "[%" ITOS(REG_LEN) "s]", data))
    {
        return AssPopm(asm_ptr, PUSHM_INDEX);
    }
    if(sscanf(arg, "%" ITOS(MAX_ARG_LEN) "s", data))
    {
        return AssPopr(asm_ptr, PUSHR_INDEX);
    }

    return ASS_ARGUMENT_INVALID;
}

ASSErr_t AssJump(Assembler* asm_ptr, size_t my_ind)
{
    assert(asm_ptr);

    char* arg = NULL;
    if((arg = asm_ptr->text[asm_ptr->line_offset].arg) == NULL) return ASS_ARGUMENT_INVALID;
    if(asm_ptr->lbl_table->current_forward_jump >= MAX_JUMPS)   return ASS_TOO_MANY_JUMPS;
    label* lbl = GetLabel(asm_ptr, arg); 

    if(lbl)
    {
        BytecodeComm(asm_ptr, COMMANDS[my_ind].num); 
        BytecodeValue(asm_ptr, lbl->address);

        return ASS_CORRECT; 
    }
    
    asm_ptr->lbl_table->forward_jumps[asm_ptr->lbl_table->current_forward_jump].command_pointer 
        = asm_ptr->offset+sizeof(command_type);
    memcpy(asm_ptr->lbl_table->forward_jumps[asm_ptr->lbl_table->current_forward_jump].label, 
        arg, MAX_COMMAND_LENGHT);
    asm_ptr->lbl_table->current_forward_jump++;
    
    BytecodeComm(asm_ptr, COMMANDS[my_ind].num); 
    BytecodeValue(asm_ptr, -1);

    return ASS_CORRECT;
}

ASSErr_t AssLabel(Assembler* asm_ptr, size_t)
{
    assert(asm_ptr);

    char* arg = NULL;
    if((arg = asm_ptr->text[asm_ptr->line_offset].arg) == NULL) 
                                                    return ASS_ARGUMENT_INVALID;;
    if(GetLabel(asm_ptr, arg))                      return ASS_USED_LABEL;
    AddLabel(asm_ptr, arg, (int)asm_ptr->offset);   return ASS_CORRECT; 
}

int BytecodeComm(Assembler* asm_ptr, int command)
{
    assert(asm_ptr);

    if(asm_ptr == NULL)             return ASS_ASSEMBLER_NULL;
    if(asm_ptr->bin_buffer == NULL) return ASS_NULL_BUFFER_POINTER;

    memcpy(asm_ptr->bin_buffer + asm_ptr->offset, &command, sizeof(command_type));
    asm_ptr->offset += sizeof(command_type);

    return ASS_CORRECT;
}

int BytecodeValue(Assembler* asm_ptr, int value)
{
    assert(asm_ptr);

    if(asm_ptr == NULL)             return ASS_ASSEMBLER_NULL;
    if(asm_ptr->bin_buffer == NULL) return ASS_NULL_BUFFER_POINTER;

    memcpy(asm_ptr->bin_buffer + asm_ptr->offset, &value, sizeof(value_type));
    asm_ptr->offset += sizeof(value_type);

    return ASS_CORRECT;
}


//======= ПОИСК РЕГИСТРА ПО ИМЕНИ =======//
static int RegCmp(const char* arg)
{
    assert(arg);

    for(size_t i = 0; i < REG_SIZE; i++)
    {
        if(!strcmp(arg, regs[i]))
        {
            return (int)i;
        }
    }

    return -1;
}