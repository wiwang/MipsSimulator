#include "latchRegister.h"


void latchRegister::clear()
{
    IR = 0;
    NPC = 0;
    A = 0;
    B = 0;
    Imm = 0;
    ALUOutput = 0;
    cond = 0;
    LMD = 0;
}

unsigned int latchRegister::getIR()
{
    return IR;
}
void latchRegister::setIR(unsigned int value)
{
    IR = value;
    return;
} 

unsigned int latchRegister::getNPC()
{
    return NPC;
}
void latchRegister::setNPC(unsigned int value)
{
    NPC = value;
    return;
}

unsigned int latchRegister::getA()
{
    return A;
}
void latchRegister::setA(unsigned int value)
{
    A = value;
    return;
}

unsigned int latchRegister::getB()
{
    return B;
}
void latchRegister::setB(unsigned int value)
{
    B = value;
    return;
}

int latchRegister::getImm()
{
    return Imm;
}
void latchRegister::setImm(int value)
{
    Imm = value;
    return;
}

unsigned int latchRegister::getALUOutput()
{
    return ALUOutput;
}
void latchRegister::setALUOutput(unsigned int value)
{
    ALUOutput = value;
    return;
}  

unsigned int latchRegister::getCond()
{
    return cond;
}
void latchRegister::setCond(unsigned int value)
{
    cond = value;
    return;
}

unsigned int latchRegister::getLMD()
{
    return LMD;
}
void latchRegister::setLMD(unsigned int value)
{
    LMD = value;
    return;
}

