#include "IO.hpp"
#include <cstdlib>
#include <cassert>
#include <sstream>


namespace BreakLine
{

void EmptyInputProducesEmptyVector()
{
    assert( BreakLine( std::string_view() ).empty() );
}

void OneWordProducesVectorWithOneEntry()
{
    {
        std::vector<std::string> result = BreakLine( "word" );

        assert( result.size() == 1);
        assert( result[0] == "word" );
    }

    {
        std::vector<std::string> result = BreakLine( "word " );

        assert( result.size() == 1);
        assert( result[0] == "word" );
    }

    {
        std::vector<std::string> result = BreakLine( " word" );

        assert( result.size() == 1);
        assert( result[0] == "word" );
    }

    {
        std::vector<std::string> result = BreakLine( " word " );

        assert( result.size() == 1);
        assert( result[0] == "word" );
    }

    {
        std::vector<std::string> result = BreakLine( "word\t" );

        assert( result.size() == 1);
        assert( result[0] == "word" );
    }

    {
        std::vector<std::string> result = BreakLine( "\tword" );

        assert( result.size() == 1);
        assert( result[0] == "word" );
    }

    {
        std::vector<std::string> result = BreakLine( "\tword\t" );

        assert( result.size() == 1);
        assert( result[0] == "word" );
    }
}

void Test()
{
    EmptyInputProducesEmptyVector();
    OneWordProducesVectorWithOneEntry();
}

}

namespace ReadLine
{

void OneLineWithOneWordProducesVectorWithOneEntry()
{
    {
        std::string buffer = "ThisIsOneLine";
        std::istringstream input( buffer );
        std::vector<std::string> result = ReadLine( input );

        assert( result.size() == 1 );
        assert( result[0] == buffer );
    }
}

void OneVectorEntryPerLineFromInput()
{
    {
        std::string buffer = "ThisIsOneLine\n";
        std::istringstream input( buffer );
        std::vector<std::string> result = ReadLine( input );

        assert( result.size() == 1 );
        assert( result[0] == buffer );
    }

    {
        std::string buffer = "\nThisIsOneLine";
        std::istringstream input( buffer );
        std::vector<std::string> result = ReadLine( input );

        assert( result.empty() );

        result = ReadLine( input );

        assert( result.size() == 1 );
        assert( result[0] == "ThisIsOneLine" );
    }

    {
        std::string buffer = "ThisIsOneLine\nThisIsAnotherLine";
        std::istringstream input( buffer );
        std::vector<std::string> result = ReadLine( input );

        assert( result.size() == 1 );
        assert( result[0] == "ThisIsOneLine" );

        result = ReadLine( input );

        assert( result.size() == 1 );
        assert( result[0] == "ThisIsAnotherLine" );
    }
}

void ReadingPastEndProducesAnEmptyVector()
{
    std::string buffer = "ThisIsOneLine\n";
    std::istringstream input( buffer );
    std::vector<std::string> result = ReadLine( input );

    assert( result.size() == 1 );
    assert( result[0] == buffer );

    result = ReadLine( input );

    assert( result.empty() );

    result = ReadLine( input );

    assert( result.empty() );
}

void Test()
{
    OneLineWithOneWordProducesVectorWithOneEntry();
    OneVectorEntryPerLineFromInput();
    ReadingPastEndProducesAnEmptyVector();
}

}

int main(void)
{
    BreakLine::Test();
    ReadLine::Test();
    return EXIT_SUCCESS;
}