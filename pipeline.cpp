#include <iostream>
#include <string.h>

#include "pipeline.h"
#include "decoder.h"
#include "ALU.h"

using namespace std;

pipeline::pipeline(memory *m) 
{
    mem = m;
    instructions = m->getNumOfInstructs();

    stopFetching = false;
    holdDataHazard = false;
    OPsOfIF = 0;
    OPsOfID = 0;
    OPsOfEX = 0;
    OPsOfMEM = 0;
    OPsOfWB = 0;
    dataHazard = 0;
    cycles = 0;

    memset(&IF_ID, 0, sizeof(latchRegister));
    memset(&ID_EX, 0, sizeof(latchRegister));
    memset(&EX_MEM, 0, sizeof(latchRegister));
    memset(&MEM_WB, 0, sizeof(latchRegister));
}

unsigned int pipeline::getNumberOfInstructions()
{
    return instructions;
}

unsigned int pipeline::getDataHazard()
{
    if (decoder::isLoadInstruction(EX_MEM.getIR()) || decoder::isLuiInstruction(EX_MEM.getIR())
    || decoder::isAndiInstruction(EX_MEM.getIR()) || decoder::isOriInstruction(EX_MEM.getIR()))
    {
        if (decoder::getRsField(IF_ID.getIR())==decoder::getRtField(EX_MEM.getIR()))
        {
            return 2;
        }

        if (decoder::isSaveInstruction(IF_ID.getIR()) && decoder::getRtField(IF_ID.getIR())==decoder::getRtField(EX_MEM.getIR()))
        {
            return 2;
        }

        if (decoder::isRInstruction(IF_ID.getIR()) && (IF_ID.getIR()& 0x0000003f)!=0 && decoder::getRtField(IF_ID.getIR())==decoder::getRtField(EX_MEM.getIR()))
        {
            return 2;
        }
                      
    }

    if (decoder::isRInstruction(EX_MEM.getIR()) && (EX_MEM.getIR()& 0x0000003f)!=0)
    {
        if (decoder::getRsField(IF_ID.getIR())==decoder::getRdField(EX_MEM.getIR())
        || decoder::getRtField(IF_ID.getIR())==decoder::getRdField(EX_MEM.getIR()))
        {
            return 2;
        }
    }

    if (decoder::isLoadInstruction(MEM_WB.getIR()) || decoder::isLuiInstruction(MEM_WB.getIR())
    || decoder::isAndiInstruction(MEM_WB.getIR()) || decoder::isOriInstruction(MEM_WB.getIR()))
    {
        if (decoder::getRsField(IF_ID.getIR())==decoder::getRtField(MEM_WB.getIR()))
        {
            return 1;
        }
        if (decoder::isSaveInstruction(IF_ID.getIR()) && decoder::getRtField(IF_ID.getIR())==decoder::getRtField(MEM_WB.getIR()))
        {
            return 1;
        }
        if (decoder::isRInstruction(IF_ID.getIR()) && (IF_ID.getIR()& 0x0000003f)!=0 && decoder::getRtField(IF_ID.getIR())==decoder::getRtField(MEM_WB.getIR()))
        {
            return 1;
        }              
    }

    if (decoder::isRInstruction(MEM_WB.getIR()) && (MEM_WB.getIR()& 0x0000003f)!=0)
    {
        if (decoder::getRsField(IF_ID.getIR())==decoder::getRdField(MEM_WB.getIR())
        || decoder::getRtField(IF_ID.getIR())==decoder::getRdField(MEM_WB.getIR()))
        {
            return 1;
        }
    }

    return 0;
}

void pipeline::IFStage()
{
    if (stopFetching)
    {
        IF_ID.clear(); //flush IF-ID register
        IF_ID.setIR(NOP_INSTRUCTION);  //Insert NOPs
        cout<<"Insert a NOP in IF stage"<<endl;
        return;
    }

    if (holdDataHazard)
    {
        /* Don't fetch new instruction to keep hazardous instruction in ID stage*/
        cout<<"stop fetching instruction in IF stage"<<endl;
        return;
    }

     /* reach the end of program*/
    if (PC >= instructions*4)
    {
        IF_ID.clear(); //flush IF-ID register
        IF_ID.setIR(END_INSTRUCTION);  //Insert NOPs
        cout<<"Insert a END instruction in IF stage"<<endl;
        return;
    }    

    IF_ID.setIR(mem->loadInstruction(PC));

    if (decoder::isBranchInstruction(EX_MEM.getIR()) && EX_MEM.getCond()) 
    {
        PC = EX_MEM.getALUOutput();        
        IF_ID.setIR(mem->loadInstruction(PC)); //jump to the new PC
        IF_ID.setNPC(PC+4);
        PC+=4; 
    }
    else
    {
        IF_ID.setNPC(PC+4);
        PC+=4;
    }

    OPsOfIF++;

    cout<<"IR in IF_ID latch is 0x"<<hex<<IF_ID.getIR()<<endl;
    cout<<"NPC in IF_ID latch is 0x"<<hex<<IF_ID.getNPC()<<endl;
    cout<<"PC in IF_ID latch is 0x"<<hex<<PC<<endl;   
} 

void pipeline::IDStage()
{
    /* branch hazards*/
    if (decoder::isBranchInstruction(IF_ID.getIR()))
    {
        stopFetching = true;
    }

    /* data hazards*/
    dataHazard = getDataHazard();
    if (dataHazard > 0)
    {
        cout<<"Insert "<< dataHazard <<" NOPs in ID stage"<<endl;
        ID_EX.clear();
        ID_EX.setIR(NOP_INSTRUCTION);
        holdDataHazard = true;
        dataHazard--;
        return;
    }
    
    /* handle NOP instruction*/
    if (decoder::isNOPInstruction(IF_ID.getIR()))
    {
        cout<<"Get a NOP in ID stage"<<endl;
        ID_EX.clear();
        ID_EX.setIR(IF_ID.getIR());   
        return;      
    }

    /* handle end instruction*/
    if (decoder::isENDInstruction(IF_ID.getIR()))
    {
        cout<<"Get a end instruction in ID stage"<<endl;
        ID_EX.clear();
        ID_EX.setIR(IF_ID.getIR());   
        return;      
    }    

    ID_EX.setA(R[decoder::getRsField(IF_ID.getIR())]);
    ID_EX.setB(R[decoder::getRtField(IF_ID.getIR())]);
    ID_EX.setNPC(IF_ID.getNPC());
    ID_EX.setIR(IF_ID.getIR());
    ID_EX.setImm((signed int)(decoder::getOffsetField(IF_ID.getIR())));

    holdDataHazard = false;

    OPsOfID++;

    cout<<"IR in ID_EX latch is "<<hex<<ID_EX.getIR()<<endl;
    cout<<"A in ID_EX latch is "<<hex<<ID_EX.getA()<<endl;
    cout<<"B in ID_EX latch is "<<hex<<ID_EX.getB()<<endl;
    cout<<"Immediate in ID_EX latch is "<<hex<<ID_EX.getImm()<<endl;
}

void pipeline::EXStage()
{
    /* handle NOP instruction*/
    if (decoder::isNOPInstruction(ID_EX.getIR()))
    {
        cout<<"Get a NOP in EX stage"<<endl;
        EX_MEM.clear();
        EX_MEM.setIR(ID_EX.getIR());   
        return;      
    }

    /* handle end instruction*/
    if (decoder::isENDInstruction(ID_EX.getIR()))
    {
        cout<<"Get a end instruction in EX stage"<<endl;
        EX_MEM.clear();
        EX_MEM.setIR(ID_EX.getIR());   
        return;      
    }    

    if (decoder::isRInstruction(ID_EX.getIR()) || decoder::isOriInstruction(ID_EX.getIR()) 
    || decoder::isAndiInstruction(ID_EX.getIR()) || decoder::isXoriInstruction(ID_EX.getIR()))
    {
        EX_MEM.setIR(ID_EX.getIR());
        EX_MEM.setALUOutput(ALU::execute(ID_EX.getA(), ID_EX.getB(), ID_EX.getIR()));

        if (decoder::isMulInstruction(ID_EX.getIR()))
        {
            EX_MEM.setMulResult(ALU::mul(ID_EX.getA(), ID_EX.getB()));
        }
        
    }
    else if (decoder::isLoadInstruction(ID_EX.getIR()) || decoder::isSaveInstruction(ID_EX.getIR()))
    {
        EX_MEM.setIR(ID_EX.getIR());
        EX_MEM.setALUOutput(ID_EX.getA() + ID_EX.getImm());
        EX_MEM.setB(ID_EX.getB());
    }
    else if (decoder::isBranchInstruction(ID_EX.getIR()))
    {
        EX_MEM.setIR(ID_EX.getIR());
        EX_MEM.setALUOutput(ID_EX.getNPC() + (ID_EX.getImm()<<2));
        EX_MEM.setCond((ID_EX.getA()==ID_EX.getB())? 1 : 0);
        stopFetching = false; //resume fetching instruction
    }
    else if (decoder::isLuiInstruction(ID_EX.getIR()))
    {
        EX_MEM.setIR(ID_EX.getIR());
        EX_MEM.setALUOutput(ID_EX.getImm() << 16); 
    }
    else
    {
        cout<<"EXStage enter a unsupported case"<<endl;
    }

    OPsOfEX++;

    cout<<"IR in EX_MEM latch is "<<hex<<EX_MEM.getIR()<<endl;
    cout<<"ALUOutput in EX_MEM latch is "<<hex<<EX_MEM.getALUOutput()<<endl;
    cout<<"Condition in EX_MEM latch is "<<hex<<EX_MEM.getCond()<<endl;   
}

void pipeline::MEMStage()
{
    /* handle NOP instruction*/
    if (decoder::isNOPInstruction(EX_MEM.getIR()))
    {
        cout<<"Get a NOP in MEM stage"<<endl;
        MEM_WB.clear();
        MEM_WB.setIR(EX_MEM.getIR());   
        return;      
    }

    /* handle end instruction*/
    if (decoder::isENDInstruction(EX_MEM.getIR()))
    {
        cout<<"Get a end instruction in MEM stage"<<endl;
        MEM_WB.clear();
        MEM_WB.setIR(EX_MEM.getIR());   
        return;      
    }    

    if (decoder::isRInstruction(EX_MEM.getIR()) || decoder::isOriInstruction(EX_MEM.getIR()) 
    || decoder::isAndiInstruction(EX_MEM.getIR()) || decoder::isXoriInstruction(EX_MEM.getIR()))
    {
        MEM_WB.setIR(EX_MEM.getIR());
        MEM_WB.setALUOutput(EX_MEM.getALUOutput());
        MEM_WB.setMulResult(EX_MEM.getMulResult());
    }
    else if (decoder::isLoadInstruction(EX_MEM.getIR()) || decoder::isSaveInstruction(EX_MEM.getIR()))
    {
        MEM_WB.setIR(EX_MEM.getIR());
        if (decoder::isLoadInstruction(EX_MEM.getIR()))
        {
            MEM_WB.setLMD(mem->loadWordData(EX_MEM.getALUOutput()));
        }
        else if (decoder::isSaveInstruction(EX_MEM.getIR()))
        {
            mem->saveWordData(EX_MEM.getB(), EX_MEM.getALUOutput());
            cout<<"save data 0x"<<hex<<EX_MEM.getB()<<" in memory 0x"<<EX_MEM.getALUOutput()<<endl;
        }
        else
        {
            cout<<"MEMStage enter a unsupported case"<<endl;
        }
    }
    else if (decoder::isLuiInstruction(EX_MEM.getIR()))
    {
        MEM_WB.setIR(EX_MEM.getIR());
        MEM_WB.setALUOutput(EX_MEM.getALUOutput());
    }

    OPsOfMEM++;

    cout<<"IR in MEM_WB latch is "<<hex<<MEM_WB.getIR()<<endl;
    cout<<"ALUOutput in MEM_WB latch is "<<hex<<MEM_WB.getALUOutput()<<endl;
    cout<<"LMD in MEM_WB latch is "<<hex<<MEM_WB.getLMD()<<endl;      
}

bool pipeline::WBStage()
{
    /* reach the end of program, inform clock to stop */
    if (decoder::isENDInstruction(MEM_WB.getIR()))
    {
        cout<<"Get a END instruction in WB stage"<<endl;
        return true;
    }
    
    /* handle NOP instruction*/
    if (decoder::isNOPInstruction(MEM_WB.getIR()))
    {
        cout<<"Get a NOP in WB stage"<<endl;
        return false;      
    }

    if (decoder::isRInstruction(MEM_WB.getIR()))
    {
        R[decoder::getRdField(MEM_WB.getIR())] = MEM_WB.getALUOutput();
        if (decoder::isMulInstruction(MEM_WB.getIR()))
        {
            R[decoder::getRdField(MEM_WB.getIR())] = (unsigned int)(MEM_WB.getMulResult() & 0x00000000ffffffff);
            R[decoder::getRdField(MEM_WB.getIR())+1] = (unsigned int)((MEM_WB.getMulResult() & 0xffffffff00000000)>>32);

            cout<<"Register value of Rd 0x"<<decoder::getRdField(MEM_WB.getIR())+1<<" in WB stage is "<<hex<<R[decoder::getRdField(MEM_WB.getIR())+1]<<endl;
        }

        cout<<"Register value of Rd 0x"<<decoder::getRdField(MEM_WB.getIR())<<" in WB stage is "<<hex<<R[decoder::getRdField(MEM_WB.getIR())]<<endl;
    }
    else if (decoder::isOriInstruction(MEM_WB.getIR()) || decoder::isAndiInstruction(MEM_WB.getIR()) 
            || decoder::isXoriInstruction(MEM_WB.getIR()) || decoder::isLuiInstruction(MEM_WB.getIR()))
    {
        R[decoder::getRtField(MEM_WB.getIR())] = MEM_WB.getALUOutput();
    }
    else if (decoder::isLoadInstruction(MEM_WB.getIR()) || decoder::isSaveInstruction(MEM_WB.getIR()))
    {
        if (decoder::isLoadInstruction(MEM_WB.getIR()))
        {
            R[decoder::getRtField(MEM_WB.getIR())] = MEM_WB.getLMD();
        }
    }

    OPsOfWB++;

    cout<<"Register value of Rt 0x" <<decoder::getRtField(MEM_WB.getIR())<<" in WB stage is "<<hex<<R[decoder::getRtField(MEM_WB.getIR())]<<endl;

    return false;
}

bool pipeline::execute(MipsMode mode)
{
    bool ifEndOfProgram = false;

    ifEndOfProgram = WBStage();
    MEMStage();
    EXStage();
    IDStage();
    IFStage();

    cycles++;

    return ifEndOfProgram;
}

void pipeline::displayResult()
{
    cout <<"======================================================="<<endl;
    cout <<"The total cycles :"<<dec<<cycles<<endl;
    cout <<"The cycles of usefull work in IF :"<<dec<<OPsOfIF<<endl;
    cout <<"The cycles of usefull work in ID :"<<dec<<OPsOfID<<endl;
    cout <<"The cycles of usefull work in EX :"<<dec<<OPsOfEX<<endl;
    cout <<"The cycles of usefull work in MEM :"<<dec<<OPsOfMEM<<endl;
    cout <<"The cycles of usefull work in WB :"<<dec<<OPsOfWB<<endl;
    cout <<"======================================================="<<endl;
}

unsigned int pipeline::PC = 0;
unsigned int pipeline::R[32] = {0};
