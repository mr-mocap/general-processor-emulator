#include "Disassembler.hpp"
#include "IO.hpp"



bool Disassembler::loadInstructionSet(std::string_view processor_name)
{
    {
        std::filesystem::path instructions_filename = InstructionSetPath( processor_name );

        if ( instructions_filename.empty() )
            return false;

        if ( !std::filesystem::exists( instructions_filename ) )
            return false;
    }

    {
        std::filesystem::path parameters_filename = ParameterPath( processor_name );

        if ( parameters_filename.empty() )
            return false;

        if ( !std::filesystem::exists( parameters_filename ) )
            return false;
    }

    std::vector<Instruction> instructions = ReadInstructions( processor_name );

    if ( instructions.empty() )
        return false;

    std::vector<Parameter> parameters = ReadParameters( processor_name );

    if ( parameters.empty() )
        return false;

    m_instruction_set = InstructionSet( parameters, instructions );

    return true;
}

std::string Disassembler::disassemble(std::span<const std::byte> data)
{
    if ( data.empty() )
        return {};

    return m_instruction_set.disassemble( data );
}