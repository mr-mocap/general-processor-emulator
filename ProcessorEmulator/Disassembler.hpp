#pragma once

#include "InstructionSet.hpp"
#include <string>
#include <string_view>
#include <span>
#include <cstddef>

class Disassembler
{
public:
    Disassembler() = default;

    bool loadInstructionSet(std::string_view processor_name);

    std::string disassemble(std::span<const std::byte> data);
protected:
    InstructionSet m_instruction_set;
};