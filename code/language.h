#ifndef LANGUAGE_H_
#define LANGUAGE_H_

typedef char    COMMAND_TYPE;
typedef int     VALUE_TYPE;

typedef int stack_type;
const size_t RAM_COUNT = 400;
const size_t VRAM_BY_LINE = 20;

const char* const regs[] = 
{
    "SR1",
    "SR2",
    "SR3",
    "SR4",
    "SR5",
    "SR6",
    "SRX",
    "SRY"
};

const size_t REG_COUNT = sizeof(regs)/sizeof(char*);
const size_t ARG_LIMIT = 10;

#endif