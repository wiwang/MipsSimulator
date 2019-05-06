
#include "ALU.h"
#include "decoder.h"


unsigned int ALU::execute(unsigned int a, unsigned int b, unsigned int instruction)
{
    unsigned int immediate = (unsigned int)decoder::getOffsetField(instruction);

    if (decoder::isRInstruction(instruction))
    {
        switch (instruction & 0x0000003f)
        {
        case 0x00000018://multiply
            return (a*b);
            break;

        case 0x00000020://add
            return (a + b);
            break;

        case 0x00000022://sub
            return (a - b);
            break;

        case 0x00000024://and
            return (a & b);
            break;

        case 0x00000025://or
            return (a | b);
            break;

        case 0x00000026://slt
            return (a - b);
            break;                                    

        default:
            break;
        }
    }

    if (decoder::isOriInstruction(instruction))
    {
        return (a | immediate);
    }

    if (decoder::isAndiInstruction(instruction))
    {
        return (a & immediate);
    }

    if (decoder::isXoriInstruction(instruction))
    {
        return (a xor immediate);
    }
}

unsigned long int ALU::mul(unsigned int a, unsigned int b)
{
    return((unsigned long int)a * (unsigned long int)b);
}
