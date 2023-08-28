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

    // Read 1 byte
    Byte operator[](u32 Address) const
    {
        // Assert here Address is < MAX_MEM
        return Data[Address];
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

    Byte FetchByte(u32& Cycles, Mem& memory)
    {
        Byte Data = memory[PC];
        PC++;
        Cycles--;
        return Data;
    }

    // Opcodes
    // Load Accumulator Immediate mode
    static constexpr Byte INS_LDA_IM = 0xA9;

    void Execute(u32 Cycles, Mem& memory)
    {
        while (Cycles > 0)
        {
            Byte Instruction = FetchByte(Cycles, memory);
            switch(Instruction)
            {
                case INS_LDA_IM:
                {
                    Byte Value = FetchByte(Cycles, memory);
                    A = Value;
                    Z = (A == 0);
                    // If bit 7 of the accumulator is set, then it's negative
                    N = (A & 0b10000000) > 0;    
                }

                break;
            }
        }
    }
};

int main()
{
    Mem mem;
    CPU cpu;
    cpu.Reset(mem);
    cpu.Execute(2, mem);
    return 0;
}
