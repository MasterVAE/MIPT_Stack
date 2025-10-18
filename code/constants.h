#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <stddef.h>

typedef char command_type;
typedef int value_type;
typedef int stack_type;

const size_t RAM_SIZE = 80 * 45;
const size_t VRAM_BY_LINE = 80;

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

const size_t REG_SIZE = sizeof(regs) / sizeof(regs[0]);

#endif // CONSTANTS_H_