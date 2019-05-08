#include "memory.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <stdlib.h>

using namespace std;

memory::memory() 
{
    if (NULL != (IMem=new char[2048]) && NULL != (DMem=new char[2048]))
    {
        memset(IMem, 0, 2048);
        memset(DMem, 0, 2048);
        numOfInstructs = 0;
        cout << "memory is allocated succesufully" << endl;
    }
}
memory::~memory() {delete [] IMem; delete [] DMem;}

unsigned int memory::loadWordData(unsigned int index)
{
    return ((0xff000000&(DMem[index]<<24)) | (0x00ff0000&(DMem[index+1]<<16)) |
        (0x0000ff00&(DMem[index+2]<<8)) | (0x000000ff&DMem[index+3]));
}

void memory::saveWordData(unsigned int data, unsigned int index)
{
    DMem[index] = data>>24;
    DMem[index+1] = data>>16;
    DMem[index+2] = data>>8;
    DMem[index+3] = data;
    return;
}

unsigned int memory::loadInstruction(unsigned int PC)
{
    return ((0xff000000&(IMem[PC+3]<<24)) | (0x00ff0000&(IMem[PC+2]<<16)) |
        (0x0000ff00&(IMem[PC+1]<<8)) | (0x000000ff&IMem[PC]));
}

void memory::saveInstruction(unsigned int instruction)
{
    return;
}

bool memory::saveBinaryProgram(char *fileName)
{
    ifstream infile;
    unsigned long int ins;
    char str[64];
    char *i = IMem;
    unsigned int *instruction = (unsigned int *)i;

    infile.open(fileName);
    while (infile.getline(str, sizeof(str)))
    {
        ins = strtoul(str, NULL, 2);
        (*instruction) = (unsigned int)ins;
        i+=4;
        instruction = (unsigned int *)i;

        numOfInstructs++;
    }

    cout<<"Load "<<numOfInstructs<<" instructions"<<endl;
    
    return true;
}

unsigned int memory::getNumOfInstructs()
{
    return numOfInstructs;
}

void memory::setNumOfInstructs(unsigned int num)
{
    numOfInstructs = num;
}
