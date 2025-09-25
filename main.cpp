#include <stdio.h>

#include "code/stack.h"

int main()
{
    const char* StackErrorNames[4] = {"NO_ERROR", "STACK_POINTER_NULL", "STACK_DATA_NULL", "STACK_SIZE_INCORECT"};

    StackErr_t err = NO_ERROR;
    Stack_t st1;
    const size_t cap1 = 2;

    err = StackInit(&st1, cap1);
    if(err != 0) printf("Error %s occured\n", StackErrorNames[err]);
    StackDump(&st1);
    err = StackPush(&st1, 10);
    if(err != 0) printf("Error %s occured\n", StackErrorNames[err]);
    StackDump(&st1);
    err = StackPush(&st1, 20);
    if(err != 0) printf("Error %s occured\n", StackErrorNames[err]);
    StackDump(&st1);
    err = StackPush(&st1, 30);
    if(err != 0) printf("Error %s occured\n", StackErrorNames[err]);
    StackDump(&st1);
    int value1 = StackPop(&st1, &err);
    if(err != 0) printf("Error %s occured\n", StackErrorNames[err]);
    printf("value: %d\n", value1);
    StackDump(&st1);
    int value2 = StackPop(&st1, &err);
    if(err != 0) printf("Error %s occured\n", StackErrorNames[err]);
    printf("value: %d\n", value2);
    StackDump(&st1);
    int value3 = StackPop(&st1, &err);
    if(err != 0) printf("Error %s occured\n", StackErrorNames[err]);
    printf("value: %d\n", value3);
    StackDump(&st1);
    int value4 = StackPop(&st1, &err);
    if(err != 0) printf("Error %s occured\n", StackErrorNames[err]);
    StackDump(&st1);
    
    printf("value: %d\n", value4);


    StackDestroy(&st1);
    return 0;
}