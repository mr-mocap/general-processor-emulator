#include "InstructionSet.hpp"
#include "IO.hpp"
#include <cstdlib>
#include <cassert>

InstructionSet Load6502InstructionSet()
{
    std::vector<Instruction> instructions = ReadInstructions( "6502" );
    std::vector<Parameter> parameters = ReadParameters( "6502" );

    return InstructionSet{ parameters, instructions };
}

void DefaultConstructed()
{
    InstructionSet is;

    assert( is.empty() );
}

void ConstructedWithInstructionsAndParameters()
{
    InstructionSet is = Load6502InstructionSet();

    assert( !is.empty() );
}

void AssembleEmptyLine()
{
    InstructionSet is = Load6502InstructionSet();

    AssembledInstruction result = is.assemble( "" );

    assert( result.empty() );
}

void AssembleInvalidInstruction()
{
    InstructionSet is = Load6502InstructionSet();

    AssembledInstruction result = is.assemble( "invalid_instruction" );

    assert( result.empty() );
}

void AssembleValidInstruction()
{
    InstructionSet is = Load6502InstructionSet();

    {
        AssembledInstruction result = is.assemble( "BRK" );

        assert( !result.empty() );
        assert( result.size() == 1 );
        assert( result[0] == 0x00 );
    }

    {
        AssembledInstruction result = is.assemble( "LDA\t#$01" );

        assert( !result.empty() );
        assert( result.size() == 2 );
        assert( result[0] == 0xA9 );
        assert( result[1] == 0x01 );
    }
}

int main()
{
    DefaultConstructed();
    ConstructedWithInstructionsAndParameters();
    AssembleEmptyLine();
    AssembleInvalidInstruction();
    AssembleValidInstruction();
    return EXIT_SUCCESS;
}