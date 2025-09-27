#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "code/stack.h"

int input(Stack_t* stack);

int main()
{
    int err = 0b0000;
    Stack_t stack;
    err = StackInit(&stack, 0);
    if(err != 0) StackDump(&stack, err);

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
        stack_type d = 0;
        if(scanf("%d", &d)) 
        {
            err = StackPush(stack, d);
            if(err != 0) StackDump(stack, err);
        }
        return 0;
    }
    
    if(strcmp(buff, "ADD") == 0)
    {

        stack_type a = StackPop(stack, &err);
        if(err != 0) StackDump(stack, err);

        stack_type b = StackPop(stack, &err);
        if(err != 0) StackDump(stack, err);

        err = StackPush(stack, a + b);
        if(err != 0) StackDump(stack, err);

        return 0;
    }

    if(strcmp(buff, "SUB") == 0)
    {
        stack_type b = StackPop(stack, &err);
        if(err != 0) StackDump(stack, err);

        stack_type a = StackPop(stack, &err);
        if(err != 0) StackDump(stack, err);

        err = StackPush(stack, a - b);
        if(err != 0) StackDump(stack, err);

        return 0;
    }

    if(strcmp(buff, "MUL") == 0)
    {
        stack_type a = StackPop(stack, &err);
        if(err != 0) StackDump(stack, err);

        stack_type b = StackPop(stack, &err);
        if(err != 0) StackDump(stack, err);

        err = StackPush(stack, a * b);
        if(err != 0) StackDump(stack, err);

        return 0;
    }
    if(strcmp(buff, "DIV") == 0)
    {

        stack_type b = StackPop(stack, &err);
        if(err != 0) StackDump(stack, err);

        if(b == 0)
        {
            err = StackPush(stack, b);
            if(err != 0) StackDump(stack, err);
            return 0;
        }

        stack_type a = StackPop(stack, &err);
        if(err != 0) StackDump(stack, err);

        
        err = StackPush(stack, a/b);
        if(err != 0) StackDump(stack, err);

        return 0;
    }
    if(strcmp(buff, "SQRT") == 0)
    {
        stack_type value = StackPop(stack, &err);
        if(err != 0) StackDump(stack, err);
        
        err = StackPush(stack, (int)sqrt(value));
        if(err != 0) StackDump(stack, err);
        return 0;
    }
    if(strcmp(buff, "OUT") == 0)
    {
        stack_type value = StackPop(stack, &err);
        if(err != 0) StackDump(stack, err);
        
        printf("%d\n", value);  
        return 0;
    }
    return 0;
}