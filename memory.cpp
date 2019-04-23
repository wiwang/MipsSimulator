#include <iostream>

using namespace std;

class memory
{
private:
    char *IMem;
    char *DMem;
public:
    memory() 
    {
        if (NULL != (IMem=new char[2048]) && NULL != (DMem=new char[2048]))
        {
            cout << "memory is allocated succesufully" << endl;
        }
    }
    ~memory() {delete [] IMem; delete [] DMem;}

    unsigned int loadWordData(unsigned int index)
    {
        return ((0xff000000&(DMem[index]<<24)) | (0x00ff0000&(DMem[index+1]<<16)) |
         (0x0000ff00&(DMem[index+2]<<8)) | (0x000000ff&DMem[index+3]));
    }

    void saveWordData(unsigned int data, unsigned int index)
    {
        return;
    }

    unsigned int loadInstruction(unsigned int PC)
    {
        return ((0xff000000&(IMem[PC]<<24)) | (0x00ff0000&(IMem[PC+1]<<16)) |
         (0x0000ff00&(IMem[PC+2]<<8)) | (0x000000ff&IMem[PC+3]));
    }

    void saveInstruction(unsigned int instruction)
    {
        return;
    }    
};