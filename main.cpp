#include <iostream>
using namespace std;

enum MipsMode
{
    MIPS_InstructionMode = 0,
    MIPS_CycleMode,
    MIPS_MaxMode
};


int main(int argc, char *argv[])
{
    char fileName[50] = {0};
    MipsMode mode;
    unsigned int execution = 0;
/*
    cout << "Enter the assembly file name:" << endl;
    cin.getline(fileName, sizeof(fileName));

    cout << "Enter the MIPS running mode:" << endl;

    cout << "Select the number of instructions:" << endl;
*/
    cout << sizeof(unsigned long int) << endl;

    return 0;
}