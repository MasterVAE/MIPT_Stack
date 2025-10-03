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
    assert(*text != NULL);

    (*text)[0].str = buffer;

    size_t j = 1;
    for(size_t i = 0; i < size-1; i++)
    {
        if(buffer[i] == '\n')
        {       
            buffer[i] = '\0';
            (*text)[j].str = buffer+i+1;
            (*text)[j - 1].len = (size_t)((*text)[j].str - (*text)[j-1].str - 1);
            j++;
        }
    } 
    (*text)[j - 1].len = (size_t)(buffer + size - (*text)[j-1].str);
    return count;
}

size_t file_len(FILE* file)
{
    struct stat file_info;
    fstat(fileno(file), &file_info);
    return (size_t)file_info.st_size;
}