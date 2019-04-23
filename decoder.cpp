#include "decoder.h"

class decoder
{
private:

public:
    static bool isRInstruction(unsigned int instruction)
    {
        if ((instruction&OP_MASK) == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    static bool isJInstruction(unsigned int instruction) 
    {
         if ((instruction&OP_MASK) != 0 && (instruction&OP_MASK) != J_INSTRUCTION)
        {
            return true;
        }
        else
        {
            return false;
        }       
    }
    static bool isBranchInstruction(unsigned int instruction) 
    {
        if ((instruction&OP_MASK) == BRANCH_INSTRUCTION)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    static bool isLoadInstruction(unsigned int instruction) 
    {
        if ((instruction&OP_MASK) == LW_INSTRUCTION)
        {
            return true;
        }
        else
        {
            return false;
        }       
    }
    static bool isSaveInstruction(unsigned int instruction) 
    {
        if ((instruction&OP_MASK) == SW_INSTRUCTION)
        {
            return true;
        }
        else
        {
            return false;
        }        
    }

    static unsigned int getRsField(unsigned int instruction) { return (instruction & RS_MASK);}
    static unsigned int getRtField(unsigned int instruction) { return (instruction & RT_MASK);}
    static unsigned int getRdField(unsigned int instruction) { return (instruction & RD_MASK);}
    static unsigned int getOffsetField(unsigned int instruction) { return (instruction & IMM_MASK);}
    static unsigned int getFunctField(unsigned int instruction) { return (instruction & FUNCT_MASK);}
};