# Assembler

## Description
This repository contains a university course project that implements an assembler program. The assembler translates code written in a defined assembly language into machine code. It processes the input file, detects syntax and semantic errors, and generates an output file compatible with a simulated computer.

### Features
- **Input Handling**: Reads assembly language source files.
- **Error Detection**: Identifies syntax and semantic errors in the input files.
- **Output Generation**: Produces machine code files in a format compatible with the simulated hardware.

### Assembly Language
The assembly language used in this project is tailored to a simulated computer model. It includes:
- Registers (r0 to r7)
- Memory management
- A set of predefined instructions
- Support for basic arithmetic, branching, and memory operations

## Usage
Compile the assembler using the `MakeFile`:
```
make
```

Run the assembler with an input assembly file:
```
./assembler input.asm
```

## Output Files
The assembler generates three types of output files for valid assembly programs:
### 1. Object File (input.ob)
The object file contains the complete memory image, including both the code segment and the data segment. The file format is as follows:
- First Line: A header line with two decimal values, separated by a space:
  - The total length of the code segment (in memory words).
  - The total length of the data segment (in memory words).
- Following Lines: Each line represents a memory word and contains:
  - The address of the memory word (in decimal, 4 digits, including leading zeros).
  - The content of the memory word (in octal, 5 digits).
  - There is a single space between the address and the content.
 
### 2. Entries File (*.ent)
The entries file contains all the symbols defined as `.entry` in the assembly program. The format is:
- One line per symbol.
- Each line contains:
  - The symbol name.
  - The address of the symbol (in decimal).
  - A single space separates the symbol name and the address.
- The order of the lines is not significant.

### 3. . Externals File (*.ext)
The externals file contains the usage of symbols defined as `.external` in the assembly program. The format is:
- One line per external reference.
- Each line contains:
  - The symbol name.
  - The address in the code segment where the external symbol is referenced (in decimal).
  - A single space separates the symbol name and the address.
- Multiple lines may exist for the same external symbol if it is referenced at multiple locations.
- The order of the lines is not significant.

## Examples
1. Failed to open a file
![Example Image](Examples/BAD/cant_open_file.jpg)
   
3. Failed


