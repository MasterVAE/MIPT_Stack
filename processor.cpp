#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "code/stack.h"

#define POP_ERR(stack, err) StackPop(stack, err); if(*err != 0) StackDump(stack, *err);
#define INIT_ERR(stack, num) int err = StackInit(stack, num); if(err != 0) StackDump(stack, err);
#define PUSH_ERR(stack, value) {int error = StackPush(stack, value); if(error != 0) StackDump(stack, error);}

int input(Stack_t* stack, FILE* steam);

enum func_bycodes //вынести в словарь
{
    HLT = 0,
    PUSH = 1,
    ADD = 2,
    SUB = 3,
    MUL = 4,
    DIV = 5, 
    SQRT = 6,
    OUT = 7
};

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

    while (1)
    {
        if(input(&stack, input_file)) break;
    }
    StackDestroy(&stack);
    return 0;
}

int input(Stack_t* stack, FILE* stream) //fixme ENUM  
{
    int inp = 0;
    int err = 0;
    if(!fscanf(stream, "%d", &inp)) return 0;

    switch (inp)
    {
        case HLT: return 1;
        case ADD:
        {
            stack_type a = POP_ERR(stack, &err);
            stack_type b = POP_ERR(stack, &err);

            PUSH_ERR(stack, a + b);
            return 0;
        }
        case SUB:
        {
            stack_type b = POP_ERR(stack, &err);
            stack_type a = POP_ERR(stack, &err);

            PUSH_ERR(stack, a - b);
            return 0;
        }
        case MUL:
        {
            stack_type a = POP_ERR(stack, &err);
            stack_type b = POP_ERR(stack, &err);

            PUSH_ERR(stack, a * b);
            return 0;
        }
        case DIV:
        {
            stack_type b = POP_ERR(stack, &err);

            if(b == 0)
            {
                PUSH_ERR(stack, b);
                return 0;
            }

            stack_type a = POP_ERR(stack, &err); //fixme ошитбеа интерпритатора

            PUSH_ERR(stack, a/b);

            return 0;
        }
        case SQRT:
        {
            stack_type value = POP_ERR(stack, &err);
        
            PUSH_ERR(stack, (int)sqrt(value));
            return 0;
        }
        case PUSH:
        {
            stack_type value = 0;
            if(fscanf(stream, "%d", &value)) 
            {
                PUSH_ERR(stack, value);
            }
            return 0;
        }
        case OUT:
        {
            stack_type value = POP_ERR(stack, &err);
            printf("%d\n", value);  
            return 0;
        }
        default:
            //ошибка интерпритатора
            break;
    }
    //fixme strcmp into enum
    return 0;
}