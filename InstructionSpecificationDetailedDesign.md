# Instruction Specification Detailed Design
This document outlines the specifics of the Instruction Specification Design.

## Main Idea
We will create a database-style structure for storing the information, per instruction.

It will be laid out in two tables:
* Instruction
* Parameter

### Instruction Table Layout
The table will contain the following columns:
* OPCODE
* MNEMONIC
* MODE
* DISPLAY

The OPCODE will contain a hexadecimal representation of the opcode.

The MNEMONIC will be the assembly-language mnemonic for the instruction.

The MODE will be a user-defined string that must also exist as a row entry in
the Parameter Table as a column with the exact string name (and exact column name of
MODE).

The DISPLAY will be a string that will be used as a fallback to display for the
instruction, should there not be a more spcific one defined in the Parameter Table.

### Parameter Table Layout
The table will contain the following columns:
* MODE
* SIZE
* DISPLAY

The MODE will be a user-defined string that will match against a row entry in the Instruction
Table under the MODE column.

The SIZE will be the count of bytes *after* the opcode for this instruction mode.

The DISPLAY will contain a string that will be displayed for the complete instruction.  Said string
can contain substitution parameters that will be substituted before displaying.

#### DISPLAY String Substitution Patterns
* %p - Represents the bytes of the instruction *after* the opcode

#### Details About the DISPLAY Entry
What to display for the disassembled instruction is first looked up in the Parameter Table's DISPLAY entry and
is displayed if found.  If the entry is empty, then the DISPLAY entry in the Instruction Table is then used.
