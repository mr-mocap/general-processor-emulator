#include "Assembler.hpp"
#include "IO.hpp"


bool Assembler::loadInstructionSet(std::string_view processor_name)
{
    if ( !InstructionSetExists( processor_name ) )
        return false;

    m_instruction_set = ReadInstructionSet( processor_name );

    return !m_instruction_set.empty();
}

std::span<const std::byte> Assembler::assemble(std::string_view assembly_instruction)
{
    if ( assembly_instruction.empty() )
        return {};
    
    return m_instruction_set.assemble( assembly_instruction );
}