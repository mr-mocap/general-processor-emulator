#include "IO.hpp"
#include <cstdlib>
#include <cassert>
#include <sstream>


namespace BreakLineTests
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
        assert( result[0] == "word " );
    }

    {
        std::vector<std::string> result = BreakLine( " word" );

        assert( result.size() == 1);
        assert( result[0] == " word" );
    }

    {
        std::vector<std::string> result = BreakLine( " word " );

        assert( result.size() == 1);
        assert( result[0] == " word " );
    }

    {
        std::vector<std::string> result = BreakLine( "word\t" );

        assert( result.size() == 1);
        assert( result[0] == "word" );
    }

    {
        std::vector<std::string> result = BreakLine( "\tword" );

        assert( result.size() == 2);
        assert( result[0].empty() );
        assert( result[1] == "word" );
    }

    // 
    {
        std::vector<std::string> result = BreakLine( "\tword\t" );

        assert( result.size() == 2);
        assert( result[0].empty() );
        assert( result[1] == "word" );
    }
}

void TrimsFinalTabs()
{
    {
        std::vector<std::string> result = BreakLine( "word\t" );

        assert( result.size() == 1);
        assert( result[1] == "word" );
    }

    {
        std::vector<std::string> result = BreakLine( "word\t\t" );

        assert( result.size() == 1);
        assert( result[1] == "word" );
    }
}

void OneVectorEntryPerWordOfInput()
{
    {
        std::vector<std::string> result = BreakLine( "word\tword2\t1234\tabc123\t1,000" );

        assert( result.size() == 5);
        assert( result[0] == "word" );
        assert( result[1] == "word2" );
        assert( result[2] == "1234" );
        assert( result[3] == "abc123" );
        assert( result[4] == "1,000" );
    }
    {
        std::vector<std::string> result = BreakLine( "word\t\tword2" );

        assert( result.size() == 3);
        assert( result[0] == "word" );
        assert( result[1].empty() );
        assert( result[2] == "word2" );
    }
}

void Run()
{
    EmptyInputProducesEmptyVector();
    OneWordProducesVectorWithOneEntry();
    OneVectorEntryPerWordOfInput();
}

}

namespace ReadLineTests
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
        assert( result[0] == "ThisIsOneLine" );
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
    assert( result[0] == "ThisIsOneLine" );

    result = ReadLine( input );

    assert( result.empty() );

    result = ReadLine( input );

    assert( result.empty() );
}

void Run()
{
    OneLineWithOneWordProducesVectorWithOneEntry();
    OneVectorEntryPerLineFromInput();
    ReadingPastEndProducesAnEmptyVector();
}

}

int main(void)
{
    BreakLineTests::Run();
    ReadLineTests::Run();
    return EXIT_SUCCESS;
}