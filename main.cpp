#include <sys/time.h>

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
	struct timeval stuTime; 
	int ret = 0;   
    char fileName[50] = {0};
    MipsMode mode;
    unsigned int execution = 0;
    unsigned int clockTick = 0;

	memset(&stuTime, 0, sizeof(struct timeval));  
    stuTime.tv_sec = 1;  
    stuTime.tv_usec = 0;  


    cout << "Enter the assembly file name:" << endl;
    cin.getline(fileName, sizeof(fileName));

    cout << "Enter the MIPS running mode:" << endl;

    cout << "Select the number of instructions:" << endl;

    cout << sizeof(unsigned long int) << endl;

    /* initiate memory and pipeline*/
    memory m;
    pipeline p(m);

    /* run the pipeline in clock cycle */
    while (true)
    {
        ret = select(0 ,NULL, NULL,NULL, &stuTime);  
        if(ret == 0)
        {
            cout<<"set_timer_s time come in:"<<endl;
            /* execute the pipeline every 1 second */
            p.execute();
            
        }       
        
    }

    return 0;
}