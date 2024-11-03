#include <iostream>
#include <bitset>
#include <stdio.h>
#include <stdlib.h>
#include <string>
using namespace std;


class instruction 
{
	public:
		bitset<32> instr;
		instruction(bitset<32> fetch); 

};

// Decode Block
	// Input coming to decode stage - 
		// Store the fetched instruction in a variable for decoding
	// Outputs of decode stage - 
		// rs1 - Source register 1 contents 
		// rs2 - Source register 2 contents
		// imm - Immediate from instruction
		// rd_addr - Destination Register
		// What the instruction has asked? - Basically the operation to be done during execution stage
class Decode 
{
	private:
		uint32_t instr;
		int32_t *reg_file;
		int32_t rs1 = 0;
		int32_t rs2 = 0;
		int32_t imm = 0;
		uint32_t rd_addr = 0;
		string operation; 
		string select_write_back = "ALU";
		// Constants for the types of Instructions
		const bitset<7> R_TYPE = bitset<7>("0110011");
		const bitset<7> I_TYPE = bitset<7>("0010011");
		const bitset<7> LW_TYPE = bitset<7>("0000011");
		const bitset<7> SW_TYPE = bitset<7>("0100011");
		const bitset<7> B_TYPE = bitset<7>("1100011");
		const bitset<7> JALR_TYPE = bitset<7>("1100111"); 
		const bitset<7> ZERO_TYPE = bitset<7>("0000000");
		
	public:
		bool decode_instr();

		// Supporting functions to access some of the private data types
		void set_instr(uint32_t instr_temp);
		void set_reg_file_ptr(int32_t* temp_reg_file);
		int32_t get_rs1();
		int32_t get_rs2();
		int32_t get_imm();
		uint32_t get_rd_addr();
		string get_operation();
		string get_select_write_back();

		//int num_r_type = 0;
};

// Execute Block
	// Inputs coming to Execute stage
		// rs1 - Source register 1 
		// rs2 - Source register 1
		// imm - Immediate from instruction
		// Type of instruction like Type R or Type I or ....
		// Type of Operation in ALU based on funct2 funct7
		// rd_addr - To pass to next stage
	// Output of Execute Stage
		// ALU Result
		// Might need more outputs for Jump or Conditional branch statements - Will come back to this		

class Execute
{
	private:
		unsigned long* PC_ptr;
		int32_t rs1 = 0;
		int32_t rs2 = 0;
		int32_t imm = 0;
		uint32_t rd_addr = 0;
		int32_t alu_result = 0;
		string operation; 
		string select_write_back;
	public:
		void execute_instr();

		// Supporting functions to set or get some of the private members of Execute Block
		void set_rs1(int32_t temp_rs1);
		void set_rs2(int32_t temp_rs2);
		void set_rd_addr(uint32_t temp_rd_addr);
		void set_imm(int32_t temp_imm);
		void set_operation(string temp_operation);
		void set_pc_ptr(unsigned long* temp_PC_ptr);
		void set_select_write_back(string temp_select_write_back);

		int32_t get_rs2();
		int32_t get_alu_result();
		uint32_t get_rd_addr();
		string get_operation();
		string get_select_write_back();
};

// Memory Block
	// Input
		// rs2 - Used for getting data that is written in memory
		// ALU Result from execute stage - Either to send to next stage or for addressing the memory
		// rd - Keep destination regsiter here to move to the next stage
		// Type of operation - Load or Store 
	// Output 
		// read_data - Data read from Memory

class MemStage
{
	private:
		int32_t rs2 = 0;
		uint32_t rd_addr = 0;
		int32_t alu_result = 0;
		uint32_t read_data = 0;;
		bitset<8>* dmemory;
		string operation;
		string select_write_back;
	public:
		void memory_instr();

		// Supporting functions for MemStage
		void set_dmem_ptr(bitset<8>* temp_dmemory);
		void set_rs2(int32_t temp_rs2);
		void set_alu_result(int32_t temp_alu_result);
		void set_rd_addr(uint32_t temp_rd_addr);
		void set_operation(string temp_operation);
		void set_select_write_back(string temp_select_write_back);

		uint32_t get_rd_addr();
		int32_t get_alu_result();
		int32_t get_read_data();
		string get_operation();
		string get_select_write_back();

};

// Write Back Block
	// Input
		// rd - To write either ALU result or data read from memory into the Destination Register
		// ALU Result from previous stage if need to WB to destination register
		// read_data - Data read from memory
		// Type of operation - To determine whether to write back ALU result or memory read data.

class WriteBack
{
	private:
		uint32_t rd_addr = 0;
		int32_t alu_result = 0;
		int32_t read_data = 0;
		int32_t *reg_file;
		string operation;
		string select_write_back;
	public:
		void wb_instr();

		// Supporting functions to set the private data types
		void set_rd_addr(uint32_t temp_rd_addr);
		void set_alu_result(int32_t temp_alu_result);
		void set_read_data(int32_t temp_read_data);
		void set_operation(string temp_operation);
		void set_select_write_back(string temp_select_write_back);
		void set_reg_file_ptr(int32_t* temp_reg_file);
};

class CPU
{
	private:
		bitset<8> dmemory[4096]; 	//data memory byte addressable in little endian fashion;
		unsigned long PC; 	// PC 
	
		// 32x32-Bit Register File - Common for all stages
		int32_t reg_file[32]; 
	
	public:
		CPU();
	
		// Directly to retrieve PC+4 since during fetch PC is incremented immediatly
		unsigned long readPC();
		unsigned long* get_pc_ptr();
		
		// Fetch Block - Just need a simple function not an independent class.
			// When want to make pipelined then need class for fetch stage also to pass data from fetch data to decode stage

		bitset<32> fetch(bitset<8> *instmem);	
		
		// Declaring the various stages objects
		Decode decode_block;
		Execute execute_block;
		MemStage mem_stage;
		WriteBack wb_block;

		// Data Transfer Function between stages
		void fetch2decode(instruction *curr, Decode *decode_block); 	// Fetch not independent part of CPU as just single func
		void decode2execute(Decode *decode_block, Execute *execute_block);
		void execute2memory(Execute *execute_block, MemStage *mem_stage);
		void memory2wb(MemStage *mem_stage, WriteBack *wb_block);

		// Getting final results
		void get_results();


};	


	

