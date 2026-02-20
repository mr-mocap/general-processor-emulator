#pragma once

#include "Concepts.hpp"
#include "InstructionSet.hpp"
#include "RegisterSet.hpp"
#include <string>
#include <string_view>
#include <cstring>
#include <optional>
#include <span>
#include <cstddef>
#include <vector>
#include <cassert>


Parameter   ToParameter(const std::vector<std::string> &row_values);

Instruction ToInstruction(const std::vector<std::string> &row_values);

Register    ToRegister(const std::vector<std::string> &row_values);

std::optional<size_t> DecimalStringToSizeT(std::string_view input);
std::optional<size_t> HexStringToSizeT(std::string_view input);

template <Concept::BasicType T>
constexpr T To(std::span<const std::byte> bytes)
{
    assert( bytes.size() == sizeof(T) );

    T result; // We do this for memory alignment of the variable type

    std::memcpy(&result, bytes.data(), sizeof(T));
    return result;
}