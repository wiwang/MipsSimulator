#include "decoder.h"


bool decoder::isRInstruction(unsigned int instruction)
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
bool decoder::isIInstruction(unsigned int instruction) 
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
bool decoder::isBranchInstruction(unsigned int instruction) 
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

bool decoder::isLoadInstruction(unsigned int instruction) 
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
bool decoder::isSaveInstruction(unsigned int instruction) 
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

bool decoder::isOriInstruction(unsigned int instruction)
{
    if ((instruction&OP_MASK) == ORI_INSTRUCTION)
    {
        return true;
    }
    else
    {
        return false;
    }  
    
}

bool decoder::isAndiInstruction(unsigned int instruction)
{
    if ((instruction&OP_MASK) == ANDI_INSTRUCTION)
    {
        return true;
    }
    else
    {
        return false;
    }  
}

bool decoder::isXoriInstruction(unsigned int instruction)
{
    if ((instruction&OP_MASK) == XORI_INSTRUCTION)
    {
        return true;
    }
    else
    {
        return false;
    }  
}        

bool decoder::isLuiInstruction(unsigned int instruction)
{
    if ((instruction&OP_MASK) == LUI_INSTRUCTION)
    {
        return true;
    }
    else
    {
        return false;
    }  
}

bool decoder::isNOPInstruction(unsigned int instruction)
{
    if (instruction == NOP_INSTRUCTION)
    {
        return true;
    }
    else
    {
        return false;
    }  
}

bool decoder::isENDInstruction(unsigned int instruction)
{
    if (instruction == END_INSTRUCTION)
    {
        return true;
    }
    else
    {
        return false;
    }  
}

bool decoder::isMulInstruction(unsigned int instruction)
{
    if (0x00000018 == (instruction&0x0000003f)
    {
        return true;
    }
    return false;
}

unsigned int decoder::getRsField(unsigned int instruction) { return ((instruction&RS_MASK)>>21);}
unsigned int decoder::getRtField(unsigned int instruction) { return ((instruction&RT_MASK)>>16);}
unsigned int decoder::getRdField(unsigned int instruction) { return ((instruction&RD_MASK)>>11);}
unsigned int decoder::getOffsetField(unsigned int instruction) { return (instruction&IMM_MASK);}
unsigned int decoder::getFunctField(unsigned int instruction) { return (instruction & FUNCT_MASK);}
