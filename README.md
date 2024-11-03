# Simulator-for-a-RISC-V-CPU

## Overview
This project involves designing and implementing a RISC-V single-cycle processor in C/C++. The processor supports a range of instructions, including arithmetic, logical, memory, and control flow operations. The goal is to create a functional processor capable of executing a list of provided instructions.

## Supported Instructions
1. Arithmetic and Logical Operations: ADD, SLL, SLT, SLTU, XOR, SRL, OR, AND, SUB, SRA, ADDI, SLLI, SLTI, SLTIU, XORI, SRLI, SRAI, ORI, ANDI
2. Memory Operations: LW, SW
3. Control Flow Operations: BEQ, BNE, BLT, BGE, BLTU, BGEU, JALR

## Files
1. cpusim.cpp: Entry point of the project. Run the program using ./cpusim <inputfile.txt> and print the final values of registers a0 and a1 in the terminal in the format "(a0,a1)".
2. 23instMem-X files: Binary files containing instructions in unsigned decimal format, stored in little-endian format. Use these files to develop and test your project.
3. 23all.txt, 23sw.txt, 23r-type.txt: Assembly programs corresponding to the instructions in the "23instMem-X" files. Each line includes the memory address (hex), the instruction (hex), and the actual assembly representation.

## Testing
Use the provided test files in the "trace" folder, starting with "r-type.txt" and gradually progressing to "sw.txt" and "all.txt" for comprehensive testing. Debug your code using the correct final values provided in the files.

Deliverables : Code should be compiled with the following command: g++ *.cpp -o cpusim, and then executed with ./cpusim <inputfile.txt>.
