#ifndef _MIPS_SIMULATOR_PIPELINE_H_
#define _MIPS_SIMULATOR_PIPELINE_H_

#include "latchRegister.h"
#include "memory.h"

enum MipsMode
{
    MIPS_InstructionMode = 0,
    MIPS_CycleMode,
    MIPS_MaxMode
};

class pipeline
{
private:
    latchRegister IF_ID;
    latchRegister ID_EX;
    latchRegister EX_MEM;
    latchRegister MEM_WB;

    bool stopFetching;
    bool holdDataHazard;

    unsigned int OPsOfIF;
    unsigned int OPsOfID;
    unsigned int OPsOfEX;
    unsigned int OPsOfMEM;
    unsigned int OPsOfWB;
    unsigned int cycles;

    unsigned int dataHazard;
    unsigned int instructions;

    enum MipsMode mode;
    unsigned int execNumber;
    unsigned int executedIR;

    memory *mem;
public:
    static unsigned int R[32];
    static unsigned int PC;

    pipeline(memory *m, MipsMode mode);
    void IFStage();
    void IDStage();
    void EXStage();
    void MEMStage();
    bool WBStage();
    bool execute();

    unsigned int getDataHazard();
    unsigned int getNumberOfInstructions();

    void displayResult();
    void dumpPipeline();
    void setExecNumber(unsigned int n);
    unsigned int getExecNumber();
};

#endif
