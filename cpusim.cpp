#include "CPU.h"

#include <iostream>
#include <bitset>
#include <stdio.h>
#include<stdlib.h>
#include <string>
#include<fstream>
#include <sstream>
using namespace std;

/*
Add all the required standard and developed libraries here
*/

/*
Put/Define any helper function/definitions you need here
*/
int main(int argc, char* argv[])
{
	/* 
	This is the front end of your project.
	You need to first read the instructions that are stored in a file and load them into an instruction memory.
	*/

	/* 
	Each cell should store 1 byte. You can define the memory either dynamically, or define it as a fixed size with size 4KB (i.e., 4096 lines). Each instruction is 32 bits (i.e., 4 lines, saved in little-endian mode).
	Each line in the input file is stored as an unsigned char and is 1 byte (each four lines are one instruction). You need to read the file line by line and store it into the memory. You may need a mechanism to convert these values to bits so that you can read opcodes, operands, etc.
	*/

	bitset<8> instMem[4096];


	if (argc < 2) {
		cout << "No file name entered. Exiting...";
		return -1;
	}

	ifstream infile(argv[1]); //open the file
	if (!(infile.is_open() && infile.good())) {
		cout<<"Error opening file\n";
		return 0; 
	}
	string line; 
	int i = 0;
	while (infile) {
			infile>>line;
			stringstream line2(line);
			int x; 
			line2>>x;
			instMem[i] = bitset<8>(x);
			i++;
		}
	int maxPC= i; 

	/* 
	Instantiate your CPU object here.  CPU class is the main class in this project that defines different components of the processor.
	CPU class also has different functions for each stage (e.g., fetching an instruction, decoding, etc.).
	*/

	CPU myCPU;

	// make sure to create a variable for PC and resets it to zero (e.g., unsigned int PC = 0); 

	/* OPTIONAL: Instantiate your Instruction object here. */
	//Instruction myInst; 
	bitset<32> curr;
	instruction instr = instruction(curr);

	bool done = true;
	while (done == true) // processor's main loop. Each iteration is equal to one clock cycle.  
	{
		// Fetch
		curr = myCPU.fetch(instMem); // fetching the instruction - For arrays when passing into pointer may or may not convert to address
		instr = instruction(curr);

		// Decode
		myCPU.fetch2decode(&instr, &(myCPU.decode_block));
		done = myCPU.decode_block.decode_instr();
		if (done ==false) // break from loop so stats are not mistakenly updated
			break;

		// Execute
		myCPU.decode2execute(&(myCPU.decode_block), &(myCPU.execute_block));
		myCPU.execute_block.execute_instr();

		// Memory
		myCPU.execute2memory(&(myCPU.execute_block), &(myCPU.mem_stage));
		myCPU.mem_stage.memory_instr();

		// WriteBack
		myCPU.memory2wb(&(myCPU.mem_stage), &(myCPU.wb_block));
		myCPU.wb_block.wb_instr();

		// To check a0,a1 after every instruction - To debug
		myCPU.get_results();

		// Sanity Check
		if (myCPU.readPC() > maxPC)
			break;
	}

	//myCPU.get_results();

	return 0;

}