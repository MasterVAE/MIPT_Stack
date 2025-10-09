#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "disassembler_life.h"

int DISInit(Disassembler* dis)
{
    if(dis == NULL) return 1;
    dis->buffer_size = 0;
    dis->offset = 0;
    return 0;
}

void DISDestroy(Disassembler* dis)
{
    free(dis->buffer);
    memset(dis, 0, sizeof(Disassembler));
}