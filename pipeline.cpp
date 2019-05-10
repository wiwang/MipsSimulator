#include <iostream>
#include <iomanip>
#include <string.h>

#include "pipeline.h"
#include "decoder.h"
#include "ALU.h"

using namespace std;

pipeline::pipeline(memory *m, MipsMode mode) 
{
    mem = m;
    instructions = m->getNumOfInstructs();
    this->mode = mode;

    stopFetching = false;
    holdDataHazard = false;
    OPsOfIF = 0;
    OPsOfID = 0;
    OPsOfEX = 0;
    OPsOfMEM = 0;
    OPsOfWB = 0;
    dataHazard = 0;
    cycles = 0;
    execNumber = 0;
    executedIR = NOP_INSTRUCTION;

    memset(&IF_ID, 0, sizeof(latchRegister));
    memset(&ID_EX, 0, sizeof(latchRegister));
    memset(&EX_MEM, 0, sizeof(latchRegister));
    memset(&MEM_WB, 0, sizeof(latchRegister));

    IF_ID.setIR(NOP_INSTRUCTION);
    ID_EX.setIR(NOP_INSTRUCTION);
    EX_MEM.setIR(NOP_INSTRUCTION);
    MEM_WB.setIR(NOP_INSTRUCTION);
}

unsigned int pipeline::getNumberOfInstructions()
{
    return instructions;
}

/* the mechanism to detect if there is a data hazard in pipeline*/
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

    OPsOfIF++; //useful operation increase by 1
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

    OPsOfID++; //useful operation increase by 1
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

    OPsOfEX++; //useful operation increase by 1
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
            cout<<"get data 0x"<<hex<<EX_MEM.getLMD()<<" in memory 0x"<<EX_MEM.getALUOutput()<<endl;
            OPsOfMEM++;
        }
        else if (decoder::isSaveInstruction(EX_MEM.getIR()))
        {
            mem->saveWordData(EX_MEM.getB(), EX_MEM.getALUOutput());
            OPsOfMEM++;
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
    else
    {
        cout<<"Get a unknown instruction in MEM stage"<<endl;
    }
}

bool pipeline::WBStage()
{
    executedIR = MEM_WB.getIR(); //record the last executed instruction

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

            //cout<<"Register value of Rd 0x"<<decoder::getRdField(MEM_WB.getIR())+1<<" in WB stage is 0x"<<hex<<R[decoder::getRdField(MEM_WB.getIR())+1]<<endl;
        }
        OPsOfWB++;

        //cout<<"Register value of Rd 0x"<<decoder::getRdField(MEM_WB.getIR())<<" in WB stage is 0x"<<hex<<R[decoder::getRdField(MEM_WB.getIR())]<<endl;
    }
    else if (decoder::isOriInstruction(MEM_WB.getIR()) || decoder::isAndiInstruction(MEM_WB.getIR()) 
            || decoder::isXoriInstruction(MEM_WB.getIR()) || decoder::isLuiInstruction(MEM_WB.getIR()))
    {
        R[decoder::getRtField(MEM_WB.getIR())] = MEM_WB.getALUOutput();
        OPsOfWB++;
    }
    else if (decoder::isLoadInstruction(MEM_WB.getIR()) || decoder::isSaveInstruction(MEM_WB.getIR()))
    {
        if (decoder::isLoadInstruction(MEM_WB.getIR()))
        {
            R[decoder::getRtField(MEM_WB.getIR())] = MEM_WB.getLMD();
            OPsOfWB++;//useful operation increase by 1
        }
    }
    else
    {
        cout<<"Get a unknown instruction in WB stage"<<endl;
    }
    
    //cout<<"Register value of Rt 0x" <<decoder::getRtField(MEM_WB.getIR())<<" in WB stage is 0x"<<hex<<R[decoder::getRtField(MEM_WB.getIR())]<<endl;

    return false;
}

bool pipeline::execute()
{
    bool ifEndOfProgram = false;

    ifEndOfProgram = WBStage();
    MEMStage();
    EXStage();
    IDStage();
    IFStage();

    cycles++; //cycle increase by 1 per timer tick

    if (mode==MIPS_InstructionMode && !(decoder::isENDInstruction(executedIR) || decoder::isNOPInstruction(executedIR)))
    {
        execNumber--;
        cout<<"instruction 0x"<<hex<<executedIR<<"is executed"<<endl;  
        dumpPipeline();
    }

    if (mode==MIPS_CycleMode)
    {
        execNumber--;        
        dumpPipeline();
    }
    
    return ifEndOfProgram;
}

void pipeline::displayResult()
{
    cout <<"===============The final result summary=================="<<endl;
    cout <<"The total cycles :"<<dec<<cycles<<endl;
    cout <<"The cycles of usefull work in IF :"<<dec<<OPsOfIF<<" ,utilization is "<<setiosflags(ios::fixed)<<setprecision(2)<<(double)(OPsOfIF*100)/cycles<<"%"<<endl;
    cout <<"The cycles of usefull work in ID :"<<dec<<OPsOfID<<" ,utilization is "<<setiosflags(ios::fixed)<<setprecision(2)<<(double)(OPsOfID*100)/cycles<<"%"<<endl;
    cout <<"The cycles of usefull work in EX :"<<dec<<OPsOfEX<<" ,utilization is "<<setiosflags(ios::fixed)<<setprecision(2)<<(double)(OPsOfEX*100)/cycles<<"%"<<endl;
    cout <<"The cycles of usefull work in MEM :"<<dec<<OPsOfMEM<<" ,utilization is "<<setiosflags(ios::fixed)<<setprecision(2)<<(double)(OPsOfMEM*100)/cycles<<"%"<<endl;
    cout <<"The cycles of usefull work in WB :"<<dec<<OPsOfWB<<" ,utilization is "<<setiosflags(ios::fixed)<<setprecision(2)<<(double)(OPsOfWB*100)/cycles<<"%"<<endl;
    cout <<"==========================end============================"<<endl;
}

void pipeline::setExecNumber(unsigned int n)
{
    execNumber = n;
}

unsigned int pipeline::getExecNumber()
{
    return execNumber;
}

void pipeline::dumpPipeline()
{
    cout<<"===============dump the status of CPU================="<<endl;    
    cout<<"Clock cycle is: "<<dec<<cycles<<endl;
    cout<<"R[0]:0x"<<hex<<R[0]<<" R[1]:0x"<<R[1]<<" R[2]:0x"<<R[2]<<" R[3]:0x"<<R[3]<<" R[4]:0x"<<R[4]<<" R[5]:0x"<<R[5]<<" R[6]:0x"<<R[6]<<" R[7]:0x"<<R[7]<<endl;
    cout<<"R[8]:0x"<<hex<<R[8]<<" R[9]:0x"<<R[9]<<" R[10]:0x"<<R[10]<<" R[11]:0x"<<R[11]<<" R[12]:0x"<<R[12]<<" R[13]:0x"<<R[13]<<" R[14]:0x"<<R[14]<<" R[15]:0x"<<R[15]<<endl;
    cout<<"R[16]:0x"<<hex<<R[16]<<" R[17]:0x"<<R[17]<<" R[18]:0x"<<R[18]<<" R[19]:0x"<<R[19]<<" R[20]:0x"<<R[20]<<" R[21]:0x"<<R[21]<<" R[22]:0x"<<R[22]<<" R[23]:0x"<<R[23]<<endl;
    cout<<"R[24]:0x"<<hex<<R[24]<<" R[25]:0x"<<R[25]<<" R[26]:0x"<<R[26]<<" R[27]:0x"<<R[27]<<" R[28]:0x"<<R[28]<<" R[29]:0x"<<R[29]<<" R[30]:0x"<<R[30]<<" R[31]:0x"<<R[31]<<endl;

    cout<<"IR in IF_ID latch is 0x"<<hex<<IF_ID.getIR()<<endl;
    cout<<"NPC in IF_ID latch is 0x"<<hex<<IF_ID.getNPC()<<endl;
    cout<<"PC in IF_ID latch is 0x"<<hex<<PC<<endl;       

    cout<<"IR in ID_EX latch is 0x"<<hex<<ID_EX.getIR()<<endl;
    cout<<"A in ID_EX latch is 0x"<<hex<<ID_EX.getA()<<endl;
    cout<<"B in ID_EX latch is 0x"<<hex<<ID_EX.getB()<<endl;
    cout<<"Immediate in ID_EX latch is 0x"<<hex<<ID_EX.getImm()<<endl;

    cout<<"IR in EX_MEM latch is 0x"<<hex<<EX_MEM.getIR()<<endl;
    cout<<"ALUOutput in EX_MEM latch is 0x"<<hex<<EX_MEM.getALUOutput()<<endl;
    cout<<"Condition in EX_MEM latch is 0x"<<hex<<EX_MEM.getCond()<<endl;

    cout<<"IR in MEM_WB latch is 0x"<<hex<<MEM_WB.getIR()<<endl;
    cout<<"ALUOutput in MEM_WB latch is 0x"<<hex<<MEM_WB.getALUOutput()<<endl;
    cout<<"LMD in MEM_WB latch is 0x"<<hex<<MEM_WB.getLMD()<<endl;   

    cout<<"==============The content of data memory================"<<endl;
    unsigned int word = 0;
    for (int i = 0; i < 2048/4; i+=4)
    {
        word = mem->loadWordData(i);
        if (word)
        {
            cout<<"the value of word at address 0x"<<hex<<i<<" is 0x"<<word<<endl;
        }
    }
    cout<<"========================end=============================="<<endl;
}

unsigned int pipeline::PC = 0;
unsigned int pipeline::R[32] = {0};
