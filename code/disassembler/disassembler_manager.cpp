#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "disassembler_manager.h"
#include "../lib.h"

#define DIS_MODE
#include "../commands.h"

static DisErr_t FindCommand(Disassembler* dis);

DisErr_t DISInit(Disassembler* dis)
{
    if(dis == NULL) return DIS_NULL_DISASSEMBLER;

    dis->buffer_size = 0;
    dis->offset = 0;

    dis->output_offset = 0;
    dis->output_buffer = (char*)calloc(OUT_BUFFER_START_SIZE, sizeof(char));
    dis->output_buffer_size = OUT_BUFFER_START_SIZE;

    for(size_t i = 0; i < MAX_LABELS; dis->labels[i++] = -1);

    return DIS_CORRECT;
}

void DISDestroy(Disassembler* dis)
{
    if(!dis) return;

    free(dis->buffer);
    free(dis->output_buffer);
    memset(dis, 0, sizeof(Disassembler));
}

DisErr_t Disassemble(Disassembler* dis)
{
    if(!dis) return DIS_NULL_DISASSEMBLER;
    if(dis->buffer == NULL) return DIS_NULL_BUFFER;
    if(dis->buffer_size == 0) return DIS_EMPTY_PROGRAMM;

    for(dis->offset = 0; dis->offset < dis->buffer_size; dis->offset+=sizeof(command_type))
    {
        InsertLabel(dis);
        FindCommand(dis);
        if(dis->offset + sizeof(command_type) > dis->buffer_size)   return DIS_SYNTAX_ERROR;
    }

    return DIS_CORRECT;
}


static DisErr_t FindCommand(Disassembler* dis)
{
    int comm = DebytecodeInt(dis->buffer + dis->offset, sizeof(command_type));
    bool found = 0;

    for(size_t command = 0; command < COMMANDS_COUNT; command++)
    {    
        if(COMMANDS[command].num == comm)
        {
            COMMANDS[command].DisFunc(dis, command);
            found = 1;
            break;
        }
    }

    if(!found)  return DIS_UNKNOWN_COMMAND;
    return DIS_CORRECT;
}


void ErrorPrinter(DisErr_t error)
{
    fprintf(stderr, "%s\n", ErrorParser(error));
}

const char* ErrorParser(DisErr_t error)
{
    switch (error)
    {
        case DIS_CORRECT:           return "Correct";
        case DIS_NULL_DISASSEMBLER: return "Disassembler NULL";
        case DIS_NULL_BUFFER:       return "Buffer NULL";
        case DIS_NULL_FILE:         return "Error opening file";
        case DIS_EMPTY_PROGRAMM:    return "Empty programm";
        case DIS_UNKNOWN_COMMAND:   return "Unknown command";
        case DIS_SYNTAX_ERROR:      return "Syntax error";
        case DIS_ARGUMENT_INVALID:  return "Argument invalid";
        default:                    return "Unknown error";
    }
}

int FindLabel(Disassembler* dis, int label_value)
{
    assert(dis);

    for(size_t i = 0; i < MAX_LABELS; i++)
    {
        if(dis->labels[i] == label_value) return (int)i;
    }

    return -1;
}

//====== ПОИСК ВСЕХ МЕТОК В КОДЕ ======//
DisErr_t LabelSearch(Disassembler* dis)
{
    if(!dis) return DIS_NULL_DISASSEMBLER;
    if(!dis->buffer) return DIS_NULL_BUFFER;
    if(dis->buffer_size == 0) return DIS_EMPTY_PROGRAMM;

    for(dis->offset = 0; dis->offset < dis->buffer_size;)
    {
        if(dis->offset + sizeof(command_type) > dis->buffer_size)   return DIS_SYNTAX_ERROR;
        int comm = DebytecodeInt(dis->buffer + dis->offset, sizeof(command_type));
        bool found = 0;
        for(size_t j = 0; j < COMMANDS_COUNT; j++)
        {    
            if(COMMANDS[j].num != comm) continue;
            found = 1;
            if(COMMANDS[j].DisFunc != DisJump)
            {
                dis->offset += COMMANDS[j].byte_size;
                break;
            }
            if(dis->offset + sizeof(command_type) + sizeof(value_type) > dis->buffer_size)
            {
                return DIS_ARGUMENT_INVALID;
            }
            int value = DebytecodeInt(dis->buffer + dis->offset + sizeof(command_type), 
                sizeof(value_type));

            for(size_t k = 0; k < MAX_LABELS; k++)
            {
                if(dis->labels[k] == -1 || dis->labels[k] == value)
                {
                    dis->labels[k] = value;
                    break;
                }
            }
            dis->offset += COMMANDS[j].byte_size;
            break;
        }
        if(!found) return DIS_UNKNOWN_COMMAND;
    }
    
    return DIS_CORRECT;
}

//====== ВСТАВКА МЕТКИ В КОД ======//
void InsertLabel(Disassembler* dis)
{
    assert(dis);

    for(size_t i = 0; i < MAX_LABELS; i++)
    {
        if(dis->labels[i] == (int)dis->offset)
        {
            int written = sprintf(dis->output_buffer + dis->output_offset, "\nLABEL %lu\n", i);
            dis->output_offset += (size_t)written;
            return;
        }
    }
}