#include "Conversions.hpp"
#include <cstdlib>
#include <cassert>

namespace DecimalString
{

void EmptyStringReturnsOptionalWithNoValue()
{
    assert( !DecimalStringToSizeT( std::string_view() ).has_value() );
}

void WithOnlyLettersReturnsOptionalWithNoValue()
{
    assert( !DecimalStringToSizeT( "Abcdefg" ).has_value() );
    assert( !DecimalStringToSizeT( "ABC" ).has_value() );
    assert( !DecimalStringToSizeT( "abc" ).has_value() );

    // Try to use hexadecimal chars
    assert( !DecimalStringToSizeT( "a" ).has_value() );
    assert( !DecimalStringToSizeT( "b" ).has_value() );
    assert( !DecimalStringToSizeT( "c" ).has_value() );
    assert( !DecimalStringToSizeT( "d" ).has_value() );
    assert( !DecimalStringToSizeT( "e" ).has_value() );
    assert( !DecimalStringToSizeT( "f" ).has_value() );
    assert( !DecimalStringToSizeT( "g" ).has_value() );
}

void SpacesProduceNoValue()
{
    assert( !DecimalStringToSizeT( " " ).has_value() );
    assert( !DecimalStringToSizeT( "\t" ).has_value() );
    assert( !DecimalStringToSizeT( "\n" ).has_value() );
    assert( !DecimalStringToSizeT( "\r" ).has_value() );
    assert( !DecimalStringToSizeT( " \t\n\r" ).has_value() );
}

void NegativeNumbersProduceNoValue()
{
    assert( !DecimalStringToSizeT( "-1" ).has_value() );
    assert( !DecimalStringToSizeT( "-99" ).has_value() );
}

void PositiveNumbersProduceGivenValue()
{
    assert( DecimalStringToSizeT( "1" ).has_value() );
    assert( DecimalStringToSizeT( "1" ).value() == 1 );

    assert( DecimalStringToSizeT( "99" ).has_value() );
    assert( DecimalStringToSizeT( "99" ).value() == 99 );

    assert( DecimalStringToSizeT( "1000" ).has_value() );
    assert( DecimalStringToSizeT( "1000" ).value() == 1000 );
}

void CommasInNumberProduceValueBeforeComma()
{
    assert( DecimalStringToSizeT( "1,000" ).has_value() );
    assert( DecimalStringToSizeT( "1,000" ).value() == 1 );

    assert( DecimalStringToSizeT( "1,000,999" ).has_value() );
    assert( DecimalStringToSizeT( "1,000,999" ).value() == 1 );

    assert( DecimalStringToSizeT( "33,000" ).has_value() );
    assert( DecimalStringToSizeT( "33,000" ).value() == 33 );

    // Except for negative numbers...
    assert( !DecimalStringToSizeT( "-33,000" ).has_value() );
}

}

namespace HexString
{

void EmptyStringReturnsOptionalWithNoValue()
{
    assert( !HexStringToSizeT( std::string_view() ).has_value() );
}

void WithOnlyNonHexLettersReturnsOptionalWithNoValue()
{
    assert( !HexStringToSizeT( "ghijklmnopqrstuv" ).has_value() );
    assert( !HexStringToSizeT( "GHI" ).has_value() );
    assert( !HexStringToSizeT( "JKL" ).has_value() );

    // Try to use hexadecimal chars
    assert( !HexStringToSizeT( "z" ).has_value() );
    assert( !HexStringToSizeT( "x" ).has_value() );
    assert( !HexStringToSizeT( "u" ).has_value() );
    assert( !HexStringToSizeT( "y" ).has_value() );
}

void SpacesProduceNoValue()
{
    assert( !HexStringToSizeT( " " ).has_value() );
    assert( !HexStringToSizeT( "\t" ).has_value() );
    assert( !HexStringToSizeT( "\n" ).has_value() );
    assert( !HexStringToSizeT( "\r" ).has_value() );
    assert( !HexStringToSizeT( " \t\n\r" ).has_value() );
}

void NegativeNumbersProduceNoValue()
{
    assert( !HexStringToSizeT( "-1" ).has_value() );
    assert( !HexStringToSizeT( "-99" ).has_value() );
    assert( !HexStringToSizeT( "-AA" ).has_value() );
}

void PositiveNumbersProduceGivenValue()
{
    assert( HexStringToSizeT( "1" ).has_value() );
    assert( HexStringToSizeT( "1" ).value() == 1 );

    assert( HexStringToSizeT( "99" ).has_value() );
    assert( HexStringToSizeT( "99" ).value() == 9 * 16 + 9 );

    assert( HexStringToSizeT( "1000" ).has_value() );
    assert( HexStringToSizeT( "1000" ).value() == 4096 );

    assert( HexStringToSizeT( "A" ).has_value() );
    assert( HexStringToSizeT( "A" ).value() == 10 );

    assert( HexStringToSizeT( "a" ).has_value() );
    assert( HexStringToSizeT( "a" ).value() == 10 );

    assert( HexStringToSizeT( "FF" ).has_value() );
    assert( HexStringToSizeT( "FF" ).value() == 255 );
}

void CommasInNumberProduceValueBeforeComma()
{
    assert( HexStringToSizeT( "1,000" ).has_value() );
    assert( HexStringToSizeT( "1,000" ).value() == 1 );

    assert( HexStringToSizeT( "1,000,999" ).has_value() );
    assert( HexStringToSizeT( "1,000,999" ).value() == 1 );

    assert( HexStringToSizeT( "33,000" ).has_value() );
    assert( HexStringToSizeT( "33,000" ).value() == 3 * 16 + 3 );

    assert( HexStringToSizeT( "FF,000" ).has_value() );
    assert( HexStringToSizeT( "FF,000" ).value() == 255 );

    // Except for negative numbers...
    assert( !HexStringToSizeT( "-FF,000" ).has_value() );
}

}

void DecimalStringToSizeT()
{
    DecimalString::EmptyStringReturnsOptionalWithNoValue();
    DecimalString::WithOnlyLettersReturnsOptionalWithNoValue();
    DecimalString::SpacesProduceNoValue();
    DecimalString::NegativeNumbersProduceNoValue();
    DecimalString::PositiveNumbersProduceGivenValue();
    DecimalString::CommasInNumberProduceValueBeforeComma();
}

void HexStringToSizeT()
{
    HexString::EmptyStringReturnsOptionalWithNoValue();
    HexString::WithOnlyNonHexLettersReturnsOptionalWithNoValue();
    HexString::SpacesProduceNoValue();
    HexString::NegativeNumbersProduceNoValue();
    HexString::PositiveNumbersProduceGivenValue();
    HexString::CommasInNumberProduceValueBeforeComma();
}

int main()
{
    DecimalStringToSizeT();
    HexStringToSizeT();
    return EXIT_SUCCESS;
}