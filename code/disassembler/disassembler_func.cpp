#include <stdio.h>
#include <string.h>

#include "disassembler_func.h"
#include "disassembler_life.h"

#define DIS_MODE
#include "../commands.h"

int dis_def(Disassembler*, size_t my_ind, FILE* out_file)
{
    fprintf(out_file, "%s\n", COMMANDS[my_ind].name);
    return 0;
}

int dis_push(Disassembler* dis, size_t my_ind, FILE* out_file)
{
    if(dis->offset + sizeof(COMMAND_TYPE) + sizeof(VALUE_TYPE) > dis->buffer_size)
    {
        return DIS_PUSH_ARGUMENT_INVALID;
    }
    int value = debytecode(dis->buffer + dis->offset + sizeof(COMMAND_TYPE), sizeof(VALUE_TYPE));
    dis->offset += sizeof(VALUE_TYPE);
    fprintf(out_file, "%s %d\n", COMMANDS[my_ind].name, value);
    return 0;
}

int dis_popr(Disassembler* dis, size_t my_ind, FILE* out_file)
{
    if(dis->offset + sizeof(COMMAND_TYPE) + sizeof(VALUE_TYPE) > dis->buffer_size)
    {
        return DIS_PUSH_ARGUMENT_INVALID;
    }
    int value = debytecode(dis->buffer + dis->offset + sizeof(COMMAND_TYPE), sizeof(VALUE_TYPE));
    dis->offset += sizeof(VALUE_TYPE);
    fprintf(out_file, "%s %s\n", COMMANDS[my_ind].name, regs[value]);
    return 0;
}

int dis_jump(Disassembler* dis, size_t my_ind, FILE* out_file)
{
    if(my_ind + sizeof(COMMAND_TYPE) + sizeof(VALUE_TYPE) > dis->buffer_size)
    {
        return DIS_PUSH_ARGUMENT_INVALID;
    }
    int value = debytecode(dis->buffer + dis->offset + sizeof(COMMAND_TYPE), sizeof(VALUE_TYPE));
    dis->offset += sizeof(VALUE_TYPE);
    fprintf(out_file, "%s %d\n", COMMANDS[my_ind].name, value);
    return 0;
}


int debytecode(char* code, size_t size)
{
    int value;
    memcpy(&value, code, size);
    return value;
}