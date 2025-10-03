#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "code/stack.h"
#include "code/language.h"
#include "code/assembler_read.h"

#define POP_ERR(stack, err) StackPop(stack, err); if(*err != 0) StackDump(stack, *err);
#define INIT_ERR(stack, num) int err = StackInit(stack, num); if(err != 0) StackDump(stack, err);
#define PUSH_ERR(stack, value) {int error = StackPush(stack, value); if(error != 0) StackDump(stack, error);}

typedef enum interpretator_errors
{
    INT_CORRECT = 0,
    INT_HALT,
    INT_DIVISION_BY_ZERO,
    INT_INVALID_COMMAND
} i_err;

int input(Stack_t* stack, char* buffer, size_t* offcet);
int get_int(char* buffer, size_t len);


int main()
{
    Stack_t stack;
    INIT_ERR(&stack, 0);
    
    FILE* input_file = fopen("files/code.bcode", "r");
    if(input_file == NULL)
    {
        StackDestroy(&stack);
        return 1;
    }
    char* buffer = NULL;
    size_t size = 0;
    
    initialize_buffer(&buffer, &size, input_file);
    fclose(input_file);

    //BREAKING STACK
    //((char*)stack.data)[2] = '7';

    size_t offcet = 0;
    while (1)
    {
        int error = input(&stack, buffer, &offcet);
        if(error == INT_HALT) break;
        else if(error == INT_DIVISION_BY_ZERO) fprintf(stderr, "Interpretator error: DIVISION BY ZERO\n"); 
        else if(error == INT_INVALID_COMMAND) fprintf(stderr, "Interpretator error: INVALID COMMAND\n"); 
    }
    StackDestroy(&stack);
    free(buffer);
    return 0;
}

int input(Stack_t* stack, char* buffer, size_t* offcet)
{
    int inp = get_int(buffer + *offcet, command_size);
    *offcet += command_size;
    int err = 0;

    switch (inp)
    {
        case HLT: return INT_HALT;
        case ADD:
        {
            stack_type a = POP_ERR(stack, &err);
            stack_type b = POP_ERR(stack, &err);

            PUSH_ERR(stack, a + b);
            return INT_CORRECT;
        }
        case SUB:
        {
            stack_type b = POP_ERR(stack, &err);
            stack_type a = POP_ERR(stack, &err);

            PUSH_ERR(stack, a - b);
            return INT_CORRECT;
        }
        case MUL:
        {
            stack_type a = POP_ERR(stack, &err);
            stack_type b = POP_ERR(stack, &err);

            PUSH_ERR(stack, a * b);
            return INT_CORRECT;
        }
        case DIV:
        {
            stack_type b = POP_ERR(stack, &err);

            if(b == 0)
            {
                PUSH_ERR(stack, b);
                return INT_DIVISION_BY_ZERO;
            }

            stack_type a = POP_ERR(stack, &err);
            PUSH_ERR(stack, a/b);
            return INT_CORRECT;
        }
        case SQRT:
        {
            stack_type value = POP_ERR(stack, &err);
            PUSH_ERR(stack, (int)sqrt(value));
            return INT_CORRECT;
        }
        case PUSH:
        {
            stack_type value = get_int(buffer + *offcet, value_size);
            *offcet += value_size;
            PUSH_ERR(stack, value);
            return INT_CORRECT;
        }
        case OUT:
        {
            stack_type value = POP_ERR(stack, &err);
            printf("%d\n", value);  
            return INT_CORRECT;
        }
        default:
            return INT_INVALID_COMMAND;
    }
    return INT_CORRECT;
}


int get_int(char* buffer, size_t len)
{
    int ans = 0;
    for(size_t i = 0; i < len; i++)
    {
        ans *= 2;
        ans += buffer[i] - '0';
    }
    return ans;
}