#ifndef LANGUAGE_H_
#define LANGUAGE_H_

#define COMMAND_TYPE char
#define VALUE_TYPE int

typedef int stack_type;

const char* const regs[] = 
{
    "SR1",
    "SR2",
    "SR3",
    "SR4",
    "SR5",
    "SR6",
    "SR7",
    "SR8"
};

const size_t REG_COUNT = sizeof(regs)/sizeof(char*);
const size_t ARG_LIMIT = 10;

#endif