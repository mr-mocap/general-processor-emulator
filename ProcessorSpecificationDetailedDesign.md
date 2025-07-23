# Processor Specification Detailed Design

This document outlines the specific design details of
the Processor Specification.

## Main Idea

There will be a single file that will contain all the data.

The way you specify the registers will look similar to a C-style
struct declaration, with the keyword 'registers' marking the
beginning of the declaration.  Each line inside will contain either
a register name follwed by a ':' and then a bit width for the register.  You may
also define registers that are not to be accessable by normal machine instructions.
These will be denoted by specifying the keyword 'private' followed by a ':',
at which point the rest of any registers declared will be considered private (and
unaccessable to normal instructions) until the end of the registers section.

### Example Registers Specification (for 6502)

    registers
    {
        A : 8
        X : 8
        Y : 8
	private:
        Address : 16
        Data : 8
    }

### Micro-Op Language

There will be a small language for notating how data can flow through the
processor.  The basic idea is that you will use the name(s) of the register(s)
used in the micro-op.  Moving data will be denoted with a '->' character
sequence, with a register being specified on each side of the '->'.  This
will move the data from the register on the left side to the register on the
right side.

Examples:

    A -> X
    X -> Y
    A -> Data

These should be obvious, so I won't go into detailed explanation here.  Representing a value in memory
is denoted by prepending a '*' to the register which contains the memory address.

Examples:

    Data -> *Address
    *Address -> Data

The first line will write the value of the Data register into the address currently contained in the
Address register.  This is a basic memory write.  The second line does the opposite, fetches a value
from memory and stores it in the Data register.
