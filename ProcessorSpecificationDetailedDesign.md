# Processor Specification Detailed Design

This document outlines the specific design details of
the Processor Specification.

## Main Idea

There will be a single file that will contain all the data.

The way you specify the registers will look similar to a C-style
struct declaration, with the keyword 'registers' marking the
beginning of the declaration.  Each line inside will contain either
a register name follwed by a ':' and then a bit width for the register.

### Example Registers Specification (for 6502)

    registers
    {
        A : 8
        X : 8
        Y : 8
        Address : 16
        Data : 8
        ProgramCounter : 16
        Status : 8
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

#### Arithmetic Operations

There will be various arithmetic operations for updating a register.  We will define
the following:

* Increment
* Decrement
* Addition

#### Logical Operations

There will be various logical operations for updating a register.  We will define
the following:

* One's Complement (Bitwise invert)
* Set Bit
* Clear Bit
* Equality
* Inequality
* Bitwise AND
* Bitwise OR
* Shift Left
* Shift Right

#### Secifying Sizes

There will be a way to access particular groups of bits of various lengths, going
all the way down to a single bit.  We will focus only on groupings in powers
of two (1, 2, 4, 8, 16, ...).

#### Calling a Function

There will be a way to call a functions and use its results.

#### Built-In Functions

There will be various functions available to perform various computations.

* Specifying Which Bits in a Value to Refer To
    * choose( register_name, number_of_bits, which_group )
* One's Complement
    * flip( bitset )
* Set Bits
    * set( bitset )
* Clear Bits
    * clear( bitset )
* AND Bits
    * and( bitset_left, bitset_right )
* OR Bits
    * or( bitset_left, bitset_right )
* Equals
    * equals( bitset_left, bitset_right )
* Not Equals
    * not_equals( bitset_left, bitset_right )
* Shift Left
    * left_shift( bitset )
* Shift Right
    * right_shift( bitset )
* Increment
    * increment( bitset )
* Decrement
    * decrement( bitset )
* Add
    * add( bitset_left, bitset_right )
* Memory Read
    * memory_read( memory_address, value )
* Memory Write
    * memory_write( memory_address, value )
* Copy Bits
    * copy_to( origin_bitset, destination_bitset )
