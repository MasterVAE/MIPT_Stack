#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "assembler_read.h"
#include "assembler_func.h"
#include "../language.h"

bool correct_label(Assembler* ass, label* lbl);

//====== РАЗБИЕНИЕ КОДА НА КОМАНДЫ И АРГУМЕНТЫ ======//
size_t initialize_text(Line** text, char* buffer, size_t size)
{
    assert(buffer != NULL);
    assert(text != NULL);

    size_t count = 1;
    for(size_t i = 0; i < size-1; i++)
    {
        if(buffer[i] == '\n')
        {
            count++;
        }
    }

    *text = (Line*)calloc(count, sizeof(Line));
    assert(text != NULL);

    (*text)[0].line = buffer;

    size_t j = 1;
    for(size_t i = 0; i < size-1; i++)
    {
        if(buffer[i] == '\n')
        {       
            buffer[i] = '\0';
            (*text)[j++].line = buffer+i+1;
        }
        else if(buffer[i] == ';')
        {
            buffer[i] = '\0';
        }
        
    } 
    for(size_t i = 0; i < count; i++)
    {
        size_t read = 0;
        size_t off = parse((*text)[i].line, (*text)[i].line, MAX_COMMAND_LENGHT, &read);

        if((i < count-1 && (*text)[i].line + off + 1 < (*text)[i+1].line) || i == count-1) off++;
        for(j = 0; j < ARG_LIMIT; j++)
        {
            size_t add = parse((*text)[i].line + off, (*text)[i].line + off, MAX_COMMAND_LENGHT, &read);
            if(read == 0)
            {
                (*text)[i].args[j] = NULL;
                break;
            }
            (*text)[i].args[j] = (*text)[i].line + off;
            (*text)[i].arg_count++;
            off += add;
            if(i < count-1 && (*text)[i].line + off + 1 < (*text)[i+1].line) off++;
            (*text)[i].args[j + 1] = (*text)[i].line + off;
        }
    }
    
    return count;
}

//====== УДАЛЕНИЕ ЛИШНИХ ПРОБЕЛЬНЫХ СИМВОЛОВ ИЗ СТРОКИ ======//
size_t parse(char* source, char* dist, size_t max, size_t* read)
{
    assert(source != NULL);
    assert(dist != NULL);

    if(read != NULL) *read = 0;

    size_t offset = 0;
    char c = '\0';
    int space = 1;
    while((c = source[offset]) != '\0' && c != '\n' && c != EOF && offset < max)
    {
        if(space && c != ' ')
        {
            space = 0;
        }
        if(!space && c != ' ')
        {
            *(dist++) = c;
            if(read != NULL) (*read)++;
        }
        else if(!space && c == ' ')
        {
            break;
        }
        offset++;    
    }
    *dist = '\0';
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

//======= ПРОВЕРКА СУЩЕСТВОВАНИЯ МЕТКИ ПО ИМЕНИ =======//
bool correct_label(Assembler* ass, label* lbl)
{
    assert(ass);
    assert(lbl);

    for(size_t i = 0; i < MAX_LABELS; i++)
    {
        if(!strcmp(ass->labels[i].name, lbl->name)) return 1;
    }
    return 0;
}

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
        label fake = {{}, -1};
        memcpy(fake.name, ass->jumps[i].label, MAX_COMMAND_LENGHT);  
        if(!correct_label(ass, &fake)) return ASS_LABEL_INVALID;

        ass->offset = ass->jumps[i].offcet;
        bytecode_value(ass, get_label(ass, ass->jumps[i].label)->value);
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