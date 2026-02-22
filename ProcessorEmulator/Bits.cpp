#include "Bits.hpp"
#include <bit>



int MinBytesInRepresentation(std::size_t value)
{
    int  w = std::bit_width(value);

    if ( w == 0 )
        return 1; // We need at least 1 byte to represent the value 0

    bool byte_boundary = (w % 8) == 0;
    int  quotient = w / 8;

    return quotient + (byte_boundary ? 0 : 1);
}

std::vector<std::uint8_t> OpcodeAsBytes(std::size_t opcode)
{
    const std::size_t opcode_array[1] = { opcode };
    std::span<const std::byte> bytes = std::as_bytes( std::span{ opcode_array } );

    std::vector<std::uint8_t> result;

    for ( const std::byte b : bytes.first( MinBytesInRepresentation( opcode ) ) )
        result.push_back( static_cast<std::uint8_t>(b) );

    return result;
}

std::vector<std::uint8_t> ParameterAsBytes(int parameter_value)
{
    const int parameter_array[1] = { parameter_value };
    std::span<const std::byte> bytes = std::as_bytes( std::span{ parameter_array } );

    std::vector<std::uint8_t> result;

    for ( const std::byte b : bytes.first( MinBytesInRepresentation( parameter_value ) ) )
        result.push_back( static_cast<std::uint8_t>(b) );

    return result;
}
