#ifndef DISASSEMBLER_FUNC_H
#define DISASSEMBLER_FUNC_H

#include "disassembler_manager.h"

DisErr_t DisDef (Disassembler* dis, size_t my_ind);
DisErr_t DisPush(Disassembler* dis, size_t my_ind);
DisErr_t DisPopr(Disassembler* dis, size_t my_ind);
DisErr_t DisJump(Disassembler* dis, size_t my_ind);

#endif // DISASSEMBLER_FUNC_H