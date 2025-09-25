#include <stdlib.h>

#include "stack.h"

StackErr_t StackVerify(Stack_t* stack)
{
    if(stack == NULL) return STACK_POINTER_NULL;

    if(stack->data == NULL) return STACK_DATA_NULL;

    if(stack->size > stack->capacity) return STACK_SIZE_INCORECT;

    return NO_ERROR;
}

StackErr_t StackInit(Stack_t* stack, size_t capacity)
{
    stack->capacity = capacity;
    stack->size = 0;
    stack->data = (stack_type*)calloc(capacity, sizeof(stack_type));

    return StackVerify(stack);
}

StackErr_t StackPush(Stack_t* stack, stack_type value)
{
    StackErr_t err = StackVerify(stack);
    if(err) return err;

    if(stack->size < stack->capacity)
    {
        stack->data[stack->size++] = value;
        return NO_ERROR;
    }
    else 
    {
        if(stack->capacity == 0)
        {
            stack->capacity = 1;
            stack->data = (stack_type*)calloc(1, sizeof(stack_type));
        }
        else
        {
            stack->capacity *= 2;
            stack->data = (stack_type*)realloc(stack->data, stack->capacity * sizeof(stack_type));
        }

        err = StackVerify(stack);
        if(err) return err;

        stack->data[stack->size++] = value;
        return NO_ERROR;
    }
}

stack_type StackPop(Stack_t* stack, StackErr_t* err)
{
    StackErr_t error = StackVerify(stack);
    if(error)
    {
        if(err != NULL) *err = error;
        return 0;
    }
    if(stack->size > 0)
    {
        if(err != NULL) *err = NO_ERROR;
        stack->size--;
        return stack->data[stack->size];
    }
    else
    {
        if(err != NULL) *err = STACK_SIZE_INCORECT;
        return 0;
    }
    
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

void StackDump(Stack_t* stack)
{
    printf("\n\033[31m- - - Stack printing START - - -\033[0m\n\n");
    if(stack != NULL)
    {   
        printf("Capacity: %lu\n", stack->capacity);
        printf("Size: %lu\n", stack->size);

        printf("Data: \n\n");
        if(stack->data != NULL)
        {
            for(size_t i = 0; i < stack->capacity; i++)
            {
                if(i < stack->size)
                {
                    printf("*[%lu]: \033[32m%d\033[0m\n", i, stack->data[i]);
                }
                else
                {
                    printf(" [%lu]: %d\n", i, stack->data[i]);
                }
            }
        }
        else
        {
            printf("NULL data\n");
        }
    }
    else
    {
        printf("NULL Stack\n");
    }
    printf("\n\033[31m- - - Stack printing END - - -\033[0m\n\n");
}