#include "decoder.h"

class decoder
{
private:

public:
    static bool isRInstruction(unsigned int i) {}
    static bool isJInstruction(unsigned int i) {}
    static bool isBranchInstruction(unsigned int i) {}

    static unsigned char getRsField(unsigned int i) {}
    static unsigned char getRtField(unsigned int i) {}
    static unsigned int getOffsetField(unsigned int i) {}
    static unsigned char getFunctField(unsigned int i) {}

};