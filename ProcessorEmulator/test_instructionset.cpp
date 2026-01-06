#include "InstructionSet.hpp"
#include <cstdlib>
#include <cassert>

void DefaultConstructed()
{
    InstructionSet is;

    assert( is.empty() );
}

int main()
{
    DefaultConstructed();
    return EXIT_SUCCESS;
}