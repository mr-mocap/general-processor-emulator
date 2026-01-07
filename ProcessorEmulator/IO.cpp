#include "IO.hpp"
#include <filesystem>
#include <fstream>
#include <charconv>


static const std::filesystem::path DataDirectory{ std::filesystem::path("DataFiles") / "InstructionSet" / "6502" };
static const std::filesystem::path InstructionFileName{ "Instruction" };
static const std::filesystem::path ParameterFileName{ "Parameter" };


std::optional<size_t> DecimalStringToSizeT(const std::string &input)
{
    size_t retval;

    if ( std::from_chars(input.data(), input.data() + input.size(), retval).ec == std::errc{} )
        return retval;

    return std::nullopt;
}

std::optional<size_t> HexStringToSizeT(const std::string &input)
{
    size_t retval;

    if ( std::from_chars(input.data(), input.data() + input.size(), retval, 16).ec == std::errc{} )
        return retval;

    return std::nullopt;
}

Instruction ToInstruction(const std::vector<std::string> &row_values)
{
    std::optional<size_t> s = HexStringToSizeT(row_values[0]);

    if ( !s.has_value() )
        return {};

    return (row_values.size() == 4) ? Instruction{ s.value(), row_values[1], row_values[2], row_values[3] } :
        Instruction{ s.value(), row_values[1], row_values[2] };
}

Parameter ToParameter(const std::vector<std::string> &row_values)
{
    std::optional<size_t> s = DecimalStringToSizeT(row_values[1]);

    if ( !s.has_value() )
        return {};

    return (row_values.size() == 3) ? Parameter{ s.value(), row_values[0], row_values[2] } :
        Parameter{ s.value(), row_values[0] };
}

bool CanMakeInstruction(const std::vector<std::string> &row_values)
{
    return (row_values.size() == 3) || (row_values.size() == 4);
}

bool CanMakeParameter(const std::vector<std::string> &row_values)
{
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

std::vector<std::string> ReadLine(std::istream &input)
{
    std::string line;

    std::getline(input, line);

    auto result = BreakLine(line);

    if ( !result.empty() )
    {
        while ( result.back().empty() )
            result.pop_back();
    }
    return result;
}

std::vector<Instruction> ReadInstructions()
{
    std::ifstream instruction_stream;

    instruction_stream.open(DataDirectory / InstructionFileName);
    if ( instruction_stream.is_open() )
    {
        std::vector<Instruction> instructions;
        std::vector<std::string> column_values = ReadLine(instruction_stream);

        // The first line we throw away
        while ( !(column_values = ReadLine(instruction_stream)).empty() )
        {
            if ( CanMakeInstruction(column_values) )
                instructions.push_back(ToInstruction(column_values));
        }

        return instructions;
    }

    return {};
}

std::vector<Parameter> ReadParameters()
{
    std::ifstream parameter_stream;

    parameter_stream.open(DataDirectory / ParameterFileName);
    if ( parameter_stream.is_open() )
    {
        std::vector<Parameter> parameters;
        std::vector<std::string> column_values = ReadLine(parameter_stream);

        // The first line we throw away
        while ( !(column_values = ReadLine(parameter_stream)).empty() )
        {
            if ( CanMakeParameter(column_values) )
                parameters.push_back(ToParameter(column_values));
        }

        return parameters;
    }

    return {};
}
