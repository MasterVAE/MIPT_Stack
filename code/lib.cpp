#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "lib.h"

static size_t FileLen(FILE* file);

char* InitializeBuffer(size_t* size, FILE* input_file)
{
    assert(size);
    assert(input_file);

    *size = FileLen(input_file);

    char* buff = (char*)calloc(*size + 2, sizeof(char));
    assert(buff);
    
    size_t len = fread(buff, sizeof(char), *size, input_file);

    buff[len] = '\0';
    buff[len+1] = '\0';

    *size = len;

    return buff;
}
static size_t FileLen(FILE* file)
{
    assert(file);

    struct stat file_info;
    fstat(fileno(file), &file_info);

    return (size_t)file_info.st_size;
}

int DebytecodeInt(char* code, size_t size)
{
    assert(code);

    int value = 0;
    memcpy(&value, code, size);

    return value;
}

int IsError(int error, int check)
{
    return error & check;
}

string itos(unsigned value, unsigned base, size_t len)
{
    string ans = {};
    for(int i = (int)len-1; i >= 0; i--)
    {
        unsigned vl = value % base;
        if(vl < 10)    ans.str[i] = (char)(vl + '0');
        else            ans.str[i] = (char)(vl + 'A');
        value /= base;
    }
    
    return ans;
}