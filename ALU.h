#ifndef _MIPS_SIMULATOR_ALU_H_
#define _MIPS_SIMULATOR_ALU_H_

class ALU
{
public:
    static unsigned int execute(unsigned int a, unsigned int b, unsigned int instruction);
    static unsigned long long int mul(unsigned int a, unsigned int b);
};

#endif
