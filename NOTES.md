# NOTES

This file contains notes about the project.

## Tasks

---

Implement a simple assembler.

Q: How is this going to work?
A: General Algorithm is:
       1. Expect input line to be tab separated with mnemonic first and instruction operand second
       2. Get the list of Instructions that contain the mnemonic (first field)
       3. For each Instruction, get the parameter that the mnemonic is used with (via Instruction's MODE)
       4. Use the Parameter's DISPLAY column value to match against the input operand's string
       5. If it does match, the operand's value will be represented by the string "%p" and the
          number will be extracted and used to build the final instruction.  The size of %p will depend
          on the byte size of the operand.
  
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
A: Look for a matching mnemonic, which gives you a set of possible instruction matches.
   Look at each instruction and find its matching parameter (via MODE).  Use the DISPLAY
   to try a string match against the operand.  If you find a match, that's your
   Instruction/Parameter pair.  You now have enough data to assemble the complete instruction.

Q: Is this going to be a separate module on its own?
A: Maybe.  Not important at this point.

Q: What would these classes look like?

Q: What would it be called?
A: Assembler

---

Create unit tests for each machine instruction

Q: Can I use the external Instruction/Parameter files for constructing these?
A: I think so.  The basic idea is to "prove" that disassembling and assembling are
   inverse operations (basically f( g(x) ) == x ).
   I could start with assembling and verify the result and then disassemble that and
   get the original input back out.

   Since I have access to the instruction's mnemonic and mode, it is a simple process
   to then be able to find the Parameter with the same mode and then use the DISPLAY
   column to produce the final disassembled output.  This is how disassembly is currently
   defined anyway, so we don't have to really do anything here.
   
   Assembly is a little more tedious.  We find all the instructions