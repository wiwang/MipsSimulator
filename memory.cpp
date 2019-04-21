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
        return ();
    }

    void saveWordData(unsigned int data)
    {
        return;
    }

    unsigned int loadInstruction(unsigned int PC)
    {
        return ();
    }

    void saveInstruction(unsigned int instruction)
    {
        return;
    }    
};