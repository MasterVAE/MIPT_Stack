#ifndef DISASSEMBLER_FUNC_H
#define DISASSEMBLER_FUNC_H

#include "disassembler_manager.h"

dis_err dis_def(Disassembler* dis, size_t my_ind, FILE* output_file);
dis_err dis_push(Disassembler* dis, size_t my_ind, FILE* output_file);
dis_err dis_popr(Disassembler* dis, size_t my_ind, FILE* output_file);
dis_err dis_jump(Disassembler* dis, size_t my_ind, FILE* output_file);

#endif // DISASSEMBLER_FUNC_H