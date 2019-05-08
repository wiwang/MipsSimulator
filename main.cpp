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
    char continueString[20];

    unsigned int clockTick = 0;
    bool end = false;

    /* set the timer interval as 1 second*/
	memset(&stuTime, 0, sizeof(struct timeval));  
    stuTime.tv_sec = 1;  
    stuTime.tv_usec = 0;  

    cout << "Enter the assembly file name:" << endl;
    cin.getline(fileName, sizeof(fileName));

    /* initiate memory and pipeline*/
    memory m;
    m.saveBinaryProgram(fileName);

    pipeline p(&m);

    repeat:
    memset(continueString, 0, sizeof(continueString));
    memset(execStr, 0, sizeof(execStr));

    /* input the number of clock cycle the pipeline can run*/
    cout << "Select the number of instructions:" << endl;
    cin >> execStr;
    cin.ignore();
    execution = atoi(execStr);

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