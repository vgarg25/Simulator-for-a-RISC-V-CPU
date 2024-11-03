#include "CPU.h"

// Suuporting functions for decode block - Accessing the private data members
void Decode::set_instr(uint32_t instr_temp)
{
	instr = instr_temp;
}

int32_t Decode::get_rs1()
{
	return rs1;
}

int32_t Decode::get_rs2()
{
	return rs2;
}

int32_t Decode::get_imm()
{
	return imm;
}

uint32_t Decode::get_rd_addr()
{
	return rd_addr;
}

string Decode::get_operation()
{
	return operation;
}
string Decode::get_select_write_back()
{
	return select_write_back;
}
void Decode::set_reg_file_ptr(int32_t* temp_reg_file)
{
	reg_file = temp_reg_file;
}
// Supporting functions of Execute Block
void Execute::set_rs1(int32_t temp_rs1)
{
	rs1 = temp_rs1;
}
void Execute::set_rs2(int32_t temp_rs2)
{
	rs2 = temp_rs2;
}
void Execute::set_rd_addr(uint32_t temp_rd_addr)
{
	rd_addr = temp_rd_addr;
}
void Execute::set_imm(int32_t temp_imm)
{
	imm = temp_imm;
}
void Execute::set_operation(string temp_operation)
{
	operation = temp_operation;
}
void Execute::set_pc_ptr(unsigned long* temp_PC_ptr)
{
	PC_ptr = temp_PC_ptr;
}
void Execute::set_select_write_back(string temp_select_write_back)
{
	select_write_back = temp_select_write_back;
}
int32_t Execute::get_rs2()
{
	return rs2;
}
int32_t Execute::get_alu_result()
{
	return alu_result;
}
uint32_t Execute::get_rd_addr()
{
	return rd_addr;
}
string Execute::get_operation()
{
	return operation;
}
string Execute::get_select_write_back()
{
	return select_write_back;
}

// Supporting functions for MemStage
void MemStage::set_rs2(int32_t temp_rs2)
{
	rs2 = temp_rs2;
}
void MemStage::set_alu_result(int32_t temp_alu_result)
{
	alu_result = temp_alu_result;
}
void MemStage::set_rd_addr(uint32_t temp_rd_addr)
{
	rd_addr = temp_rd_addr;
}
void MemStage::set_operation(string temp_operation)
{
	operation = temp_operation;
}
void MemStage::set_select_write_back(string temp_select_write_back)
{
	select_write_back = temp_select_write_back;
}
void MemStage::set_dmem_ptr(bitset<8>* temp_dmemory)
{
	dmemory = temp_dmemory;
}
uint32_t MemStage::get_rd_addr()
{
	return rd_addr;
}
int32_t MemStage::get_alu_result()
{
	return alu_result;
}
int32_t MemStage::get_read_data()
{
	return read_data;
}
string MemStage::get_operation()
{
	return operation;
}
string MemStage::get_select_write_back()
{
	return select_write_back;
}

// Supporting functions for WB Stage
void WriteBack::set_rd_addr(uint32_t temp_rd_addr)
{
	rd_addr = temp_rd_addr;	
}
void WriteBack::set_alu_result(int32_t temp_alu_result)
{
	alu_result = temp_alu_result;
}
void WriteBack::set_read_data(int32_t temp_read_data)
{
	read_data = temp_read_data;
}
void WriteBack::set_operation(string temp_operation)
{
	operation = temp_operation;
}
void WriteBack::set_select_write_back(string temp_select_write_back)
{
	select_write_back = temp_select_write_back;
}
void WriteBack::set_reg_file_ptr(int32_t* temp_reg_file)
{
	reg_file = temp_reg_file;
}

instruction::instruction(bitset<32> fetch)
{
	//cout << fetch << endl;
	instr = fetch;
	//cout << instr << endl;
}

CPU::CPU()
{
	PC = 0; //set PC to 0
	for (int i = 0; i < 4096; i++) //copy instrMEM
	{
		dmemory[i] = 0;
	}

	for (int j = 0; j < 32; j++)
	{
		reg_file[j] = 0;
	}
}

unsigned long CPU::readPC()
{
	return PC;
}

unsigned long* CPU::get_pc_ptr()
{
	return &PC;
}

bitset<32> CPU::fetch(bitset<8> *instmem) 
{
	// From four consecutive 8-bit locations retrieve the full instruction
	bitset<32> instr = ((((instmem[PC + 3].to_ulong()) << 24)) + ((instmem[PC + 2].to_ulong()) << 16) + ((instmem[PC + 1].to_ulong()) << 8) + (instmem[PC + 0].to_ulong()));
	// Increment PC to next address location
	PC += 4;
	return instr;
}

void CPU::fetch2decode(instruction *curr ,Decode *decode_block)
{
	// Getting the 32-bit instruction - Type convert to uint32_t for easy parsing
	decode_block->set_instr((uint32_t)(curr->instr.to_ulong()));
	decode_block->set_reg_file_ptr(reg_file);
	// Transfer PC pointer to decode stage - If pipeline we can make it just PC
	// Not needed in single cycle hence not added - To add for pipelined remove PC pointer and add get and set functions for a PC in Decode Block
}

bool Decode::decode_instr()
{	
	// Retreiving opcode from the instruction - Bits 6 to 0
	bitset<7> opcode = bitset<7>(instr & 0x7F);

	select_write_back = "ALU";

	// R Type Instructions
	if (opcode == R_TYPE)
	{
		// num_r_type++;
		// Parsing parts of Instruction for R Type Instruction
		rd_addr = (instr >> 7) & 0x1F;	// Bits 11 to 7 
		uint32_t funct3 = (instr >> 12) & 0x7; 		// Bits 14 to 12 
		uint32_t funct7 = (instr >> 25) & 0x7F;		// Bits 31 to 25
		uint32_t rs1_addr = (instr >> 15) & 0x1F;		// Bits 15 to 19 	
		uint32_t rs2_addr = (instr >> 20) & 0x1F;		// Bits 20 to 24
	
		rs1 = reg_file[rs1_addr];
		rs2 = reg_file[rs2_addr];        

		
		if (funct7 == 0x00)
		{
			if (funct3 == 0x0) {operation = "ADD";}
			else if (funct3 == 0x1) {operation = "SLL";}
			else if (funct3 == 0x2) {operation = "SLT";}
			else if (funct3 == 0x3) {operation = "SLTU";}
			else if (funct3 == 0x4) {operation = "XOR";}
			else if (funct3 == 0x5) {operation = "SRL";}
			else if (funct3 == 0x6) {operation = "OR";}
			else if (funct3 == 0x7) {operation = "AND";}
			else 
			{
				// TODO Something for invalid entry
			}

		}

		// For SUB & SRA Instructions
		else if (funct7 == 0x20)
		{	
			if (funct3 == 0x0) {operation = "SUB";}
			if (funct3 == 0x5) {operation = "SRA";}

		}

		else 
		{ 
			// TODO Something for invalid entry 
		}


	}

	// I Type Instructions
	else if (opcode == I_TYPE)
	{

		// Parsing parts of Instruction for I Type Instruction
		rd_addr = (instr >> 7) & 0x1F;	// Bits 11 to 7 
		uint32_t funct3 = (instr >> 12) & 0x7; 		// Bits 14 to 12 
		uint32_t funct7 = (instr >> 25) & 0x7F;		// Bits 31 to 25
		uint32_t rs1_addr = (instr >> 15) & 0x1F;	// Bits 15 to 19 
		imm = static_cast<int32_t>(instr >> 20);	// Bits 31 to 20 for immediate operand

		// Sign Extension for the immediate operand
		if (imm >> 11 == 1)
		{
			imm = imm | 0xFFFFF000;
		}
		
		rs1 = reg_file[rs1_addr];
		rs2 = (instr >> 20) & 0x1F;		// Bits 20 to 24 - Stores the shift amount

		if (funct3 == 0x0) {operation = "ADDI";}

		// For SLLI instruction
		else if ((funct3 == 0x1) && (funct7 == 0x00)) 
		{
			operation = "SLLI";
		}
		else if (funct3 == 0x2) {operation = "SLTI";}
		else if (funct3 == 0x3) {operation = "SLTIU";}
		else if (funct3 == 0x4) {operation = "XORI";}

		// For SRLI & SRAI instructions
		else if (funct3 == 0x5) 
		{
			if (funct7 == 0x00) {operation = "SRLI";}
			else if (funct7 == 0x20) {operation = "SRAI";}
			else 
			{
				// TODO Something for invalid entry  
			}
		}
		else if (funct3 == 0x6) {operation = "ORI";}
		else if (funct3 == 0x7) {operation = "ANDI";}
		else 
		{
			select_write_back = "NA";
			// TODO Something for invalid entry
		}
	}

	// Branch B Type Instructions
	else if (opcode == B_TYPE)
	{
		select_write_back = "NA";	// In branch instructions nothing to be written back

		// Parsing parts of Instruction for B Type Instruction
		uint32_t funct3 = (instr >> 12) & 0x7; 		// Bits 14 to 12 
		uint32_t rs1_addr = (instr >> 15) & 0x1F;		// Bits 15 to 19 	
		uint32_t rs2_addr = (instr >> 20) & 0x1F;		// Bits 20 to 24
		
		rs1 = reg_file[rs1_addr];
		rs2 = reg_file[rs2_addr];   

		// No 0th bit reqd for branch instructions
		int32_t imm12 = ((instr >> 31) & 0x1); 
		imm = 
			imm12 << 12 | 						// imm[12]
			((instr >> 25) & 0x3F) << 5 | 		// imm[10:5]
			((instr >> 8) & 0xF) << 1 |   		// imm[4:1]
			((instr >> 7) & 0x1) << 11;   		// imm[11]

		if (imm12 == 1)
		{
			imm = imm | 0xFFFFF000;
		}

		if (funct3 == 0x0) {operation = "BEQ";}
		else if (funct3 == 0x1) {operation = "BNE";}
		else if (funct3 == 0x4) {operation = "BLT";}
		else if (funct3 == 0x5) {operation = "BGE";}
		else if (funct3 == 0x6) {operation = "BLTU";}
		else if (funct3 == 0x7) {operation = "BGEU";}
		else 
		{
			// TODO Something for invalid entry
		}
	}

	// LW Type Instructions
	else if (opcode == LW_TYPE)
	{
		select_write_back = "ReadData";

		// Parsing parts of Instruction for LW Type Instruction
		rd_addr = (instr >> 7) & 0x1F;	// Bits 11 to 7 
		uint32_t funct3 = (instr >> 12) & 0x7; 		// Bits 14 to 12 
		uint32_t rs1_addr = (instr >> 15) & 0x1F;	// Bits 15 to 19 
		imm = static_cast<int32_t>(instr >> 20);	// Bits 31 to 20 for immediate operand

		// Sign Extension for the immediate operand
		if (imm >> 11 == 1)
		{
			imm = imm | 0xFFFFF000;
		}

		rs1 = reg_file[rs1_addr];

		if (funct3 == 0x2)
		{
			operation = "LW";
		}
		else
		{
			// TODO Something for invalid entry
		}
	
	}

	// SW Type Instructions
	else if (opcode == SW_TYPE)
	{
		select_write_back = "NA";
		
		// Parsing parts of Instruction for SW Type Instruction
		uint32_t funct3 = (instr >> 12) & 0x7; 		// Bits 14 to 12 
		uint32_t rs1_addr = (instr >> 15) & 0x1F;		// Bits 15 to 19 	
		uint32_t rs2_addr = (instr >> 20) & 0x1F;		// Bits 20 to 24

		rs1 = reg_file[rs1_addr];
		rs2 = reg_file[rs2_addr]; 	

		imm = 
				((instr >> 25) & 0x7F) << 5 | 		// imm[11:5]
       				((instr >> 7) & 0x1F);        		// imm[4:0]

		// Sign Extension for the immediate operand
		if (imm >> 11 == 1)
		{
			imm = imm | 0xFFFFF000;
		}	

		if (funct3 == 0x2)
		{
			//cout<<"SW DBG: Decode"<<endl;
			operation = "SW";	
		}
		else
		{
			
			// TODO Something for invalid entry
		}

	}

	// JALR Type Instructions
	else if (opcode == JALR_TYPE)
	{

		// For JALR write back is ALU result not if other jump instructions implemented

		// Parsing parts of Instruction for JALR Type Instruction
		rd_addr = (instr >> 7) & 0x1F;	// Bits 11 to 7 
		uint32_t funct3 = (instr >> 12) & 0x7; 		// Bits 14 to 12 
		uint32_t rs1_addr = (instr >> 15) & 0x1F;		// Bits 15 to 19 
		imm = static_cast<int32_t>(instr >> 20);	// Bits 31 to 20 for immediate operand

		rs1 = reg_file[rs1_addr];

		// Sign Extension for the immediate operand
		if (imm >> 11 == 1)
		{
			imm = imm | 0xFFFFF000;
		}
		
		if (funct3 == 0x0)
		{
			operation = "JALR";
		}
		else
		{
			// TODO Something for invalid entry
		}	
	}

	// ZERO Type Instructions
	else if (opcode == ZERO_TYPE)
	{
		operation = "ZERO";
		select_write_back = "NA";
	}


	else
	{
		select_write_back = "NA";
		// TODO Something for invalid entry
	}

	return true;
}

void CPU::decode2execute(Decode *decode_block, Execute *execute_block)
{
	execute_block->set_rs1(decode_block->get_rs1());
	execute_block->set_rs2(decode_block->get_rs2());
	execute_block->set_rd_addr(decode_block->get_rd_addr());
	execute_block->set_imm(decode_block->get_imm());
	execute_block->set_operation(decode_block->get_operation());
	execute_block->set_pc_ptr(get_pc_ptr());			// get_pc_ptr is from CPU function
	execute_block->set_select_write_back(decode_block->get_select_write_back());
}

void Execute::execute_instr()
{

	// Type R Instructions

	if (operation == "ADD")
	{
		alu_result = rs1 + rs2;
	}

	else if (operation == "SUB")
	{
		alu_result = rs1 - rs2;
	} 

	else if (operation == "SLL")
	{
		alu_result = rs1 << rs2;
	} 

	else if (operation == "SLT")
	{
		alu_result = (rs1 < rs2) ? 1 : 0;
	} 

	else if (operation == "SLTU")
	{
		alu_result = ((uint32_t)rs1 < (uint32_t)rs2) ? 1 : 0;		
	} 

	else if (operation == "XOR")
	{
		alu_result = rs1 ^ rs2;
	} 

	else if (operation == "SRL")
	{
		alu_result = (uint32_t)rs1 >> rs2;
	} 

	else if (operation == "SRA")
	{
		alu_result = rs1 >> rs2;	
	} 

	else if (operation == "OR")
	{
		alu_result = rs1 | rs2;
	} 

	else if (operation == "AND")
	{
		alu_result = rs1 & rs2;
	} 

	// Type I Instructions

	else if (operation == "ADDI")
	{
		alu_result = rs1 + imm;
	}

	else if (operation == "SLTI")
	{
		alu_result = (rs1 < imm) ? 1 : 0;
	} 

	else if (operation == "SLTIU")
	{
		alu_result = ((uint32_t)rs1 < (uint32_t)imm) ? 1 : 0;
	} 

	else if (operation == "XORI")
	{
		alu_result = rs1 ^ imm;
	} 

	else if (operation == "SLLI")
	{
		alu_result = rs1 << imm;
	} 

	else if (operation == "SRLI")
	{
		alu_result = (uint32_t)rs1 >> imm;
	} 

	else if (operation == "SRAI")
	{
		alu_result = rs1 >> imm;
	} 

	else if (operation == "ORI")
	{
		alu_result = rs1 | imm;
	} 

	else if (operation == "ANDI")
	{
		alu_result = rs1 & imm;
	} 

	// Type B Instructions
		// Add branch condition - branch if condn satisfied
		// Calculate offset
		// If satisfied condn calculate new PC with old PC and offset
		// & with ~1 to make the imm even 

	else if (operation == "BEQ")
	{
		if (rs1 == rs2)
			{
				*PC_ptr = *PC_ptr + (imm & ~3) - 4;
			}
	}

	else if (operation == "BNE")
	{
		if (rs1 != rs2)
			{
				*PC_ptr = *PC_ptr + (imm & ~3) - 4;
			}
	} 

	else if (operation == "BLT")
	{
		if (rs1 < rs2)
			{
				*PC_ptr = *PC_ptr + (imm & ~3) - 4;
			}
	} 

	else if (operation == "BGE")
	{
		if (rs1 >= rs2)
			{
				*PC_ptr = *PC_ptr + (imm & ~3) - 4;
			}
	}

	else if (operation == "BLTU")
	{
		if ((uint32_t)rs1 < (uint32_t)rs2)
			{
				*PC_ptr = *PC_ptr + (imm & ~3) - 4;
			}
	} 

	else if (operation == "BGEU")
	{
		if ((uint32_t)rs1 >= (uint32_t)rs2)
			{
				*PC_ptr = *PC_ptr + (imm & ~3) - 4;
			}
	}  

	// LW Instruction

	else if (operation == "LW")
	{
		alu_result = rs1 + imm; 	// Address Calculation
	}

	// SW Instruction

	else if (operation == "SW")
	{
		//cout<<"SW DBG: Execute"<<endl;
		alu_result = rs1 + imm;		// Address Calculaion
	}

	// JALR Instruction 

	else if (operation == "JALR")
	{
		alu_result = *PC_ptr;		// Store the return address into rd during write back stage
		*PC_ptr = ((rs1 + imm) & ~3);
	} 
	
	// Error/Invalid condition

	else 
	{
		// TODO - Add error/invalid condition
	}
 

}

void CPU::execute2memory(Execute *execute_block, MemStage *mem_stage)
{
	mem_stage->set_rs2(execute_block->get_rs2());
	mem_stage->set_alu_result(execute_block->get_alu_result());
	mem_stage->set_rd_addr(execute_block->get_rd_addr());
	mem_stage->set_operation(execute_block->get_operation());  
	mem_stage->set_select_write_back(execute_block->get_select_write_back());
	mem_stage->set_dmem_ptr(dmemory);
}

void MemStage::memory_instr()
{

	if (operation == "LW")
	{
		read_data = 0;
		for (int i=0; i<4; i++)
		{
		read_data = read_data | (uint32_t)(dmemory[(uint32_t)alu_result + i].to_ulong() << (8*i));
		}
	}

	else if (operation == "SW")
	{
		//cout<<"SW DBG: MemStage : "<<endl<<alu_result<<endl<<rs2<<endl;
		dmemory[(uint32_t)alu_result] = rs2 & 0xFF;
		dmemory[(uint32_t)alu_result + 1] = (rs2 & 0xFF00)>>8;
		dmemory[(uint32_t)alu_result + 2] = (rs2 & 0xFF0000)>>16;
		dmemory[(uint32_t)alu_result + 3] = (rs2 & 0xFF000000)>>24;
		//cout<<"SW DBG: MemStage Complete"<<endl;
	}

}

void CPU::memory2wb(MemStage *mem_stage, WriteBack *wb_block)
{
	wb_block->set_rd_addr(mem_stage->get_rd_addr());
	wb_block->set_alu_result(mem_stage->get_alu_result());
	wb_block->set_read_data(mem_stage->get_read_data());
	wb_block->set_operation(mem_stage->get_operation());
	wb_block->set_select_write_back(mem_stage->get_select_write_back());
	wb_block->set_reg_file_ptr(reg_file);
}

void WriteBack::wb_instr()
{

	if (select_write_back == "ALU")
	{
		reg_file[rd_addr] = alu_result;
	}
	else if (select_write_back == "ReadData")
	{
		reg_file[rd_addr] = read_data;
	}

}

void CPU::get_results()
{
	int a0 = reg_file[10];
	int a1 = reg_file[11];  
	// Print the Results 
	cout << "(" << a0 << "," << a1 << ")" << endl;
	
	// Get num of R Type Instructions
	//cout << decode_block.num_r_type << endl;

	// Debug results
	//cout << decode_block.get_operation()<<endl;
	//cout << "(" << reg_file[0] << "," << reg_file[1] << "," << reg_file[2] << "," << reg_file[3]  << "," << reg_file[4] << "," << reg_file[5] << "," << reg_file[10] << "," << reg_file[11] << ")" << endl;
}