#pragma once

#include <cstdint>
#include <cstddef>
#include <string>
#include <map>
#include <optional>
#include <utility>
#include <span>


class InstructionSet
{
public:

    struct Instruction
    {
        Instruction() = default;
        explicit Instruction(size_t o,
            const std::string &mn,
            const std::string &mo)
            :
            opcode(o),
            mnemonic(mn),
            mode(mo)
        {
        }
        explicit Instruction(size_t o,
            const std::string &mn,
            const std::string &mo,
            const std::string &d)
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
        explicit Parameter(size_t  s,
            const std::string &m)
            :
            mode{ m },
            size{ s }
        {
        }
        explicit Parameter(size_t  s,
            const std::string &m,
            const std::string &d)
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

    void fetch();

    std::string disassemble(const std::span<std::byte> input_instruction) const;

    std::optional<std::pair<Instruction, Parameter>> retrieveInstructionData(uint8_t opcode) const;
protected:
    std::map<std::string, Parameter> _parameters;
    std::map<size_t, Instruction>    _instructions;

};
