


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
    latchRegister(/* args */);
    ~latchRegister();

    unsigned int getIR()
    {
        return IR;
    }
    void setIR(unsigned int value)
    {
        IR = value;
        return;
    } 

    unsigned int getNPC()
    {
        return NPC;
    }
    void setNPC(unsigned int value)
    {
        NPC = value;
        return;
    }

    unsigned int getA()
    {
        return A;
    }
    void setA(unsigned int value)
    {
        A = value;
        return;
    }

    unsigned int getB()
    {
        return B;
    }
    void setB(unsigned int value)
    {
        B = value;
        return;
    }

    int getImm()
    {
        return Imm;
    }
    void setImm(int value)
    {
        Imm = value;
        return;
    }

    unsigned int getALUOutput()
    {
        return ALUOutput;
    }
    void setALUOutput(unsigned int value)
    {
        ALUOutput = value;
        return;
    }  

    unsigned int getCond()
    {
        return cond;
    }
    void setCond(unsigned int value)
    {
        cond = value;
        return;
    }

    unsigned int getLMD()
    {
        return LMD;
    }
    void setLMD(unsigned int value)
    {
        LMD = value;
        return;
    }                            
};

