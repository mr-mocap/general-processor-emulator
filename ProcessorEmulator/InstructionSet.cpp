#include "InstructionSet.hpp"
#include "Conversions.hpp"
#include "StringProcessing.hpp"
#include "IO.hpp"
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
#include <algorithm>
#include <system_error>


namespace
{

struct ParameterMatchResult
{
    std::vector<int> parameter_values;
    bool             matched = false;
};

    std::string LookupOpcode(const Instruction &, const Parameter &, std::span<const std::byte>);
    std::string LookupMnemonic(const Instruction &, const Parameter &, std::span<const std::byte>);
    std::string LookupInstructionParameter(const Instruction &, const Parameter &, std::span<const std::byte>);

    using SubstitutionFunction_t = std::function<std::string(const Instruction &, const Parameter &, std::span<const std::byte>)>;
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

    std::string LookupOpcode(const Instruction                &instruction,
                             const Parameter                  &parameter,
                                   std::span<const std::byte>  instruction_bytes)
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

    std::string LookupMnemonic(const Instruction                &instruction,
                               const Parameter                  &parameter,
                                     std::span<const std::byte>  instruction_bytes)
    {
        return instruction.mnemonic;
    }

    std::string LookupInstructionParameter(const Instruction                &instruction,
                                           const Parameter                  &parameter,
                                                 std::span<const std::byte>  instruction_bytes)
    {
#if 1
        std::span<const std::byte> parameter_bytes{ instruction_bytes.subspan(1) };

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

    std::string Decode(      std::span<const std::byte>  input,
                       const Instruction                &instruction,
                       const Parameter                  &parameter,
                             std::string                 display_template)
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

    ParameterMatchResult DisplayMatchesPattern(std::string_view display_template, std::string_view input)
    {
        std::vector<int> matches;
        std::vector<std::string_view> display_chunks = SplitString(display_template, "%p");

        for (std::string_view chunk : display_chunks)
        {
            if ( chunk == "%p" )
            {
                // We need to read a number
                ReadNumberResult result = ReadNumber(input);

                if ( result.value == -1 )
                    return {}; // No match!

                matches.push_back( result.value );
                input = result.remaining_input;
                continue;
            }

            // Otherwise, we need to match the chunk at the front of the input.
            if ( !input.starts_with( chunk ) )
                return {}; // No match!

            input.remove_prefix( chunk.size() );
        }

        return { matches, true };
    }
}

bool InstructionSet::empty() const
{
    return _parameters.empty() && _instructions.empty();
}

std::string InstructionSet::disassemble(std::span<const std::byte> input_instruction) const
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

std::u8string InstructionSet::assemble(std::string_view tab_separated_line) const
{
    if ( tab_separated_line.empty() )
        return {};
    
    // Break the line into tokens and try to match the first token to an instruction mnemonic.
    // If we find a match, we can then try to parse the rest of the tokens as parameters,
    // and if that succeeds, we can assemble the instruction.
    std::vector<std::string_view> tokens = BreakLineView(tab_separated_line);

    if ( tokens.empty() )
        return {};

    const std::string_view mnemonic = tokens[0];

    if ( mnemonic.empty() )
        return {};

    tokens.erase(tokens.begin()); // Remove the mnemonic from the tokens, leaving only the parameters.

    std::vector<ConstInstructionRef> instruction_data = findInstructions( mnemonic );

    if ( instruction_data.empty() )
        return {}; // No instruction with a matching mnemonic was found.

    std::u8string assembled_instruction;

    // Let's try to find an instruction with a matching parameter
    for (const Instruction &instruction : instruction_data)
    {
        const Parameter *parameter = findParameter( instruction.mode );

        if ( !parameter )
            continue;

        // Check for mismatch of parameter size and number of parameters provided in the input line.

        if ( parameter->size == 0 && !tokens.empty() )
            continue; // This instruction doesn't take any parameters, but we have some, so this isn't a match.

        if ( parameter->size > 0 && tokens.empty() )
            continue; // This instruction requires parameters, but we don't have any, so this isn't a match.

        std::string display_template = RetrieveDisplay(instruction, *parameter); 

        // Check for having a display template and an operand
        if ( !display_template.empty() && !tokens.empty() )
        {
            // Possible match
            ParameterMatchResult parameter_match_result = DisplayMatchesPattern(display_template, tokens.front());

            if ( !parameter_match_result.matched )
                continue; // The display template doesn't match the input line, so this isn't a match.

            assert( parameter->mode == instruction.mode ); // This should always be true, but we want to make sure.

            // Matched!

            // Go ahead and put the opcode in
            assembled_instruction.push_back( static_cast<std::uint8_t>(instruction.opcode) );

            // Let's check if the parameters we matched are valid for this instruction's operand.
            for ( int parameter_value : parameter_match_result.parameter_values )
            {
                if ( parameter_value >= 0 )
                {
                    int max_value = (1 << (parameter->size * 8)) - 1;

                    if ( parameter_value > max_value )
                        continue; // This parameter value is too large for the expected parameter size, so this isn't a match.

                    // MATCH

                    // Append the parameter bytes to the assembled instruction
                    // auto bytes = std::as_bytes( std::span{ &parameter_value  } );
                    // assembled_instruction.push_back( static_cast<std::uint8_t>( parameter_value ) );
                }
            }
        }

        if ( display_template.empty() && tokens.empty())
            return std::u8string{ static_cast<std::uint8_t>(instruction.opcode) }; // MATCH
    }

    // If we get here, we have a matching instruction and parameters.
    // std::u8string assembled_instruction;
    // assembled_instruction.push_back(static_cast<std::uint8_t>(opcode));
    // return assembled_instruction;

    return {};
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

std::vector<ConstInstructionRef> InstructionSet::findInstructions(std::string_view mnemonic) const
{
    std::vector<ConstInstructionRef> results;
    size_t count = 0;

    for ( const auto &[opcode, instruction] : _instructions )
    {
        if ( instruction.mnemonic == mnemonic )
            count++;
    }

    results.reserve(count);

    for ( const auto &[opcode, instruction] : _instructions )
    {
        if ( instruction.mnemonic == mnemonic )
            results.push_back( std::cref(instruction) );
    }
    return results;
}

const Parameter *InstructionSet::findParameter(std::string_view mode) const
{
    const auto parameter_iter = _parameters.find(mode);

    if ( parameter_iter == _parameters.end() )
        return nullptr;

    return &parameter_iter->second;
}