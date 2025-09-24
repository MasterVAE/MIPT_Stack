#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>

#include "stack.h"

StackErr_t StackVerify(Stack_t* stack)
{
    if(stack == NULL) return STACK_POINTER_NULL;

    if(stack->data == NULL) return STACK_DATA_NULL;

    if(stack->capacity <= 0) return STACK_CAPACITY_INCORRECT;

    if(stack->size > stack->capacity) return STACK_OVERFLOW;

    return NO_ERROR;
}

StackErr_t StackInit(Stack_t* stack, size_t capacity)
{
    stack->capacity = capacity;
    stack->size = 0;
    stack->data = (int*)calloc(capacity, sizeof(int));

    return StackVerify(stack);
}

StackErr_t StackPush(Stack_t* stack, int value)
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
        return STACK_OVERFLOW;
    }
}

int StackPop(Stack_t* stack, StackErr_t* err)
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
    printf("- - - Stack printing START - - -\n\n");
    if(stack != NULL)
    {   
        printf("Capacity: %lu\n", stack->capacity);
        printf("Size: %lu\n", stack->size);

        printf("Data: \n\n", stack->size);
        if(stack->data != NULL)
        {
            for(size_t i = 0; i < stack->capacity; i++)
            {
                if(i < stack->size)
                {
                    printf("*[%lu]: %d\n", i, stack->data[i]);
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
    printf("\n- - - Stack printing END - - -\n\n");
}