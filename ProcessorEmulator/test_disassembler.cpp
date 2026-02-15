#include "Disassembler.hpp"
#include <cstdlib>
#include <cassert>


void LoadInstructionSet()
{
    Disassembler da;

    assert( da.loadInstructionSet("6502") );
}

void Disassemble()
{
    Disassembler da;

    assert( da.loadInstructionSet("6502") );

    // BRK
    {
        unsigned char data[] = { 0x00 };
        std::span<const std::byte> bytes = std::as_bytes( std::span( data ) );

        assert( da.disassemble( bytes ) == "BRK" );
    }

    // BPL 
    {
        {
            unsigned char data[] = { 0x10, 0x00 };
            std::span<const std::byte> bytes = std::as_bytes( std::span( data ) );

            assert( da.disassemble( bytes ) == "BPL $0" );
        }
        {
            unsigned char data[] = { 0x10, 0x01 };
            std::span<const std::byte> bytes = std::as_bytes( std::span( data ) );

            assert( da.disassemble( bytes ) == "BPL $1" );
        }
        {
            unsigned char data[] = { 0x10, 0x7F };
            std::span<const std::byte> bytes = std::as_bytes( std::span( data ) );

            assert( da.disassemble( bytes ) == "BPL $7f" );
        }
    }
}

int main()
{
    LoadInstructionSet();
    Disassemble();

    return EXIT_SUCCESS;
}