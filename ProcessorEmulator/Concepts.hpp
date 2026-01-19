#pragma once

#include <type_traits>

namespace Concept
{

template <typename T>
concept BasicType = std::is_arithmetic_v<T> || std::is_enum_v<T>;

};