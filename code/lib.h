#ifndef LIB_H_
#define LIB_H_

#include "stdio.h"

typedef struct string
{
    char str[10];
}   string;

size_t file_len(FILE* file);
void initialize_buffer(char** buffer, size_t* size, FILE* input_file);
int debytecode_int(char* code, size_t size);
int IsError(int error, int check);
string itos(unsigned value, unsigned base, size_t len);

#endif