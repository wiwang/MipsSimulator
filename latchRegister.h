#ifndef _MIPS_SIMULATOR_LATCH_REGISTER_H_
#define _MIPS_SIMULATOR_LATCH_REGISTER_H_

class latchRegister
{
private:
    unsigned int IR;
    unsigned int NPC;
    unsigned int A;
    unsigned int B;
    int Imm;
    unsigned int ALUOutput;
    unsigned int cond;
    unsigned int LMD;

public:
    void clear();
    unsigned int getIR();
    void setIR(unsigned int value);
    unsigned int getNPC();
    void setNPC(unsigned int value);
    unsigned int getA();
    void setA(unsigned int value);
    unsigned int getB();
    void setB(unsigned int value);
    int getImm();
    void setImm(int value);
    unsigned int getALUOutput();
    void setALUOutput(unsigned int value);
    unsigned int getCond();
    void setCond(unsigned int value);
    unsigned int getLMD();
    void setLMD(unsigned int value);                    
};

#endif