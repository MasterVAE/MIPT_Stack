#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "assembler_read.h"
#include "assembler_func.h"
#include "../language.h"

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
        size_t off = parse((*text)[i].line, (*text)[i].line, 10, &read);

        if((i < count-1 && (*text)[i].line + off + 1 < (*text)[i+1].line) || i == count-1) off++;
        for(j = 0; j < ARG_LIMIT; j++)
        {
            size_t add = parse((*text)[i].line + off, (*text)[i].line + off, 10, &read);
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

int ASSInit(Assembler* ass)
{
    if(ass == NULL) return 1;
    ass->offset = 0;
    ass->lines_count = 0;
    ass->line_offset = 0;
    ass->buffer_size = buffer_start_size;
    ass->buffer = (char*)calloc(ass->buffer_size, sizeof(char));
    ass->current_jump_memory = 0;
    
    for(size_t i = 0; i < MAX_LABELS; ass->labels[i++] = -1);
    for(size_t i = 0; i < MAX_JUMPS; ass->jumps[i++] = {-1, 0});

    return 0;
}

void ASSDestroy(Assembler* ass)
{
    if(ass == NULL) return;
    free(ass->buffer);
    free(ass->text);
    ass->buffer = NULL;
    ass->text = NULL;
}

bool correct_label(int label)
{
    if(label < 0 || label >= (int)MAX_LABELS) return 0;
    return 1;
}

int ASSPostCompile(Assembler* ass)
{
    size_t max_offset = ass->offset;
    if(ass == NULL) return ASS_ASSEMBLER_NULL;
    for(size_t i = 0; i < ass->current_jump_memory; i++)
    {
        if(!correct_label(ass->jumps[i].label)) return ASS_LABEL_INVALID;

        ass->offset = ass->jumps[i].offcet;
        bytecode_value(ass, ass->labels[ass->jumps[i].label]);
    }
    ass->offset = max_offset;
    return ASS_CORRECT;
}