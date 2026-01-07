#pragma once

#include "InstructionSet.hpp"
#include <vector>
#include <istream>
#include <string>
#include <string_view>
#include <optional>
#include <span>
#include <cstring>
#include <cstddef>
#include <type_traits>


std::vector<Instruction> ReadInstructions();

std::vector<Parameter> ReadParameters();

std::vector<std::string> ReadLine(std::istream &input);

// Splits a string at tabs
std::vector<std::string> BreakLine(std::string_view input);

bool CanMakeInstruction(const std::vector<std::string> &row_values);
bool CanMakeParameter(const std::vector<std::string> &row_values);

Instruction ToInstruction(const std::vector<std::string> &row_values);
Parameter   ToParameter  (const std::vector<std::string> &row_values);

std::optional<size_t> DecimalStringToSizeT(const std::string &input);
std::optional<size_t> HexStringToSizeT(const std::string &input);

template <class BasicType>
static constexpr BasicType To(std::span<const std::byte> bytes) requires(std::is_arithmetic_v<BasicType> || std::is_enum_v<BasicType>)
{
    BasicType result; // We do this for memory alignment of the variable type

    std::memcpy(&result, bytes.data(), sizeof(BasicType));
    return result;
}
