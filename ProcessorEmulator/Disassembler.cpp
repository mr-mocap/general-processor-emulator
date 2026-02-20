#include "Disassembler.hpp"
#include "IO.hpp"



bool Disassembler::loadInstructionSet(std::string_view processor_name)
{
    if ( !InstructionSetExists( processor_name ) )
        return false;

    m_instruction_set = ReadInstructionSet( processor_name );

    return !m_instruction_set.empty();
}

std::string Disassembler::disassemble(std::span<const std::byte> data)
{
    if ( data.empty() )
        return {};

    return m_instruction_set.disassemble( data );
}