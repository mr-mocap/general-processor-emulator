#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>
#include <span>

int MinBytesInRepresentation(std::size_t value);

std::vector<std::uint8_t> OpcodeAsBytes(std::size_t opcode);

std::vector<std::uint8_t> ParameterAsBytes(int parameter_value);