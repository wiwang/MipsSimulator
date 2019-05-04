#ifndef _MIPS_SIMULATOR_MEMORY_H_
#define _MIPS_SIMULATOR_MEMORY_H_

class memory
{
private:
    char *IMem;
    char *DMem;
    unsigned int numOfInstructs;
public:
    memory();
    ~memory();
    unsigned int loadWordData(unsigned int index);
    void saveWordData(unsigned int data, unsigned int index);
    unsigned int loadInstruction(unsigned int PC);
    void saveInstruction(unsigned int instruction); 
    bool saveBinaryProgram(char *fileName); 
    unsigned int getNumOfInstructs();
    void setNumOfInstructs(unsigned int num);
};

#endif