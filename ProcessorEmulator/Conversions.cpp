#include "Conversions.hpp"
#include <charconv>


std::optional<size_t> DecimalStringToSizeT(std::string_view input)
{
    size_t retval;

    if ( std::from_chars(input.data(), input.data() + input.size(), retval).ec == std::errc{} )
        return retval;

    return std::nullopt;
}

std::optional<size_t> HexStringToSizeT(std::string_view input)
{
    size_t retval;

    if ( std::from_chars(input.data(), input.data() + input.size(), retval, 16).ec == std::errc{} )
        return retval;

    return std::nullopt;
}

Instruction ToInstruction(const std::vector<std::string> &row_values)
{
    std::optional<size_t> s = HexStringToSizeT(row_values[0]);

    if ( !s.has_value() )
        return {};

    return (row_values.size() == 4) ? Instruction{ s.value(), row_values[1], row_values[2], row_values[3] } :
        Instruction{ s.value(), row_values[1], row_values[2] };
}

Parameter ToParameter(const std::vector<std::string> &row_values)
{
    std::optional<size_t> s = DecimalStringToSizeT(row_values[1]);

    if ( !s.has_value() )
        return {};

    return (row_values.size() == 3) ? Parameter{ s.value(), row_values[0], row_values[2] } :
        Parameter{ s.value(), row_values[0] };
}
