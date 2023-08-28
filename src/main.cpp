#include <stdio.h>
#include <stdlib.h>

struct CPU
{
    using Byte = unsigned char;     // 8 bits
    using Word = unsigned short;    // 16 bits

    Word PC;    // Program Counter
    Word SP;    // Stack Pointer

    Byte A, X, Y;     // Accumulator, Index Register X, and Index Register Y

    // Processor Status Flags
    Byte C : 1;     // Carry Flag
    Byte Z : 1;     // Zero Flag
    Byte I : 1;     // Interrupt Disable
    Byte D : 1;     // Decimal Mode
    Byte B : 1;     // Break Command
    Byte V : 1;     // Overflow Flag
    Byte N : 1;     // Negative Flag
};

int main()
{
    CPU cpu;
    return 0;
}
