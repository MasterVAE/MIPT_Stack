#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "assembler_manager.h"
#include "assembler_func.h"
#include "../constants.h"

#define FREE(var) free(var); var = NULL;

enum InitState
{
    SPACE_BEFORE_COMMAND,
    COMMAND,
    SPACE_BEFORE_ARGUMENT,
    ARGUMENT,
    SPACE_AFTER_ARGUMENT,
    COMMENT
};

//====== РАЗБИЕНИЕ КОДА НА КОМАНДЫ И АРГУМЕНТЫ ======//
size_t InitializeText(Line** text, char* buffer, size_t size)
{
    assert(buffer != NULL);
    assert(text != NULL);

    size_t line_count = 1;

    for(size_t i = 0; i < size; i++)
    {
        if(buffer[i] == '\n')   line_count++;
    }

    *text = (Line*)calloc(line_count, sizeof(Line));
    assert(text != NULL);

    line_count = 0;
    InitState state = SPACE_BEFORE_COMMAND;
    for(size_t i = 0; i < size; i++)
    {
        if(buffer[i] == '\n' || buffer[i] == '\0')
        {       
            buffer[i] = '\0';
            line_count++;
            state = SPACE_BEFORE_COMMAND;
        }
        else if(buffer[i] == ';')
        {
            buffer[i] = '\0';
            state = COMMENT;
        }
        else if(buffer[i] == ' ')
        {
            if(state == COMMAND)
            {
                buffer[i] = '\0';
                state = SPACE_BEFORE_ARGUMENT;
            }
            else if(state == ARGUMENT)
            {
                buffer[i] = '\0';
                state = SPACE_AFTER_ARGUMENT;
            }
        }
        else
        {  
            if(state == SPACE_BEFORE_COMMAND)
            {
                (*text)[line_count].line = buffer+i;
                state = COMMAND;
            }
            if(state == SPACE_BEFORE_ARGUMENT)
            {
                (*text)[line_count].arg = buffer+i;
                state = ARGUMENT;
            }
        }
    } 
    return line_count + 1;
}

ASSErr_t ASSInit(Assembler* ass)
{
    if(ass == NULL) return ASS_ASSEMBLER_NULL;

    ass->offset = 0;
    ass->lines_count = 0;
    ass->line_offset = 0;
    ass->text = NULL;
    ass->buffer_size = BUFFER_START_SIZE;
    ass->bin_buffer = (char*)calloc(ass->buffer_size, sizeof(char));
    ass->lbl_table = {};
    
    for(size_t i = 0; i < MAX_LABELS; i++)
    {
        ass->lbl_table.labels[i] = {"", -1};
    }

    for(size_t i = 0; i < MAX_JUMPS; i++)
    {
        ass->lbl_table.forward_jumps[i] = {"", 0};
    }

    return ASS_CORRECT;
}

void ASSDestroy(Assembler* ass)
{
    if(ass == NULL) return;

    FREE(ass->bin_buffer)
    FREE(ass->text)
}

//======= ПОЛУЧЕНИЕ МЕТКИ ПО ИМЕНИ =======//
label* GetLabel(Assembler* ass, char* label_name)
{
    assert(ass);
    assert(label_name);

    for(size_t i = 0; i < MAX_LABELS; i++)
    {
        if(!strcmp(ass->lbl_table.labels[i].name, label_name)) return ass->lbl_table.labels + i;
    }

    return NULL;
}

//======= ПОСТКОМПИЛЯЦИЯ ДЛЯ РАССТАНОВКИ МЕТОК =======//
ASSErr_t ASSPostCompile(Assembler* ass)
{
    if(ass == NULL) return ASS_ASSEMBLER_NULL;
    
    size_t max_offset = ass->offset;

    for(size_t i = 0; i < ass->lbl_table.current_forward_jump; i++)
    {
        label* lbl = GetLabel(ass, ass->lbl_table.forward_jumps[i].label);
        if(!lbl) return ASS_LABEL_INVALID;

        ass->offset = ass->lbl_table.forward_jumps[i].command_pointer;
        BytecodeValue(ass, lbl->address);
    }

    ass->offset = max_offset;

    return ASS_CORRECT;
}

//====== СОЗДАНИЕ НОВОЙ МЕТКИ ======//
void AddLabel(Assembler* ass, char* name, int value)
{
    assert(ass);
    assert(name);

    for(size_t i = 0; i < MAX_LABELS; i++)
    {
        if(ass->lbl_table.labels[i].address == -1)
        {
            strcpy(ass->lbl_table.labels[i].name, name);
            ass->lbl_table.labels[i].address = value;
            break;
        }
    }
}

void ErrorPrinter(ASSErr_t error)
{
    fprintf(stderr, "%s\n", ErrorParser(error));
}

const char* ErrorParser(ASSErr_t error)
{
    switch (error)
    {
        case ASS_CORRECT:               return "Correct";
        case ASS_ASSEMBLER_NULL:        return "Assembler NULL";
        case ASS_NULL_TEXT_POINTER:     return "NULL text pointet";
        case ASS_NULL_BUFFER_POINTER:   return "NULL buffer pointer";
        case ASS_EMPTY_PROGRAMM:        return "Empty programm";
        case ASS_UNKNOWN_COMMAND:       return "Unknown command";
        case ASS_ARGUMENT_INVALID:      return "Argument invalid";
        case ASS_NULL_FILE:             return "Error opening file";
        case ASS_SYNTAX_ERROR:          return "Syntax error";
        case ASS_USED_LABEL:            return "Label overriding";
        case ASS_LABEL_INVALID:         return "Label incorrect";
        case ASS_TOO_MANY_JUMPS:        return "Too many jumps";
        default:                        return "Unknown error";
    }
}