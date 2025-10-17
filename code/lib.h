#ifndef LIB_H_
#define LIB_H_

#include "stdio.h"

const size_t MAX_STRING_LENGHT = 100;

typedef struct string
{
    char str[MAX_STRING_LENGHT];
}   string;

size_t file_len(FILE* file);
char* initialize_buffer(char** buffer, size_t* size, FILE* input_file);
int debytecode_int(char* code, size_t size);
int IsError(int error, int check);
string itos(unsigned value, unsigned base, size_t len);

#endif //LIB_H_