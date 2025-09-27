#include <stdlib.h>

#include "stack.h"

void ErrorParser(int error)
{
    if((error & 0b1000) > 0) printf("Error: stack NULL");
    if((error & 0b0100) > 0) printf("Error: data NULL");
    if((error & 0b0010) > 0) printf("Error: stack overflow");
    if((error & 0b0001) > 0) printf("Error: data corrupted");
}

int StackVerify(Stack_t* stack)
{
    int i = 0;
    if(stack == NULL) return i | 0b1000;

    if(stack->size > stack->capacity) i = i | 0b0010;

    if(stack->data == NULL) return i | 0b0100;

    if(stack->data[0] != 0x6BADF00D || stack->data[1 + stack->capacity] != 0x7BADF00D) return i | 0b0001;

    return i;
}

int StackInit(Stack_t* stack, size_t capacity)
{
    if(stack == NULL) return 0b1000;

    stack->capacity = capacity;
    stack->size = 0;
    stack->data = (stack_type*)calloc(capacity+2, sizeof(stack_type));
    stack->data[0] = 0x6BADF00D;
    stack->data[1 + capacity] = 0x7BADF00D;

    return StackVerify(stack);
}

int StackPush(Stack_t* stack, stack_type value)
{
    int err = StackVerify(stack);
    if(err != 0) return err;

    if(stack->size < stack->capacity)
    {
        stack->data[1 + stack->size++] = value;
        return 0b0000;
    }

    if(stack->capacity == 0)
    {
        stack->capacity = 1;
        stack->data = (stack_type*)realloc(stack->data, 3 * sizeof(stack_type));
        stack->data[0] = 0x6BADF00D;
        stack->data[1 + stack->capacity] = 0x7BADF00D;
    }
    else
    {
        stack->capacity *= 2;
        stack->data = (stack_type*)realloc(stack->data, (stack->capacity + 2) * sizeof(stack_type));
        stack->data[1 + stack->capacity] = 0x7BADF00D;
    }

    err = StackVerify(stack);
    if(err != 0) return err;

    stack->data[1 + stack->size++] = value;
    return 0b0000;
    
}

stack_type StackPop(Stack_t* stack, int* err)
{
    int error = StackVerify(stack);
    if(error != 0)
    {
        if(err != NULL) *err = error;
        return 0;
    }
    if(stack->size > 0)
    {
        if(err != NULL) *err = 0b0000;
        stack->size--;
        return stack->data[1 + stack->size];
    }

    if(err != NULL) *err = 0b0010;
    return 0;
    
}

void StackDestroy(Stack_t* stack)
{
    if(stack != NULL)
    {
        if(stack->data != NULL)
        {
            free(stack->data);
        }
        stack->capacity = 0;
        stack->size = 0;
    }
}
#define RED "\033[31m"
#define CLEAN "\033[0m"
#define GREEN "\033[32m"


void StackDump(Stack_t* stack, int error)
{


    printf("\n" RED "- - - Stack printing START - - - " CLEAN "\n");
    ErrorParser(error);
    if(stack == NULL)
    {   
        printf("NULL Stack\n");
        printf("\n" RED "- - - Stack printing END - - -" CLEAN "\n\n");
        return;
    }
        
    printf("Capacity: %lu\n", stack->capacity);
    printf("Size: %lu\n\n", stack->size);

    if(stack->data == NULL)
    {
        printf("NULL data\n");
        printf("\n" RED "- - - Stack printing END - - -" CLEAN "\n\n");
        return;
    }
    if(stack->data[0] == 0x6BADF00D) printf("Guard 1: " GREEN "%d" CLEAN "\n", stack->data[0]);
    else printf("Guard 1: " RED "%d" CLEAN "\n", stack->data[0]);

    if(stack->data[stack->capacity+1] == 0x7BADF00D) printf("Guard 2: " GREEN "%d" CLEAN "\n", stack->data[stack->capacity+1]);
    else printf("Guard 2: " RED "%d" CLEAN "\n", stack->data[stack->capacity+1]);

    printf("Data: \n\n");
    for(size_t i = 1; i < stack->capacity+1; i++)
    {
        if(i < stack->size)
        {
            printf("*[%lu]: " GREEN "%d" CLEAN "\n", i, stack->data[i]);
        }
        else
        {
            printf(" [%lu]: %d\n", i, stack->data[i]);
        }
    }

    printf("\n" RED "- - - Stack printing END - - -" CLEAN "\n\n");
}


