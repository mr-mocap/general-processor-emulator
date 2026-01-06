#include "InstructionSet.hpp"
#include <fstream>
#include <vector>
#include <array>
#include <utility>
#include <optional>
#include <string_view>
#include <filesystem>
#include <functional>
#include <charconv>
#include <cstring>
#include <format>

namespace
{

    std::string LookupOpcode(const InstructionSet::Instruction &, const InstructionSet::Parameter &, const std::span<std::byte>);
    std::string LookupMnemonic(const InstructionSet::Instruction &, const InstructionSet::Parameter &, const std::span<std::byte>);
    std::string LookupInstructionParameter(const InstructionSet::Instruction &, const InstructionSet::Parameter &, const std::span<std::byte>);

    using SubstitutionFunction_t = std::function<std::string(const InstructionSet::Instruction &, const InstructionSet::Parameter &, const std::span<std::byte>)>;
    using SubstitutionMap_t = std::map<std::string_view, SubstitutionFunction_t>;

    using namespace std::literals;


    const std::filesystem::path DataDirectory{ std::filesystem::path("DataFiles") / "InstructionSet" / "6502" };
    const std::filesystem::path InstructionFileName{ "Instruction" };
    const std::filesystem::path ParameterFileName{ "Parameter" };
    const std::string_view      DefaultInstructionDisplayTemplate{ "%m" };

    const SubstitutionMap_t SubstitutionFlags{
                                               {"%o"sv, LookupOpcode},
                                               {"%m"sv, LookupMnemonic},
                                               {"%p"sv, LookupInstructionParameter}
    };

    template <class BasicType>
    static constexpr BasicType To(std::span<const std::byte> bytes) requires(std::is_arithmetic_v<BasicType> || std::is_enum_v<BasicType>)
    {
        BasicType result; // We do this for memory alignment of the variable type

        std::memcpy(&result, bytes.data(), sizeof(BasicType));
        return result;
    }

    // Splits a string at tabs
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

    bool CanMakeInstruction(const std::vector<std::string> &row_values)
    {
        return (row_values.size() == 3) || (row_values.size() == 4);
    }

    bool CanMakeParameter(const std::vector<std::string> &row_values)
    {
        return (row_values.size() == 2) || (row_values.size() == 3);
    }

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

    InstructionSet::Instruction ToInstruction(const std::vector<std::string> &row_values)
    {
        std::optional<size_t> s = HexStringToSizeT(row_values[0]);

        if ( !s.has_value() )
            return {};

        return (row_values.size() == 4) ? InstructionSet::Instruction{ s.value(), row_values[1], row_values[2], row_values[3] } :
            InstructionSet::Instruction{ s.value(), row_values[1], row_values[2] };
    }

    InstructionSet::Parameter ToParameter(const std::vector<std::string> &row_values)
    {
        std::optional<size_t> s = DecimalStringToSizeT(row_values[1]);

        if ( !s.has_value() )
            return {};

        return (row_values.size() == 3) ? InstructionSet::Parameter{ s.value(), row_values[0], row_values[2] } :
            InstructionSet::Parameter{ s.value(), row_values[0] };
    }

    std::vector<InstructionSet::Instruction> ReadInstructions()
    {
        std::ifstream instruction_stream;

        instruction_stream.open(DataDirectory / InstructionFileName);
        if ( instruction_stream.is_open() )
        {
            std::vector<InstructionSet::Instruction> instructions;
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

    std::vector<InstructionSet::Parameter> ReadParameters()
    {
        std::ifstream parameter_stream;

        parameter_stream.open(DataDirectory / ParameterFileName);
        if ( parameter_stream.is_open() )
        {
            std::vector<InstructionSet::Parameter> parameters;
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

    std::string RetrieveDisplay(const InstructionSet::Instruction &instruction, const InstructionSet::Parameter &parameter)
    {
        if ( !parameter.display.empty() )
            return parameter.display;

        return instruction.display;
    }

    bool HasDisplay(const InstructionSet::Instruction &instruction, const InstructionSet::Parameter &parameter)
    {
        if ( !parameter.display.empty() )
            return true;

        return !instruction.display.empty();
    }

    std::string LookupOpcode(const InstructionSet::Instruction &instruction,
        const InstructionSet::Parameter &parameter,
        const std::span<std::byte>         instruction_bytes)
    {
#if 1
        return std::format("{:x}", instruction.opcode);
#else
        std::array<char, 8> buffer;

        std::to_chars_result result = std::to_chars(buffer.data(),
            buffer.data() + buffer.size(),
            instruction.opcode,
            16);

        if ( result.ec != std::errc() )
            return {}; // Bad Value!

        size_t length = result.ptr - buffer.data();

        return { buffer.data(), length };
#endif
    }

    std::string LookupMnemonic(const InstructionSet::Instruction &instruction,
        const InstructionSet::Parameter &parameter,
        const std::span<std::byte>         instruction_bytes)
    {
        return instruction.mnemonic;
    }

    std::string LookupInstructionParameter(const InstructionSet::Instruction &instruction,
        const InstructionSet::Parameter &parameter,
        const std::span<std::byte>         instruction_bytes)
    {
#if 1
        const std::span<std::byte> parameter_bytes{ instruction_bytes.subspan(1) };

        if ( parameter_bytes.size() != parameter.size )
            return {}; // ERROR

        if ( parameter_bytes.size() == 1 )
            return std::format("${:x}", To<std::uint8_t>(parameter_bytes));
        else if ( parameter_bytes.size() == 2 )
            return std::format("${:x}", To<std::uint8_t>(parameter_bytes));

        return {};
#else
        const std::span<std::byte> parameter_bytes{ instruction_bytes.subspan(1) };
        std::array<char, 8>  buffer{ '$' };
        std::to_chars_result result;

        if ( parameter.size == 1 )
        {
            std::uint8_t p_8bit = To<std::uint8_t>(parameter_bytes);

            result = std::to_chars(buffer.data() + 1,
                buffer.data() + buffer.size(),
                instruction.opcode,
                16);
        }
        else if ( parameter.size == 2 )
        {
            std::uint16_t p_16_bit = To<std::uint16_t>(parameter_bytes);

            result = std::to_chars(buffer.data() + 1,
                buffer.data() + buffer.size(),
                instruction.opcode,
                16);
        }

        if ( result.ec != std::errc() )
            return {}; // Bad Value!

        size_t length = result.ptr - buffer.data();

        return { buffer.data(), length };
#endif
    }

    std::string Decode(const std::span<std::byte>         input,
        const InstructionSet::Instruction &instruction,
        const InstructionSet::Parameter &parameter,
        std::string                  display_template)
    {
        std::size_t search_position = 0;
        std::vector<std::string> retval;

        // Let's do a replacement on all of the substitution patterns...
        for ( const auto &[key, value] : SubstitutionFlags )
        {
            // Take care of multiple instances of the same pattern...
            for ( size_t pos = display_template.find(key);
                pos != std::string::npos;
                pos = display_template.find(key) )
            {
                std::string t = value(instruction, parameter, input);

                display_template.replace(pos, key.size(), t);
            }
        }

        return display_template;
    }

}

void InstructionSet::fetch()
{
    std::vector<InstructionSet::Parameter>   parameters = ReadParameters();
    std::vector<InstructionSet::Instruction> instructions = ReadInstructions();

    for ( const Parameter &parameter : parameters )
        _parameters[parameter.mode] = parameter;

    for ( const Instruction &instruction : instructions )
        _instructions[instruction.opcode] = instruction;
}

std::string InstructionSet::disassemble(const std::span<std::byte> input_instruction) const
{
    if ( input_instruction.empty() )
        return {};

    auto instruction_data = retrieveInstructionData(To<std::uint8_t>(input_instruction.subspan(0, 1)));

    if ( !instruction_data )
        return {};

    std::string entire_instruction_display{ DefaultInstructionDisplayTemplate };

    if ( HasDisplay(instruction_data.value().first, instruction_data.value().second) )
        entire_instruction_display.append(" ").append(RetrieveDisplay(instruction_data.value().first, instruction_data.value().second));

    return Decode(input_instruction, instruction_data.value().first, instruction_data.value().second, entire_instruction_display);
}

std::optional<std::pair<InstructionSet::ConstInstructionRef, InstructionSet::ConstParameterRef>> InstructionSet::retrieveInstructionData(uint8_t opcode) const
{
    const auto instruction_iter = _instructions.find(opcode);

    if ( instruction_iter == _instructions.end() )
        return std::nullopt;

    const Instruction &i = instruction_iter->second;
    const auto parameter_iter = _parameters.find(i.mode);

    if ( parameter_iter == _parameters.end() )
        return std::nullopt;

    return std::make_optional( std::make_pair( std::ref(i), std::ref(parameter_iter->second) ) );
}