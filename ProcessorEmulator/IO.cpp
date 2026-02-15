#include "IO.hpp"
#include "Conversions.hpp"
#include <filesystem>
#include <fstream>

#if defined(WIN32) || defined(WIN64)
static const std::filesystem::path DataDirectory{ std::filesystem::path("..") / ".." / ".." / "DataFiles" / "InstructionSet" };
#else
static const std::filesystem::path DataDirectory{ std::filesystem::path("..") / "DataFiles" / "InstructionSet" };
#endif

static const std::filesystem::path InstructionFileName{ "Instruction" };
static const std::filesystem::path ParameterFileName{ "Parameter" };
static const std::filesystem::path RegisterFileName{ "RegistersAssumedFromInstructionSet" };

std::filesystem::path InstructionSetPath(std::string_view processor_name)
{
    return DataDirectory / processor_name / InstructionFileName;
}

std::filesystem::path ParameterPath(std::string_view processor_name)
{
    return DataDirectory / processor_name / ParameterFileName;
}

std::filesystem::path RegisterPath(std::string_view processor_name)
{
    return DataDirectory / processor_name / RegisterFileName;
}

bool CanMakeInstruction(const std::vector<std::string> &row_values)
{
    return (row_values.size() == 3) || (row_values.size() == 4);
}

bool CanMakeParameter(const std::vector<std::string> &row_values)
{
    return (row_values.size() == 2) || (row_values.size() == 3);
}

bool CanMakeRegister(const std::vector<std::string> &row_values)
{
    // We need the NAME & BITS columns at least...
    return (row_values.size() == 2) || (row_values.size() == 3);
}

std::vector<std::string> BreakLine(std::string_view input)
{
    std::size_t final_position = 0;
    std::vector<std::string> retval;

    for ( std::pair<std::size_t, std::size_t> search_position = std::make_pair(0, 0);
        (search_position.second = input.find('\t', search_position.first)) != std::string_view::npos;
        search_position.first = search_position.second + 1, final_position = search_position.first
        )
    {
        retval.push_back(std::string{ input.substr(search_position.first, search_position.second - search_position.first) });
    }

    if ( final_position < input.size() )
        retval.push_back(std::string{ input.substr(final_position) });
    return retval;
}

std::vector<std::string> TrimRight(std::vector<std::string> &&line)
{
    if ( !line.empty() )
    {
        while ( line.back().empty() )
            line.pop_back();
    }
    return line;
}

std::vector<std::string> ReadLine(std::istream &input)
{
    std::string line;

    std::getline(input, line);

    return TrimRight( BreakLine(line) );
}

std::vector<Instruction> ReadInstructions(std::string_view processor_name)
{
    std::ifstream instruction_stream;
    std::filesystem::path instruction_file_path = InstructionSetPath(processor_name);

    instruction_stream.open( instruction_file_path );
    if ( instruction_stream.is_open() )
    {
        std::vector<Instruction> instructions;
        std::vector<std::string> column_values = ReadLine(instruction_stream);

        // The first line we throw away
        while ( !(column_values = ReadLine(instruction_stream)).empty() )
        {
            if ( CanMakeInstruction(column_values) )
                instructions.push_back( ToInstruction(column_values) );
        }

        return instructions;
    }

    return {};
}

std::vector<Parameter> ReadParameters(std::string_view processor_name)
{
    std::ifstream parameter_stream;
    std::filesystem::path parameter_file_path = ParameterPath(processor_name);

    parameter_stream.open( parameter_file_path );
    if ( parameter_stream.is_open() )
    {
        std::vector<Parameter> parameters;
        std::vector<std::string> column_values = ReadLine(parameter_stream);

        // The first line we throw away
        while ( !(column_values = ReadLine(parameter_stream)).empty() )
        {
            if ( CanMakeParameter(column_values) )
                parameters.push_back( ToParameter(column_values) );
        }

        return parameters;
    }

    return {};
}

std::vector<Register> ReadRegisters(std::string_view processor_name)
{
    std::ifstream register_stream;
    std::filesystem::path register_file_path = RegisterPath(processor_name);

    register_stream.open( register_file_path );
    if ( register_stream.is_open() )
    {
        std::vector<Register> registers;
        std::vector<std::string> column_values = ReadLine(register_stream);

        // The first line we throw away
        while ( !(column_values = ReadLine(register_stream)).empty() )
        {
            if ( CanMakeRegister(column_values) )
                registers.push_back( ToRegister(column_values) );
        }

        return registers;
    }

    return {};
}