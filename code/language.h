#ifndef LANGUAGE_H_
#define LANGUAGE_H_

enum funcs
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

const int command_size = 2;
const int value_size = 8;

#endif