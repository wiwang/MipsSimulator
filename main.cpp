#include <Windows.h>
#include <string.h>

#include <iostream>

#include "memory.h"
#include "pipeline.h"

using namespace std;

int main(int argc, char *argv[])
{
	int ret = 0;   
    char fileName[50] = {0};
    char modeString;
    MipsMode mode;
    unsigned int execution = 0;
    char execStr[20];
    char continueString[20];

    unsigned int clockTick = 0;
    bool end = false;  

    cout << "Enter the assembly file name:" << endl;
    cin.getline(fileName, sizeof(fileName));
    /* initiate memory and pipeline*/
    memory m;
    m.saveBinaryProgram(fileName);

    cout << "Enter the MIPS running mode:" <<endl;
    cout << "0 : Instruction Mode" <<endl;
    cout << "1 : Cycle Mode" <<endl;
    cin >> &modeString;
    cin.ignore();
    mode = (MipsMode)atoi(&modeString);    

    pipeline p(&m, mode);

    repeat:
    memset(continueString, 0, sizeof(continueString));
    memset(execStr, 0, sizeof(execStr));

    /* input the number of clock cycle the pipeline can run*/
    cout << "Select the number of instructions:" << endl;
    cin >> execStr;
    cin.ignore();
    execution = atoi(execStr);

    p.setExecNumber(execution);

    /* run the pipeline in clock cycle */
    while (true && p.getExecNumber() && !end)
    {
        Sleep(10); //sleep 1 second per clock tick
        clockTick++;
        //cout<<"clock 0x"<<clockTick<<" come in!"<<endl;
        /* execute the pipeline every 1 second */
        end = p.execute();
    }

    if (!end)
    {
        cout << "Continue to execute instructions?" << endl;
        cout << "yes|no" << endl;
        cin >> continueString;
        cin.ignore();
    }

    if (strcmp(continueString, "yes")==0)
    {
        goto repeat;
    }

    p.displayResult();

    return 0;
}