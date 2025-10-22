#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "disassembler_func.h"
#include "disassembler_manager.h"

#include "../lib.h"

#define DIS_MODE
#include "../commands.h"

#define WRITE(dis, format, value)\
    if(dis->output_buffer_size - dis->output_offset < OUT_BUFFER_START_SIZE)\
    {\
        dis->output_buffer_size *= OUT_BUFFER_SIZE_MULT;\
        dis->output_buffer = (char*)realloc(dis->output_buffer,\
            dis->output_buffer_size * sizeof(char));\
    }\
    {\
        int written = sprintf(dis->output_buffer + dis->output_offset, format, value);\
        dis->output_offset += (size_t)written;\
    }


DisErr_t DisDef(Disassembler* dis, size_t my_ind)
{
    assert(dis);

    WRITE(dis, "%s\n", COMMANDS[my_ind].name);
    
    return DIS_CORRECT;
}

DisErr_t DisPush(Disassembler* dis, size_t my_ind)
{
    assert(dis);

    if(dis->offset + sizeof(command_type) + sizeof(value_type) > dis->buffer_size)
        return DIS_ARGUMENT_INVALID;
    
    int value = DebytecodeInt(dis->buffer + dis->offset + sizeof(command_type), sizeof(value_type));
    dis->offset += sizeof(value_type);
    
    WRITE(dis, "%s ", COMMANDS[my_ind].name);
    WRITE(dis, "%d\n", value);

    return DIS_CORRECT;
}

DisErr_t DisPopr(Disassembler* dis, size_t my_ind)
{
    assert(dis);

    if(dis->offset + sizeof(command_type) + sizeof(value_type) > dis->buffer_size)
    {
        return DIS_ARGUMENT_INVALID;
    }
    int value = DebytecodeInt(dis->buffer + dis->offset + sizeof(command_type), sizeof(value_type));
    dis->offset += sizeof(value_type);

    WRITE(dis, "%s ", COMMANDS[my_ind].name);
    WRITE(dis, "%s\n", regs[value]);

    return DIS_CORRECT;
}

DisErr_t DisJump(Disassembler* dis, size_t my_ind)
{
    assert(dis);

    if(my_ind + sizeof(command_type) + sizeof(value_type) > dis->buffer_size)
    {
        return DIS_ARGUMENT_INVALID;
    }
    int value = DebytecodeInt(dis->buffer + dis->offset + sizeof(command_type), sizeof(value_type));
    dis->offset += sizeof(value_type);

    WRITE(dis, "%s ", COMMANDS[my_ind].name);
    WRITE(dis, "%d\n", FindLabel(dis, value));

    return DIS_CORRECT;
}
