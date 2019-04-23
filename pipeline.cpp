#include <iostream>

#include "latchRegister.cpp"
#include "memory.cpp"
#include "decoder.cpp"
#include "ALU.cpp"
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
        if (decoder::isRInstruction(ID_EX.getIR()))
        {
            EX_MEM.saveInstruction(ID_EX.getIR());
            EX_MEM.setALUOutput(ALU::execute(ID_EX.getA(), ID_EX.getB(), ID_EX.getIR));
        }
        else if (decoder::isJInstruction(ID_EX.getIR()))
        {
            EX_MEM.saveInstruction(ID_EX.getIR());
            EX_MEM.setALUOutput(ID_EX.getA() + ID_EX.getImm());
        }
        else if (decoder::isBranchInstruction(ID_EX.getIR()))
        {
            EX_MEM.setALUOutput(ID_EX.getNPC() + (ID_EX.getImm()<<2);
            EX_MEM.setCond((ID_EX.getA()==ID_EX.getB)? 1 : 0);
        }
        else
        {
            /* code */
        }
        
        
    }

    latchRegister* MEMStage()
    {
        if (decoder::isRInstruction(EX_MEM.getIR()))
        {
            MEM_WB.saveInstruction(EX_MEM.getIR());
            MEM_WB.setALUOutput(EX_MEM.getALUOutput());
        }
        else if (decoder::isJInstruction(EX_MEM.getIR()))
        {
            MEM_WB.saveInstruction(EX_MEM.getIR());
            if (decoder::isLoadInstruction())
            {
                MEM_WB.setLMD(mem->loadWordData(EX_MEM.getALUOutput()));
            }
            else if (decoder::isSaveInstruction())
            {
                mem->saveWordData(EX_MEM.getB(), EX_MEM.getALUOutput());
            }
            else
            {
             /* code */
            }
            
        }
    }

    latchRegister* WBStage()
    {
        if (decoder::isRInstruction(MEM_WB.getIR()))
        {
            R[decoder::getRdField(MEM_WB.getIR())] = MEM_WB.getALUOutput();
        }
        else if (decoder::isJInstruction(MEM_WB.getIR()))
        {
            if (decoder::isLoadInstruction())
            {
                R[decoder::getRtField(MEM_WB.getIR())] = MEM_WB.getLMD();
            }
        }
    }

    void execute(MipsMode mode)
    {
        WBStage();
        MEMStage();
        EXStage();
        IDStage();
        IFStage();
    }
};

unsigned int pipeline::PC = 0;

pipeline::pipeline(/* args */)
{
}

pipeline::~pipeline()
{
}




