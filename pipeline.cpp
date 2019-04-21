#include <iostream>

#include "latchRegister.cpp"
#include "memory.cpp"
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
    pipeline(/* args */);
    ~pipeline();

    latchRegister* IFStage()
    {

    } 

    latchRegister* IDStage()
    {


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

pipeline::pipeline(/* args */)
{
}

pipeline::~pipeline()
{
}


