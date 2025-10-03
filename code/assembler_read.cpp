#include <assert.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

#include "assembler_read.h"

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

size_t initialize_text(char*** text, char* buffer, size_t size)
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

    *text = (char**)calloc(count * (arg_limit + 1), sizeof(char*));
    assert(text != NULL);

    (*text)[0] = buffer;

    size_t j = 1;
    for(size_t i = 0; i < size-1; i++)
    {
        if(buffer[i] == '\n')
        {       
            buffer[i] = '\0';
            (*text)[j * (arg_limit + 1)] = buffer+i+1;
            j++;
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
        for(j = 0; j < arg_limit; j++)
        {
            off += parse((*text)[i * (arg_limit + 1)] + off, (*text)[i * (arg_limit + 1) + j], 10, &read);
            if(read == 0)
            {
                (*text)[i * (arg_limit + 1) + j] = NULL;
                break;
            }
            if(i < count-1 && (*text)[i * (arg_limit + 1)] + off + 1 < (*text)[(i+1) * (arg_limit + 1)]) off++;
            (*text)[i * (arg_limit + 1) + j + 1] = (*text)[i * (arg_limit + 1)] + off;
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