// ProcessorEmulator.cpp : Defines the entry point for the application.
//

#include "ProcessorEmulator.h"
#include "IO.hpp"

using namespace std;

int main()
{
    InstructionSet instruction_set( ReadParameters(), ReadInstructions() );

    std::byte instruction_test[] = { std::byte{1}, std::byte{32} };

    std::string result = instruction_set.disassemble(instruction_test);

    std::cout << std::format("result = \"{}\"", result) << std::endl;

    return EXIT_SUCCESS;
}
