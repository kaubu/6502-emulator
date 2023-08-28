#include <stdio.h>
#include <stdlib.h>

using Byte = unsigned char;     // 8 bits
using Word = unsigned short;    // 16 bits

using u32 = unsigned int;

struct Mem
{
    static constexpr u32 MAX_MEM = 1024 * 64;
    Byte Data[MAX_MEM];

    void Initialize()
    {
        for (u32 i = 0; i < MAX_MEM; i++)
        {
            Data[i] = 0;
        }
    }
};

struct CPU
{
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

    void Reset(Mem& memory)
    {
        PC = 0xFFFC;
        SP = 0x0100;
        // Clear flags
        C = Z = I = D = B = V = N = 0;
        A = X = Y = 0;
        memory.Initialize();
    }
};

int main()
{
    Mem mem;
    CPU cpu;
    cpu.Reset(mem);
    return 0;
}
