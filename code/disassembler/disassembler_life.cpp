#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "disassembler_life.h"
#include "../lib.h"

#define DIS_MODE
#include "../commands.h"

int DISInit(Disassembler* dis)
{
    if(dis == NULL) return 1;
    dis->buffer_size = 0;
    dis->offset = 0;
    for(size_t i = 0; i < MAX_LABELS; dis->labels[i++] = -1);
    return 0;
}

void DISDestroy(Disassembler* dis)
{
    free(dis->buffer);
    memset(dis, 0, sizeof(Disassembler));
}

void error_printer(dis_err error)
{
    fprintf(stderr, "%s\n", error_parser(error));
}

const char* error_parser(dis_err error)
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

int find_label(Disassembler* dis, int label_value)
{
    for(size_t i = 0; i < MAX_LABELS; i++)
    {
        if(dis->labels[i] == label_value) return (int)i;
    }
    return -1;
}

//====== ПОИСК ВСЕХ МЕТОК В КОДЕ ======//
dis_err label_search(Disassembler* dis)
{
    if(dis->buffer == NULL) return DIS_NULL_BUFFER;
    if(dis->buffer_size == 0) return DIS_EMPTY_PROGRAMM;

    for(dis->offset = 0; dis->offset < dis->buffer_size;)
    {
        if(dis->offset + sizeof(command_type) > dis->buffer_size)   return DIS_SYNTAX_ERROR;
        int comm = debytecode_int(dis->buffer + dis->offset, sizeof(command_type));
        bool found = 0;
        for(size_t j = 0; j < COMMANDS_COUNT; j++)
        {    
            if(COMMANDS[j].num != comm) continue;
            found = 1;
            if(COMMANDS[j].dis_func != dis_jump)
            {
                dis->offset += COMMANDS[j].byte_size;
                break;
            }
            if(dis->offset + sizeof(command_type) + sizeof(value_type) > dis->buffer_size)
            {
                return DIS_ARGUMENT_INVALID;
            }
            int value = debytecode_int(dis->buffer + dis->offset + sizeof(command_type), sizeof(value_type));
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
void insert_label(Disassembler* dis, FILE* output_file)
{
    for(size_t i = 0; i < MAX_LABELS; i++)
    {
        if(dis->labels[i] == (int)dis->offset)
        {
            fprintf(output_file, "\nLABEL %lu\n", i);
            return;
        }
    }
}