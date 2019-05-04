#include <sys/time.h>
#include <string.h>

#include <iostream>

#include "memory.h"
#include "pipeline.h"

using namespace std;

int main(int argc, char *argv[])
{
	struct timeval stuTime; 
	int ret = 0;   
    char fileName[50] = {0};
    char modeString;
    MipsMode mode;
    unsigned int execution = 0;
    char execStr[20];
    unsigned int clockTick = 0;
    bool end = false;

	memset(&stuTime, 0, sizeof(struct timeval));  
    stuTime.tv_sec = 1;  
    stuTime.tv_usec = 0;  


    cout << "Enter the assembly file name:" << endl;
    cin.getline(fileName, sizeof(fileName));

    cout << "Enter the MIPS running mode:" << endl;
    cout << "0 : Instruction Mode" << endl;
    cout << "1 : Cycle mode" << endl;
    cin >> &modeString;
    cin.ignore();
    mode = (MipsMode)atoi(&modeString);

    cout << "Select the number of instructions:" << endl;
    cin >> execStr;
    cin.ignore();
    execution = atoi(execStr);

    /* initiate memory and pipeline*/
    memory m;
    m.saveBinaryProgram(fileName);

    pipeline p(&m);

    /* run the pipeline in clock cycle */
    while (true && execution && !end)
    {
        ret = select(0 ,NULL, NULL,NULL, &stuTime);  
        if(ret == 0)
        {
            clockTick++;
            cout<<"clock 0x"<<clockTick<<" come in!"<<endl;
            /* execute the pipeline every 1 second */
            end = p.execute(mode);   
            execution--;
        }
    }

    p.displayResult();

    return 0;
}