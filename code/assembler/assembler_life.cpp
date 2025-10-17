#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "assembler_life.h"
#include "assembler_func.h"
#include "../universal_constants.h"

//====== РАЗБИЕНИЕ КОДА НА КОМАНДЫ И АРГУМЕНТЫ ======//
size_t initialize_text(Line** text, char* buffer, size_t size)
{
    assert(buffer != NULL);
    assert(text != NULL);

    size_t line_count = 1;
    for(size_t i = 0; i < size-1; i++)
    {
        if(buffer[i] == '\n')
        {
            line_count++;
        }
    }

    *text = (Line*)calloc(line_count, sizeof(Line));
    assert(text != NULL);

    (*text)[0].line = buffer;

    line_count = 1;
    for(size_t i = 0; i < size-1; i++)
    {
        if(buffer[i] == '\n')
        {       
            buffer[i] = '\0';
            (*text)[line_count++].line = buffer+i+1;
        }
        else if(buffer[i] == ';')
        {
            buffer[i] = '\0';
        }
        
    } 
    for(size_t current_line = 0; current_line < line_count; current_line++)
    {
        size_t read = 0;
        size_t off = parse((*text)[current_line].line, 
            (*text)[current_line].line, MAX_COMMAND_LENGHT, &read);

        if((current_line < line_count-1 && 
            (*text)[current_line].line + off + 1 < (*text)[current_line+1].line) || 
            current_line == line_count-1) off++;
        for(size_t current_arg = 0; current_arg < ARG_LIMIT; current_arg++)
        {
            size_t add = parse((*text)[current_line].line + off, 
                               (*text)[current_line].line + off, MAX_COMMAND_LENGHT, &read);
            if(read == 0)
            {
                (*text)[current_line].args[current_arg] = NULL;
                break;
            }
            (*text)[current_line].args[current_arg] = (*text)[current_line].line + off;
            (*text)[current_line].arg_count++;
            off += add;
            if(current_line < line_count-1 && 
                (*text)[current_line].line + off + 1 < (*text)[current_line+1].line) off++;
            (*text)[current_line].args[current_arg + 1] = (*text)[current_line].line + off;
        }
    }
    
    return line_count;
}

//====== УДАЛЕНИЕ ЛИШНИХ ПРОБЕЛЬНЫХ СИМВОЛОВ ИЗ СТРОКИ ======//
size_t parse(const char* source, char* dest, size_t max, size_t* read)
{
    assert(source != NULL);
    assert(dest != NULL);

    if(read != NULL) *read = 0;

    size_t offset = 0;
    char c = '\0';
    int space = 1;
    while((c = source[offset]) != '\0' && c != '\n' && offset < max)
    {
        if(space && c != ' ')
        {
            space = 0;
        }
        if(!space && c != ' ')
        {
            *(dest++) = c;
            if(read != NULL) (*read)++;
        }
        else if(!space && c == ' ')
        {
            break;
        }
        offset++;    
    }
    *dest = '\0';
    return offset;
}

ASSErr_t ASSInit(Assembler* ass)
{
    if(ass == NULL) return ASS_ASSEMBLER_NULL;

    ass->offset = 0;
    ass->lines_count = 0;
    ass->line_offset = 0;
    ass->buffer_size = buffer_start_size;
    ass->buffer = (char*)calloc(ass->buffer_size, sizeof(char));
    ass->current_jump_memory = 0;
    
    for(size_t i = 0; i < MAX_LABELS; i++)
    {
        ass->labels[i] = {"", -1};
    }
    for(size_t i = 0; i < MAX_JUMPS; i++)
    {
        ass->jumps[i] = {"", 0};
    }

    return ASS_CORRECT;
}

void ASSDestroy(Assembler* ass)
{
    if(ass == NULL) return;
    free(ass->buffer);
    free(ass->text);
    ass->buffer = NULL;
    ass->text = NULL;
}

//======= ПОЛУЧЕНИЕ МЕТКИ ПО ИМЕНИ =======//
label* get_label(Assembler* ass, char* label_name)
{
    assert(ass);
    assert(label_name);

    for(size_t i = 0; i < MAX_LABELS; i++)
    {
        if(!strcmp(ass->labels[i].name, label_name)) return ass->labels + i;
    }
    return NULL;
}

//======= ПОСТКОМПИЛЯЦИЯ ДЛЯ РАССТАНОВКИ МЕТОК =======//
ASSErr_t ASSPostCompile(Assembler* ass)
{
    if(ass == NULL) return ASS_ASSEMBLER_NULL;
    
    size_t max_offset = ass->offset;

    for(size_t i = 0; i < ass->current_jump_memory; i++)
    {
        label* lbl = get_label(ass, ass->jumps[i].label);
        if(!lbl) return ASS_LABEL_INVALID;

        ass->offset = ass->jumps[i].offcet;
        bytecode_value(ass, lbl->value);
    }
    ass->offset = max_offset;
    return ASS_CORRECT;
}

//====== СОЗДАНИЕ НОВОЙ МЕТКИ ======//
void add_label(Assembler* ass, char* name, int value)
{
    assert(ass);
    assert(name);

    for(size_t i = 0; i < MAX_LABELS; i++)
    {
        if(ass->labels[i].value == -1)
        {
            strcpy(ass->labels[i].name, name);
            ass->labels[i].value = value;
            break;
        }
    }
}

void error_printer(ASSErr_t error)
{
    fprintf(stderr, "%s\n", error_parser(error));
}

const char* error_parser(ASSErr_t error)
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