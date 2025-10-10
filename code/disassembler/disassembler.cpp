#include <stdio.h>
#include <stdlib.h>

#include "../language.h"
#include "../lib.h"
#include "../colors.h"

#define DIS_MODE
#include "../commands.h"

#include "disassembler_life.h"
#include "disassembler_func.h"

#define OPEN_R(file_ptr, file_name) FILE* file_ptr = fopen(file_name, "r"); \
    if(file_ptr == NULL) {  error_printer(DIS_NULL_FILE); DISDestroy(&dis);   return 1;}

#define OPEN_W(file_ptr, file_name) FILE* file_ptr = fopen(file_name, "w+"); \
    if(file_ptr == NULL) {  error_printer(DIS_NULL_FILE); DISDestroy(&dis);   return 1;}

#define ARG_PARSE if(argc >= 2) {   input_file_name = argv[1];if(argc >= 3) \
    output_file_name = argv[2];}

#define CHECK(error) if(error != DIS_CORRECT){ error_printer(error);DISDestroy(&dis);return 1;}

dis_err disassemble(Disassembler* dis, FILE* out_file);
dis_err label_search(Disassembler* dis);
void insert_label(Disassembler* dis, FILE* output_file);

const char* input_file_name = "files/code.bcode";
const char* output_file_name = "files/code_disassembled.asm";

int main(int argc, char *argv[])
{
    ARG_PARSE

    Disassembler dis = {};
    DISInit(&dis);

    printf("Start disassembling: %s -> %s\n", input_file_name, output_file_name);

    OPEN_R(input_file, input_file_name)

    initialize_buffer(&dis.buffer, &dis.buffer_size, input_file);

    fclose(input_file);

    OPEN_W(output_file, output_file_name)

    dis_err error = label_search(&dis);
    CHECK(error)
    
    error = disassemble(&dis, output_file);
    CHECK(error)

    fclose(output_file);
    DISDestroy(&dis);


    printf(GREEN "Success disassembling: " CLEAN "%s -> %s\n", input_file_name, output_file_name);
    return 0;
}

dis_err label_search(Disassembler* dis)
{
    if(dis->buffer == NULL) return DIS_NULL_BUFFER;
    if(dis->buffer_size == 0) return DIS_EMPTY_PROGRAMM;

    for(dis->offset = 0; dis->offset < dis->buffer_size;)
    {
        if(dis->offset + sizeof(COMMAND_TYPE) > dis->buffer_size)   return DIS_SYNTAX_ERROR;
        int comm = debytecode_int(dis->buffer + dis->offset, sizeof(COMMAND_TYPE));
        bool found = 0;
        for(size_t j = 0; j < COMMANDS_COUNT; j++)
        {    
            if(COMMANDS[j].num != comm) continue;
            found = 1;
            if(COMMANDS[j].dis_func != dis_jump)
            {
                dis->offset += COMMANDS[j].byte_size;
                break;
            }
            if(dis->offset + sizeof(COMMAND_TYPE) + sizeof(VALUE_TYPE) > dis->buffer_size)
            {
                return DIS_ARGUMENT_INVALID;
            }
            int value = debytecode_int(dis->buffer + dis->offset + sizeof(COMMAND_TYPE), sizeof(VALUE_TYPE));
            for(size_t k = 0; k < MAX_LABELS; k++)
            {
                if(dis->labels[k] == -1 || dis->labels[k] == value)
                {
                    dis->labels[k] = value;
                    break;
                }
            }
            dis->offset += COMMANDS[j].byte_size;
            break;
        }
        if(!found) return DIS_UNKNOWN_COMMAND;
    }
    return DIS_CORRECT;
}

void insert_label(Disassembler* dis, FILE* output_file)
{
    for(size_t i = 0; i < MAX_LABELS; i++)
    {
        if(dis->labels[i] == (int)dis->offset)
        {
            fprintf(output_file, "LABEL %lu\n", i);
            return;
        }
    }
}

dis_err disassemble(Disassembler* dis, FILE* out_file)
{
    if(dis->buffer == NULL) return DIS_NULL_BUFFER;
    if(out_file == NULL) return DIS_NULL_FILE;
    if(dis->buffer_size == 0) return DIS_EMPTY_PROGRAMM;

    for(dis->offset = 0; dis->offset < dis->buffer_size; dis->offset+=sizeof(COMMAND_TYPE))
    {
        insert_label(dis, out_file);
        if(dis->offset + sizeof(COMMAND_TYPE) > dis->buffer_size)   return DIS_SYNTAX_ERROR;
        int comm = debytecode_int(dis->buffer + dis->offset, sizeof(COMMAND_TYPE));
        
        bool found = 0;
        for(size_t j = 0; j < COMMANDS_COUNT; j++)
        {    
            if(COMMANDS[j].num == comm)
            {
                COMMANDS[j].dis_func(dis, j, out_file);
                found = 1;
                break;
            }
        }
        if(!found)  return DIS_UNKNOWN_COMMAND;
    }
    return DIS_CORRECT;
}

