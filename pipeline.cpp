#include <iostream>

#include "latchRegister.cpp"
#include "memory.cpp"
#include "decoder.cpp"
#include "main.cpp"

using namespace std;

class pipeline
{
private:
    latchRegister IF_ID;
    latchRegister ID_EX;
    latchRegister EX_MEM;
    latchRegister MEM_WB;
    
    memory *mem;
public:
    static unsigned int R[32];
    static unsigned int PC;

    pipeline(/* args */);
    ~pipeline();

    void IFStage()
    {
        IF_ID.setIR(mem->loadInstruction(PC));
        if (decoder::isBranchInstruction(EX_MEM.getIR()) && EX_MEM.getCond())
        {
            IF_ID.setNPC(EX_MEM.getALUOutput());
            PC = EX_MEM.getALUOutput();
        }
        else
        {
            IF_ID.setNPC(PC+4);
            PC+=4;
        }
    } 

    latchRegister* IDStage()
    {
        ID_EX.setA(R[decoder::getRsField(IF_ID.getIR())]);
        ID_EX.setB(R[decoder::getRtField(IF_ID.getIR())]);
        ID_EX.setNPC(IF_ID.getNPC);
        ID_EX.setIR(IF_ID.getIR);
        ID_EX.setImm((signed int)(decoder::getOffsetField(IF_ID.getIR())));
    }

    latchRegister* EXStage()
    {

    }

    latchRegister* MEMStage()
    {

    }

    latchRegister* WBStage()
    {

    }

    void execute(MipsMode mode)
    {

    }
};

unsigned int pipeline::PC = 0;

pipeline::pipeline(/* args */)
{
}

pipeline::~pipeline()
{
}




