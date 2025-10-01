#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

void initialize_buffer(char** buffer, size_t* size, FILE* input_file);
size_t initialize_text(char*** text, char* buffer, size_t size);
size_t file_len(FILE* file);
void assemble(char** text, size_t count, FILE* out_file);

int main()
{
    FILE* input_file = fopen("files/code.asm", "r");
    assert(input_file != NULL);

    char* buffer = NULL;
    char** text = NULL;

    size_t size = file_len(input_file);
    size_t count = 0;

    initialize_buffer(&buffer, &size, input_file);
    count = initialize_text(&text, buffer, size);

    fclose(input_file);

    // for(size_t i = 0; i < count; i++)
    // {
    //     printf("%s\n", text[i]);
    // }

    FILE* output_file = fopen("files/code.bcode", "w");
    assert(output_file != NULL);

    assemble(text, count, output_file);

    fclose(output_file);

    free(buffer);
    free(text);
}

void initialize_buffer(char** buffer, size_t* size, FILE* input_file)
{
    assert(buffer != NULL);
    assert(size != NULL);
    assert(input_file != NULL);

    char* buff = (char*)calloc(*size + 1, sizeof(char));
    size_t len = fread(buff, sizeof(char), *size, input_file);

    buff[len] = '\0';

    printf("File size: %lu\n", *size);
    printf("Buffer size: %lu\n", len);

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

    *text = (char**)calloc(count, sizeof(char*));
    assert(*text != NULL);

    (*text)[0] = buffer;

    size_t j = 1;
    for(size_t i = 0; i < size-1; i++)
    {
        if(buffer[i] == '\n')
        {       
            buffer[i] = '\0';
            (*text)[j] = buffer+i+1;
            j++;
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

void assemble(char** text, size_t count, FILE* out_file)
{
    assert(text != NULL);
    assert(out_file != NULL);

    char buff[10] = {};
    for(size_t i = 0; i < count; i++)
    {
        sscanf(text[i], "%10s", buff); //ограничение на ввод
        if(!strcmp(buff, "HLT")) fprintf(out_file, "%d\n", 0);
        else if(!strcmp(buff, "ADD")) fprintf(out_file, "%d\n", 1);
        else if(!strcmp(buff, "SUB")) fprintf(out_file, "%d\n", 2);
        else if(!strcmp(buff, "MUL")) fprintf(out_file, "%d\n", 3);
        else if(!strcmp(buff, "DIV")) fprintf(out_file, "%d\n", 4);
        else if(!strcmp(buff, "SQRT")) fprintf(out_file, "%d\n", 5);
        else if(!strcmp(buff, "OUT")) fprintf(out_file, "%d\n", 6);
        else if(!strcmp(buff, "PUSH"))
        {
            int value = 0;
            if(sscanf(text[i] + 4, "%d", &value, 10)) 
            {
                fprintf(out_file, "%d %d\n", 7, value); 
            }
            else
            {
                printf("Compilation error, gay\n");
            }
        }
    }
    //fixme strcmp into enum
}
