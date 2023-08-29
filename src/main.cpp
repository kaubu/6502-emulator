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
        if (Address > MAX_MEM) {
            printf("Address is out-of-bounds of max memory!\n");
        }
        return Data[Address];
    }

    // Write 1 byte
    Byte& operator[](u32 Address)
    {
        // Assert here Address is < MAX_MEM
        if (Address > MAX_MEM) {
            printf("Address is out-of-bounds of max memory!\n");
        }
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

    Byte ReadByte(u32& Cycles, Byte Address, Mem& memory)
    {
        Byte Data = memory[Address];
        Cycles--;
        return Data;
    }

    // Opcodes
    static constexpr Byte
        INS_LDA_IM  = 0xA9,     // Load Accumulator: Immediate Mode
        INS_LDA_ZP  = 0xA5,     // Load Accumulator: Zero Page
        INS_LDA_ZPX = 0xB5;     // Load Accumulator: Zero Page.X

    void LDASetStatus()
    {
        Z = (A == 0);
        // If bit 7 of the accumulator is set, then it's negative
        N = (A & 0b10000000) > 0;
    }

    void Execute(u32 Cycles, Mem& memory)
    {
        while (Cycles > 0)
        {
            Byte Ins = FetchByte(Cycles, memory);
            switch(Ins)
            {
                case INS_LDA_IM:
                {
                    Byte Value = FetchByte(Cycles, memory);
                    A = Value;
                    LDASetStatus();
                    break;    
                }
                
                case INS_LDA_ZP:
                {
                    Byte ZeroPageAddress = FetchByte(Cycles, memory);
                    A = ReadByte(Cycles, ZeroPageAddress, memory);
                    LDASetStatus();
                    break;
                }

                case INS_LDA_ZPX:
                {
                    Byte ZeroPageAddress = FetchByte(Cycles, memory);
                    ZeroPageAddress += X;
                    Cycles--;
                    A = ReadByte(Cycles, ZeroPageAddress, memory);
                    LDASetStatus();
                    break;
                }

                default:
                {
                    printf("Instruction not handled: %d\n", Ins);
                    break;
                }
            }
        }
    }
};

int main()
{
    Mem mem;
    CPU cpu;
    cpu.Reset(mem);
    // Inline a little program
    mem[0xFFFC] = CPU::INS_LDA_ZP;
    mem[0xFFFD] = 0x42;
    mem[0x0042] = 0x84;
    // End inline
    cpu.Execute(3, mem);
    return 0;
}
