# Processor Specification Design

This document outlines the overall design of how to specify the
specifics of a processor.  This will include specifying the registers
along with their size, how many clock cycles a particular instruction
takes in executing, and even breaking down what happens during a specific
clock cycle.

## High-Level Design

We will specify:
* Each register and its properties
* Number of clock cycles per instruction
* Ability to break the instruction execution down into individual clock cycles and what happens
* A mini-language (sort-of a microcode) to represent how the data flows per clock cycle
* What registers the machine code assumes to exist

### Specifying Registers

There will be a way to specify each register, along with its size.

### Specifying Clock Cycles per Instruction

There will be a way to specify the number of clock cycles an instruction takes.

#### Specifying Micro-Ops for each Clock Cycle

You will be able to use a small language to specify how the data moves around during the
execution of an instruction.  You will also be able to specify what happens during a particular
clock cycle using the small landuage, as well as give a name to the particular cycle within the
overall execution steps.

### A Language For Micro-Ops

We will define a small and simple language for specifying how data is transferred within the
processor.  This will include a way to both pull data in from memory as well as push data
out to memory as well as update the state of the various registers.
