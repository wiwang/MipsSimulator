

class ALU
{
private:
    /* data */
public:

    static unsigned int execute(unsigned int a, unsigned int b, unsigned int instruction)
    {
        switch (instruction & 0xffffffc0)
        {
        case 0x00000020://add
            return (a + b);
            break;

        case 0x00000022://sub
            return (a - b);
            break;

        case 0x00000024://and
            return (a & b);
            break;

        case 0x00000025://or
            return (a | b);
            break;

        case 0x00000026://slt
            return (a - b);
            break;                                    

        default:
            break;
        }
    }
};