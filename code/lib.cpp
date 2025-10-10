#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "lib.h"

void initialize_buffer(char** buffer, size_t* size, FILE* input_file)
{
    assert(buffer != NULL);
    assert(size != NULL);
    assert(input_file != NULL);

    *size = file_len(input_file);

    char* buff = (char*)calloc(*size + 2, sizeof(char));
    size_t len = fread(buff, sizeof(char), *size, input_file);

    buff[len] = '\0';
    buff[len+1] = '\0';

    *size = len;
    *buffer = buff;
}

size_t file_len(FILE* file)
{
    struct stat file_info;
    fstat(fileno(file), &file_info);
    return (size_t)file_info.st_size;
}

int debytecode_int(char* code, size_t size)
{
    int value;
    memcpy(&value, code, size);
    return value;
}