typedef struct RFormat
{
    unsigned int op: 6;
    unsigned int rs: 5;
    unsigned int rt: 5;
    unsigned int rd: 5;
    unsigned int shamt: 5;
    unsigned int funct: 6;
} RInstruction;

typedef struct IFormat
{
    unsigned int op: 6;
    unsigned int rs: 5;
    unsigned int rt: 5;
    unsigned int offset: 16;
} IInstruction;

typedef struct JFormat
{
    unsigned int op: 6;
    unsigned int address: 26;
} JInstruction;

typedef union instruction
{
    RInstruction r;
    IInstruction i;
    JInstruction j;
} Instruction;


unsigned int const OP_MASK = 0xfc000000;
unsigned int const RS_MASK = 0x03e00000;
unsigned int const RT_MASK = 0x001f0000;
unsigned int const RD_MASK = 0x0000f800;
unsigned int const SHAMT_MASK = 0x000007c0;
unsigned int const FUNCT_MASK = 0x0000003f;
unsigned int const IMM_MASK = 0x0000ffff;
unsigned int const TARGET_MASK = 0x03ffffff;

unsigned int const R_INSTRUCTION = 0x00000000;
unsigned int const J_INSTRUCTION = 0x08000000;
unsigned int const LW_INSTRUCTION = 0x8c000000;
unsigned int const SW_INSTRUCTION = 0xac000000;
unsigned int const BRANCH_INSTRUCTION = 0x10000000;
unsigned int const ORI_INSTRUCTION = 0x34000000;
unsigned int const ANDI_INSTRUCTION = 0x30000000;
unsigned int const XOR_INSTRUCTION = 0x38000000;

class decoder
{
private:

public:
    static bool isRInstruction(unsigned int instruction) {}
    static bool isJInstruction(unsigned int instruction) {}
    static bool isBranchInstruction(unsigned int instruction) {}

    static bool isLoadInstruction(unsigned int instruction) {}
    static bool isSaveInstruction(unsigned int instruction) {}

    static unsigned int getRsField(unsigned int instruction) {}
    static unsigned int getRtField(unsigned int instruction) {}
    static unsigned int getRdField(unsigned int instruction) {}
    static unsigned int getOffsetField(unsigned int instruction) {}
    static unsigned int getFunctField(unsigned int instruction) {}
    static bool isOriInstruction(unsigned int instruction) {}
    static bool isAndiInstruction(unsigned int instruction) {}
    static bool isXorInstruction(unsigned int instruction) {}
};