#ifndef ASSEMBLER_READ_H_
#define ASSEMBLER_READ_H_

#include <stdio.h>

void initialize_buffer(char** buffer, size_t* size, FILE* input_file);
size_t initialize_text(char*** text, char* buffer, size_t size);
size_t file_len(FILE* file);
size_t parse(char* source, char* dist, size_t max, size_t* read = NULL);

#endif