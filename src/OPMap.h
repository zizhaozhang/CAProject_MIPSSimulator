//-----------------------------------------------------------------------------------------------------------------------
// Titile: CDA5155 Computer Architecture project I
// Author: Zizhao Zhang
// UFID: 11338226
// Email: zizhaozhang@ufl.edu

// All rights reserved
//-----------------------------------------------------------------------------------------------------------------------

#ifndef OPMAP_H
#define	OPMAP_H
#include "commonHeader.h"
#include <map>
/*static string OPName[27] = {"SW", "LW", // 2
	"J", "BEQ", "BNE", "BGEZ", "BGTZ", "BLEZ", "BLTZ", // 7
	"ADDI", "ADDIU", // 2
	"BREAK", // 1
	"SLT", "SLTI", "SLTU", // 3
	"SLL", "SRL", "SRA", // 3
	"SUB", "SUBU", "ADD", "ADDU", // 4
	"AND", "OR", "XOR", "NOR", // 4
	"NOP"}; // 
*/


class OprandMap{

public:
	 enum InsType{
		// I-format
		I_LoadStore = 1, // LW base rt offset: rt <- memory[base+offset]
		I_Jump, // J target 
		I_Branch, // BEQ(,BNZ) rs, rt, offset: if rs = rt then branch
		I_Branch_2, // BGEZ(BLTZ) 
		I_Branch_3, // BGTZ(BLEZ) rs, offset: if rs > 0, then branch
		I_ALUInstant, // 
		// R-format
		R_Break,
		R_Shift,
		R_ALU
	};
	std::map<string, InsType> Name_Type;
	std::map<Instr, string> OPIndex_I;
	std::map<Instr, string> OPIndex_R;

    string Register[32];
	
public:
	OprandMap();

};

#endif
