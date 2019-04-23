

class ALU
{
private:
    /* data */
public:

    static unsigned int execute(unsigned int a, unsigned int b, unsigned int op)
    {
        switch (op & 0xffffffc0)
        {
        case 0x00000020:
            return (a + b);
            break;

        case 0x00000022:
            return (a - b);
            break;

        default:
            break;
        }
    }
};