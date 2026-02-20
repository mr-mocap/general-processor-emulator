#pragma once

#include "InstructionSet.hpp"
#include "RegisterSet.hpp"
#include <vector>
#include <istream>
#include <string>
#include <string_view>
#include <optional>
#include <span>
#include <cstring>
#include <cstddef>
#include <type_traits>
#include <filesystem>

std::filesystem::path InstructionSetPath(std::string_view processor_name);
std::filesystem::path ParameterPath(std::string_view processor_name);
std::filesystem::path RegisterPath(std::string_view processor_name);

bool InstructionSetExists(std::string_view processor_name);

std::vector<Instruction> ReadInstructions(std::string_view processor_name);

std::vector<Parameter> ReadParameters(std::string_view processor_name);

std::vector<Register> ReadRegisters(std::string_view processor_name);

InstructionSet ReadInstructionSet(std::string_view processor_name);

std::vector<std::string> TrimRight(std::vector<std::string> &&line);

std::vector<std::string> ReadLine(std::istream &input);

// Splits a string at tabs
std::vector<std::string> BreakLine(std::string_view input);
std::vector<std::string_view> BreakLineView(std::string_view input);

bool CanMakeInstruction(const std::vector<std::string> &row_values);
bool CanMakeParameter(const std::vector<std::string> &row_values);

Instruction ToInstruction(const std::vector<std::string> &row_values);
Parameter   ToParameter  (const std::vector<std::string> &row_values);