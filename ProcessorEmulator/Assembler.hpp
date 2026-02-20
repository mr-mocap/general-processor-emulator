#pragma once

#include "InstructionSet.hpp"
#include <string>
#include <string_view>
#include <span>
#include <cstddef>

class Assembler
{
public:
    Assembler() = default;

    bool loadInstructionSet(std::string_view processor_name);

    std::span<const std::byte> assemble(std::string_view assembly_instruction);
protected:
    InstructionSet m_instruction_set;
};