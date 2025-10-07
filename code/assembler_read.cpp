#include <assert.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <assert.h>

#include "assembler_read.h"
#include "language.h"

void initialize_buffer(char** buffer, size_t* size, FILE* input_file)
{
    assert(buffer != NULL);
    assert(size != NULL);
    assert(input_file != NULL);

    *size = file_len(input_file);

    char* buff = (char*)calloc(*size + 1, sizeof(char));
    size_t len = fread(buff, sizeof(char), *size, input_file);

    buff[len] = '\0';

    *size = len;
    *buffer = buff;
}

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
        size_t off = 0;
        off = parse((*text)[i].line, (*text)[i].line, 10, &read);
        
        if(i < count-1 && (*text)[i].line + off + 1 < (*text)[i+1].line) off++;
        for(j = 0; j < arg_limit; j++)
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

size_t file_len(FILE* file)
{
    struct stat file_info;
    fstat(fileno(file), &file_info);
    return (size_t)file_info.st_size;
}

size_t parse(char* source, char* dist, size_t max, size_t* read)
{
    assert(source != NULL);
    assert(dist != NULL);

    if(read != NULL) *read = 0;

    size_t offcet = 0;
    char c = '\0';
    int space = 1;
    while((c = source[offcet]) != '\0' && c != '\n' && c != EOF && offcet < max)
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
        offcet++;    
    }
    *dist = '\0';
    return offcet;
}