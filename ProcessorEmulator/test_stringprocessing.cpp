#include <cstdlib>
#include <cassert>
#include "StringProcessing.hpp"


void ReadNumberReadsEmptyInput()
{
    ReadNumberResult result = ReadNumber("");

    assert( result.value == -1 );
    assert( result.remaining_input.empty() );
}

void ReadNumberReadsNonNumericInput()
{
    {
        ReadNumberResult result = ReadNumber("abc");

        assert( result.value == -1 );
        assert( result.remaining_input.empty() );
    }
    {
        ReadNumberResult result = ReadNumber("abc123");

        assert( result.value == -1 );
        assert( result.remaining_input.empty() );
    }
}

void ReadNumberReadsValidDecimalInput()
{
    {
        ReadNumberResult result = ReadNumber("123abc");

        assert( result.value == 123 );
        assert( result.remaining_input == "abc" );
    }
    {
        ReadNumberResult result = ReadNumber("0123abc");

        assert( result.value == 123 );
        assert( result.remaining_input == "abc" );
    }
    {
        ReadNumberResult result = ReadNumber("255abc");

        assert( result.value == 255 );
        assert( result.remaining_input == "abc" );
    }
    {
        ReadNumberResult result = ReadNumber("65535abc");

        assert( result.value == 65535 );
        assert( result.remaining_input == "abc" );
    }
}

void ReadNumberReadsValidHexadecimalInput()
{
    {
        ReadNumberResult result = ReadNumber("$7Babc");

        assert( result.value == 0x7BABC );
        assert( result.remaining_input.empty() );
    }
    {
        ReadNumberResult result = ReadNumber("$7babc");

        assert( result.value == 0x7BABC );
        assert( result.remaining_input.empty() );
    }
    {
        ReadNumberResult result = ReadNumber("$7Bhello");

        assert( result.value == 0x7B );
        assert( result.remaining_input == "hello" );
    }
    {
        ReadNumberResult result = ReadNumber("$0123ghi");

        assert( result.value == 0x123 );
        assert( result.remaining_input == "ghi" );
    }
    {
        ReadNumberResult result = ReadNumber("$FF");

        assert( result.value == 0xFF );
        assert( result.remaining_input.empty() );
    }
    {
        ReadNumberResult result = ReadNumber("$ff");

        assert( result.value == 0xFF );
        assert( result.remaining_input.empty() );
    }
    {
        ReadNumberResult result = ReadNumber("$FFFF ");

        assert( result.value == 0xFFFF );
        assert( result.remaining_input == " " );
    }
    {
        ReadNumberResult result = ReadNumber("$ffff ");

        assert( result.value == 0xFFFF );
        assert( result.remaining_input == " " );
    }
}

void ReadNumberReadsValidBinaryInput()
{
    {
        ReadNumberResult result = ReadNumber("%1010abc");

        assert( result.value == 0b1010 );
        assert( result.remaining_input == "abc" );
    }
    {
        ReadNumberResult result = ReadNumber("%11111111def");

        assert( result.value == 0b11111111 );
        assert( result.remaining_input == "def" );
    }
    {
        ReadNumberResult result = ReadNumber("%00000000 ");

        assert( result.value == 0b00000000 );
        assert( result.remaining_input == " " );
    }
}

void ReadNumberReadsValidOctalInput()
{
    {
        ReadNumberResult result = ReadNumber("o123");

        assert( result.value == 0123 );
        assert( result.remaining_input.empty() );
    }
    {
        ReadNumberResult result = ReadNumber("o777def");

        assert( result.value == 0777 );
        assert( result.remaining_input == "def" );
    }
    {
        ReadNumberResult result = ReadNumber("o000 ");

        assert( result.value == 0 );
        assert( result.remaining_input == " " );
    }
}

void SplitStringSplitsAtDelimiter()
{
    {
        std::vector<std::string_view> result = SplitString("abc,def,ghi", ",");

        assert( result.size() == 5 );
        assert( result[0] == "abc" );
        assert( result[1] == "," );
        assert( result[2] == "def" );
        assert( result[3] == "," );
        assert( result[4] == "ghi" );
    }
    {
        std::vector<std::string_view> result = SplitString("abc,def,ghi", ",,");

        assert( result.size() == 1 );
        assert( result[0] == "abc,def,ghi" );
    }
    {
        std::vector<std::string_view> result = SplitString("abc,,def,,ghi", ",,");

        assert( result.size() == 5 );
        assert( result[0] == "abc" );
        assert( result[1] == ",," );
        assert( result[2] == "def" );
        assert( result[3] == ",," );
        assert( result[4] == "ghi" );
    }
    {
        std::vector<std::string_view> result = SplitString("", "%p");

        assert( result.empty() );
    }
}

int main()
{
    ReadNumberReadsEmptyInput();
    ReadNumberReadsNonNumericInput();
    ReadNumberReadsValidDecimalInput();
    ReadNumberReadsValidHexadecimalInput();
    ReadNumberReadsValidBinaryInput();
    ReadNumberReadsValidOctalInput();

    SplitStringSplitsAtDelimiter();

    return EXIT_SUCCESS;
}