#include "InstructionSet.hpp"
#include "Conversions.hpp"
#include <fstream>
#include <vector>
#include <array>
#include <utility>
#include <optional>
#include <string_view>
#include <filesystem>
#include <functional>
#include <charconv>
#include <format>

namespace
{

    std::string LookupOpcode(const Instruction &, const Parameter &, const std::span<std::byte>);
    std::string LookupMnemonic(const Instruction &, const Parameter &, const std::span<std::byte>);
    std::string LookupInstructionParameter(const Instruction &, const Parameter &, const std::span<std::byte>);

    using SubstitutionFunction_t = std::function<std::string(const Instruction &, const Parameter &, const std::span<std::byte>)>;
    using SubstitutionMap_t      = std::map<std::string_view, SubstitutionFunction_t>;

    using namespace std::literals;


    const std::string_view  DefaultInstructionDisplayTemplate{ "%m" };

    const SubstitutionMap_t SubstitutionFlags{
                                               {"%o"sv, LookupOpcode},
                                               {"%m"sv, LookupMnemonic},
                                               {"%p"sv, LookupInstructionParameter}
    };

    std::string RetrieveDisplay(const Instruction &instruction, const Parameter &parameter)
    {
        if ( !parameter.display.empty() )
            return parameter.display;

        return instruction.display;
    }

    bool HasDisplay(const Instruction &instruction, const Parameter &parameter)
    {
        if ( !parameter.display.empty() )
            return true;

        return !instruction.display.empty();
    }

    std::string LookupOpcode(const Instruction          &instruction,
                             const Parameter            &parameter,
                             const std::span<std::byte>  instruction_bytes)
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

    std::string LookupMnemonic(const Instruction          &instruction,
                               const Parameter            &parameter,
                               const std::span<std::byte>  instruction_bytes)
    {
        return instruction.mnemonic;
    }

    std::string LookupInstructionParameter(const Instruction          &instruction,
                                           const Parameter            &parameter,
                                           const std::span<std::byte>  instruction_bytes)
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

    std::string Decode(const std::span<std::byte>  input,
                       const Instruction          &instruction,
                       const Parameter            &parameter,
                             std::string           display_template)
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

bool InstructionSet::empty() const
{
    return _parameters.empty() && _instructions.empty();
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

std::optional<std::pair<ConstInstructionRef, ConstParameterRef>> InstructionSet::retrieveInstructionData(uint8_t opcode) const
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