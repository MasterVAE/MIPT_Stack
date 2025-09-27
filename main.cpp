#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "code/stack.h"

int input(Stack_t* stack);

int main()
{
    StackErr_t err = NO_ERROR;
    Stack_t stack;
    err = StackInit(&stack, 0);
    if(err != 0)
    { 
        printf("Error %d occured\n", err);
        StackDump(&stack);
    }

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
    StackErr_t err;
    if(!scanf("%s", buff)) return 0;
    
    if(strcmp(buff, "HLT") == 0) return 1;

    if(strcmp(buff, "PUSH") == 0)
    {
        stack_type d = 0;
        if(scanf("%d", &d))
        {
            err = StackPush(stack, d);
            if(err != 0)
            {
                printf("Error %d occured\n", err);
                StackDump(stack);
            }
        }
        return 0;
    }
    
    if(strcmp(buff, "ADD") == 0)
    {

        stack_type a = StackPop(stack, &err);
        if(err != 0)
        { 
            printf("Error %d occured\n", err);
            StackDump(stack);\
        }
        stack_type b = StackPop(stack, &err);
        if(err != 0)
        { 
            printf("Error %d occured\n", err);
            StackDump(stack);\
        }
        err = StackPush(stack, a + b);
        if(err != 0)
        {
            printf("Error %d occured\n", err);
            StackDump(stack);
        }
        return 0;
    }

    if(strcmp(buff, "SUB") == 0)
    {

        stack_type b = StackPop(stack, &err);
        if(err != 0)
        { 
            printf("Error %d occured\n", err);
            StackDump(stack);\
        }
        stack_type a = StackPop(stack, &err);
        if(err != 0)
        { 
            printf("Error %d occured\n", err);
            StackDump(stack);
        }
        err = StackPush(stack, a - b);
        if(err != 0)
        {
            printf("Error %d occured\n", err);
            StackDump(stack);
        }
        return 0;
    }

    if(strcmp(buff, "MUL") == 0)
    {

        stack_type a = StackPop(stack, &err);
        if(err != 0)
        { 
            printf("Error %d occured\n", err);
            StackDump(stack);
        }
        stack_type b = StackPop(stack, &err);
        if(err != 0)
        { 
            printf("Error %d occured\n", err);
            StackDump(stack);
        }
        err = StackPush(stack, a * b);
        if(err != 0)
        {
            printf("Error %d occured\n", err);
            StackDump(stack);
        }
        return 0;
    }
    if(strcmp(buff, "DIV") == 0)
    {

        stack_type b = StackPop(stack, &err);
        if(err != 0)
        { 
            printf("Error %d occured\n", err);
            StackDump(stack);\
        }
        stack_type a = StackPop(stack, &err);
        if(err != 0)
        { 
            printf("Error %d occured\n", err);
            StackDump(stack);
        }
        err = StackPush(stack, a/b);
        if(err != 0)
        {
            printf("Error %d occured\n", err);
            StackDump(stack);
        }
        return 0;
    }
    if(strcmp(buff, "OUT") == 0)
    {

        stack_type value = StackPop(stack, &err);
        if(err != 0)
        { 
            printf("Error %d occured\n", err);
            StackDump(stack);
        }
        printf("%d\n", value);
        return 0;
    }
    return 0;
}