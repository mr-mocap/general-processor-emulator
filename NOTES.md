# NOTES

This file contains notes about the project.

## Tasks

---

Implement a simple assembler.

Q: How is this going to work?
A: General Algorithm is:
       1. Check if there is a mnemonic available
       2. If so, get a list of parameters that the mnemonic is used with
       3. Iterate over each parameter and use the "DISPLAY" column's value
          to match that string with the input line's parameter position and
          see if it matches
       4. If it does, the number will be extracted and used to build the final
          instruction
  
  Base it on a 6502 assembly syntax initially.
   

Q: What are some design goals?
A: -- This should leverage the ideas used in the disassemble method of InstructionSet
      & be able to use the processor's Instruction and Parameter files.
   -- Should be completely driven by external data files (like disassemble does)
   -- Each line to be parsed will contain **only** the opcode and operand(s).
      Any other line item, such as a label, should be handled before the assembly process.
   -- The %p (instruction operand... or parameter) will represent an integer, the size
      of which is defined by the instruction mode's size in bytes.
   -- Numbers are in hexadecimal currently ("$" prefix), but will eventually need to
      support other bases.  Binary comes to mind ("%" prefix), with decimal having no prefix.

Q: How would I know when I'm done?
A: When I can successfully assemble all encoded instructions.

---

Invent a method of using the InstructionSet/Parameter classes for assembling

Q: What would it look like?

Q: Is this going to be a separate module on its own?

Q: What would these classes look like?

Q: What would it be called?

---

