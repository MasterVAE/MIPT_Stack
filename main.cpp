#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "code/stack.h"

#define POP_ERR(stack, err) StackPop(stack, err); if(*err != 0) StackDump(stack, *err);
#define INIT_ERR(stack, num) int err = StackInit(stack, num); if(err != 0) StackDump(stack, err);
#define PUSH_ERR(stack, value) int error = StackPush(stack, value); if(error != 0) StackDump(stack, error);

int input(Stack_t* stack);

int main()
{
    Stack_t stack;
    INIT_ERR(&stack, 0);

    while (1)
    {
        if(input(&stack)) break;
    }
    StackDestroy(&stack);
    return 0;
}

int input(Stack_t* stack)
{
    char buff[10];
    int err;
    if(!scanf("%s", buff)) return 0;
    
    if(strcmp(buff, "HLT") == 0) return 1;

    if(strcmp(buff, "PUSH") == 0)
    {
        stack_type value = 0;
        if(scanf("%d", &value)) 
        {
            PUSH_ERR(stack, value);
        }
        return 0;
    }
    
    if(strcmp(buff, "ADD") == 0)
    {
        stack_type a = POP_ERR(stack, &err);
        stack_type b = POP_ERR(stack, &err);

        PUSH_ERR(stack, a + b);
        return 0;
    }

    if(strcmp(buff, "SUB") == 0)
    {
        stack_type b = POP_ERR(stack, &err);
        stack_type a = POP_ERR(stack, &err);

        PUSH_ERR(stack, a - b);
        return 0;
    }

    if(strcmp(buff, "MUL") == 0)
    {
        stack_type a = POP_ERR(stack, &err);
        stack_type b = POP_ERR(stack, &err);

        PUSH_ERR(stack, a * b);
        return 0;
    }
    if(strcmp(buff, "DIV") == 0)
    {
        stack_type b = POP_ERR(stack, &err);

        if(b == 0)
        {
            PUSH_ERR(stack, b);
            return 0;
        }

        stack_type a = POP_ERR(stack, &err);

        PUSH_ERR(stack, a/b);

        return 0;
    }
    if(strcmp(buff, "SQRT") == 0)
    {
        stack_type value = POP_ERR(stack, &err);
        
        PUSH_ERR(stack, (int)sqrt(value));
        return 0;
    }
    if(strcmp(buff, "OUT") == 0)
    {
        stack_type value = POP_ERR(stack, &err);
        printf("%d\n", value);  
        return 0;
    }
    return 0;
}