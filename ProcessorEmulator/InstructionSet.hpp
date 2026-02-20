#pragma once

#include <cstdint>
#include <cstddef>
#include <string>
#include <map>
#include <optional>
#include <utility>
#include <span>
#include <functional>
#include <ranges>


struct Instruction
{
    Instruction() = default;
    Instruction(size_t o,
                std::string_view mn,
                std::string_view mo)
        :
        opcode(o),
        mnemonic(mn),
        mode(mo)
    {
    }
    Instruction(size_t o,
                std::string_view mn,
                std::string_view mo,
                std::string_view d)
        :
        opcode(o),
        mnemonic(mn),
        mode(mo),
        display(d)
    {
    }

    size_t       opcode = 0;
    std::string  mnemonic;
    std::string  mode;
    std::string  display;
};

struct Parameter
{
    Parameter() = default;
    Parameter(size_t           s,
              std::string_view m)
        :
        mode{ m },
        size{ s }
    {
    }
    Parameter(size_t           s,
              std::string_view m,
              std::string_view d)
        :
        mode{ m },
        size{ s },
        display{ d }
    {
    }

    std::string  mode;
    size_t       size = 0;
    std::string  display;
};

using InstructionRef = std::reference_wrapper<Instruction>;
using ParameterRef   = std::reference_wrapper<Parameter>;
using ConstInstructionRef = std::reference_wrapper<const Instruction>;
using ConstParameterRef   = std::reference_wrapper<const Parameter>;


class InstructionSet
{
public:
    InstructionSet() = default;
    
    template <std::ranges::range ParameterRangeType, std::ranges::range InstructionRangeType>
    InstructionSet(const ParameterRangeType   &parameters,
                   const InstructionRangeType &instructions)
    {
        for ( const Parameter &parameter : parameters )
            _parameters.insert_or_assign( parameter.mode, parameter );

        for ( const Instruction &instruction : instructions )
            _instructions.insert_or_assign( instruction.opcode, instruction );
    }

    bool empty() const;

    /** Disassembles the given instruction into a human readable format
     * 
     * @param input_instruction The bytes of the instruction to disassemble
     *
     * @return A human readable string representing the instruction,
     *         or an empty string if the instruction could not be disassembled
     */
    std::string disassemble(std::span<const std::byte> input_instruction) const;

    /** Assembles the given line of assembly code into machine code
     * 
     * @param line A line of assembly code to assemble
     *
     * @return The machine code bytes representing the instruction,
     *         or an empty span if the instruction could not be assembled
     */
    std::u8string assemble(std::string_view line) const;

    std::optional<std::pair<ConstInstructionRef, ConstParameterRef>> retrieveInstructionData(uint8_t opcode) const;

    std::vector<ConstInstructionRef> findInstructions(std::string_view mnemonic) const;

    const Parameter *findParameter(std::string_view mode) const;
protected:
    std::map<std::string, Parameter, std::less<>> _parameters;
    std::map<size_t, Instruction>                 _instructions;
};
