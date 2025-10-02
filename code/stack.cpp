#include <stdlib.h>

#include "stack.h"

bool IsError(int error, StackError check)
{
    return error & check;
}

void ErrorParser(int error)
{
    if(IsError(error, StackNull)) fprintf(ERROR_STREAM, "Error: stack NULL\n");
    if(IsError(error, CapacityInvalid)) fprintf(ERROR_STREAM, "Error: capacity invalid\n");
    if(IsError(error, DataNull)) fprintf(ERROR_STREAM, "Error: data NULL\n");
    if(IsError(error, StackOverflow)) fprintf(ERROR_STREAM, "Error: stack overflow\n");
    if(IsError(error, DataCorrupted)) fprintf(ERROR_STREAM, "Error: data corrupted\n");
}

int StackVerify(Stack_t* stack)
{
    int error = Verified;

    if(stack == NULL) return error | StackNull;
    if(stack->capacity == 0) error |= CapacityInvalid;
    if(stack->size > stack->capacity) error |= StackOverflow;
    if(stack->data == NULL) return error | DataNull;
    if(stack->data[0] != SHIELD_START || stack->data[1 + stack->capacity] != SHIELD_END) return error | DataCorrupted;

    return error;
}

int StackInit(Stack_t* stack, size_t capacity)
{
    if(stack == NULL) return StackNull;

    if(capacity == 0) capacity = 1;
    stack->capacity = capacity;
    stack->size = 0;
    stack->data = (stack_type*)calloc(capacity + 2, sizeof(stack_type));
    stack->data[0] = SHIELD_START;
    stack->data[1 + capacity] = SHIELD_END;

    return StackVerify(stack);
}

int StackPush(Stack_t* stack, stack_type value)
{
    int err = StackVerify(stack); if(err != 0) return err;

    if(stack->size < stack->capacity)
    {
        stack->data[1 + stack->size++] = value;
        err = StackVerify(stack); if(err != 0) return err;
        return Verified;
    }

    stack->capacity *= STACK_MULTIPLIER;
    stack->data = (stack_type*)realloc(stack->data, (stack->capacity + 2) * sizeof(stack_type));
    stack->data[1 + stack->capacity] = SHIELD_END;
    
    err = StackVerify(stack); if(err != 0) return err;

    stack->data[1 + stack->size++] = value;

    err = StackVerify(stack); if(err != 0) return err;
    return Verified;
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
        if(err != NULL) *err = Verified;
        stack->size--;
        return stack->data[1 + stack->size];
    }

    if(err != NULL) *err = StackOverflow;
    return 0;
}

void StackDestroy(Stack_t* stack)
{
    if(stack != NULL)
    {
        free(stack->data);
        stack->data = NULL;
        stack->capacity = 0;
        stack->size = 0;
    }
}

void StackDump(Stack_t* stack, int error)//сломать стак разными способами в мейне
{
    fprintf(ERROR_STREAM,"\n" RED "- - - Stack printing START - - - " CLEAN "\n");
    ErrorParser(error);
    if(stack == NULL)
    {   
        fprintf(ERROR_STREAM, "NULL Stack\n");
        fprintf(ERROR_STREAM, "\n" RED "- - - Stack printing END - - -" CLEAN "\n\n");
        return;
    }
        
    fprintf(ERROR_STREAM, "Capacity: %lu\n", stack->capacity);
    fprintf(ERROR_STREAM, "Size: %lu\n\n", stack->size);

    if(stack->data == NULL)
    {
        fprintf(ERROR_STREAM, "NULL data\n");
        fprintf(ERROR_STREAM, "\n" RED "- - - Stack printing END - - -" CLEAN "\n\n");
        return;
    }
    if(stack->data[0] == SHIELD_START) fprintf(ERROR_STREAM, "Guard 1: " GREEN "%d" CLEAN "\n", stack->data[0]);
    else fprintf(ERROR_STREAM, "Guard 1: " RED "%d" CLEAN "\n", stack->data[0]);

    if(stack->data[stack->capacity+1] == SHIELD_END) fprintf(ERROR_STREAM, "Guard 2: " GREEN "%d" CLEAN "\n", stack->data[stack->capacity+1]);
    else fprintf(ERROR_STREAM, "Guard 2: " RED "%d" CLEAN "\n", stack->data[stack->capacity+1]);

    fprintf(ERROR_STREAM, "Data: \n\n");
    for(size_t i = 1; i < stack->capacity+1; i++)
    {
        if(i < stack->size)
        {
            fprintf(ERROR_STREAM, "*[%lu]: " GREEN "%d" CLEAN "\n", i, stack->data[i]);
        }
        else
        {
            fprintf(ERROR_STREAM, " [%lu]: %d\n", i, stack->data[i]);
        }
    }

    fprintf(ERROR_STREAM, "\n" RED "- - - Stack printing END - - -" CLEAN "\n\n");
}