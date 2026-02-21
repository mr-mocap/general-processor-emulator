#pragma "Bits.hpp"
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