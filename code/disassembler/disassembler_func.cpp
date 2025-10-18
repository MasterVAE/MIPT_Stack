#include <stdio.h>
#include <string.h>

#include "disassembler_func.h"
#include "disassembler_manager.h"

#include "../lib.h"

#define DIS_MODE
#include "../commands.h"

DisErr_t DisDef(Disassembler*, size_t my_ind, FILE* out_file)
{
    fprintf(out_file, "%s\n", COMMANDS[my_ind].name);
    return DIS_CORRECT;
}

DisErr_t DisPush(Disassembler* dis, size_t my_ind, FILE* out_file)
{
    if(dis->offset + sizeof(command_type) + sizeof(value_type) > dis->buffer_size)
    {
        return DIS_ARGUMENT_INVALID;
    }
    int value = DebytecodeInt(dis->buffer + dis->offset + sizeof(command_type), sizeof(value_type));
    dis->offset += sizeof(value_type);
    fprintf(out_file, "%s %d\n", COMMANDS[my_ind].name, value);
    return DIS_CORRECT;
}

DisErr_t DisPopr(Disassembler* dis, size_t my_ind, FILE* out_file)
{
    if(dis->offset + sizeof(command_type) + sizeof(value_type) > dis->buffer_size)
    {
        return DIS_ARGUMENT_INVALID;
    }
    int value = DebytecodeInt(dis->buffer + dis->offset + sizeof(command_type), sizeof(value_type));
    dis->offset += sizeof(value_type);
    fprintf(out_file, "%s %s\n", COMMANDS[my_ind].name, regs[value]);
    return DIS_CORRECT;
}

DisErr_t DisJump(Disassembler* dis, size_t my_ind, FILE* out_file)
{
    if(my_ind + sizeof(command_type) + sizeof(value_type) > dis->buffer_size)
    {
        return DIS_ARGUMENT_INVALID;
    }
    int value = DebytecodeInt(dis->buffer + dis->offset + sizeof(command_type), sizeof(value_type));
    dis->offset += sizeof(value_type);
    fprintf(out_file, "%s %d\n", COMMANDS[my_ind].name, FindLabel(dis, value));
    return DIS_CORRECT;
}
