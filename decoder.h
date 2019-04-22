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