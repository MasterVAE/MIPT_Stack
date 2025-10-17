#include <stdio.h>
#include <string.h>

#include "disassembler_func.h"
#include "disassembler_life.h"

#include "../lib.h"

#define DIS_MODE
#include "../commands.h"

dis_err dis_def(Disassembler*, size_t my_ind, FILE* out_file)
{
    fprintf(out_file, "%s\n", COMMANDS[my_ind].name);
    return DIS_CORRECT;
}

dis_err dis_push(Disassembler* dis, size_t my_ind, FILE* out_file)
{
    if(dis->offset + sizeof(command_type) + sizeof(value_type) > dis->buffer_size)
    {
        return DIS_ARGUMENT_INVALID;
    }
    int value = debytecode_int(dis->buffer + dis->offset + sizeof(command_type), sizeof(value_type));
    dis->offset += sizeof(value_type);
    fprintf(out_file, "%s %d\n", COMMANDS[my_ind].name, value);
    return DIS_CORRECT;
}

dis_err dis_popr(Disassembler* dis, size_t my_ind, FILE* out_file)
{
    if(dis->offset + sizeof(command_type) + sizeof(value_type) > dis->buffer_size)
    {
        return DIS_ARGUMENT_INVALID;
    }
    int value = debytecode_int(dis->buffer + dis->offset + sizeof(command_type), sizeof(value_type));
    dis->offset += sizeof(value_type);
    fprintf(out_file, "%s %s\n", COMMANDS[my_ind].name, regs[value]);
    return DIS_CORRECT;
}

dis_err dis_jump(Disassembler* dis, size_t my_ind, FILE* out_file)
{
    if(my_ind + sizeof(command_type) + sizeof(value_type) > dis->buffer_size)
    {
        return DIS_ARGUMENT_INVALID;
    }
    int value = debytecode_int(dis->buffer + dis->offset + sizeof(command_type), sizeof(value_type));
    dis->offset += sizeof(value_type);
    fprintf(out_file, "%s %d\n", COMMANDS[my_ind].name, find_label(dis, value));
    return DIS_CORRECT;
}
