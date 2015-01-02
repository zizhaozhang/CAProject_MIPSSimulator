#include "OPMap.h"
#include "MIPSdis.h"
OprandMap::OprandMap(){
	//build map OPIndex_I I-format 
	OPIndex_I[0x00] = "R";
	OPIndex_I[0x2B] = "SW";
	OPIndex_I[0x23] = "LW";
	OPIndex_I[0x02] = "J";
	OPIndex_I[0x04] = "BEQ";
	OPIndex_I[0x05] = "BNE";
	OPIndex_I[0x01] = "BGEZ"; // same as BLTZ
	OPIndex_I[0x07] = "BGTZ";
	OPIndex_I[0x06] = "BLEZ",
		// OPIndex_I[0x000001] =	"BLTZ",
	OPIndex_I[0x08] = "ADDI";
	OPIndex_I[0x09] = "ADDIU";
	OPIndex_I[0x0A] = "SLTI";

	//R-format index is the last 6 bits
	OPIndex_R[0x0D] = "BREAK";
	OPIndex_R[0x2A] = "SLT";
	OPIndex_R[0x2B] = "SLTU",
	OPIndex_R[0x00] = "SLL"; //NOP
	OPIndex_R[0x02] = "SRL";
	OPIndex_R[0x03] = "SRA";	
	OPIndex_R[0x22] = "SUB";
	OPIndex_R[0x23] = "SUBU";
	OPIndex_R[0x20] = "ADD";
	OPIndex_R[0x21] = "ADDU";
	OPIndex_R[0x24] = "AND";
	OPIndex_R[0x25] = "OR";
	OPIndex_R[0x26] = "XOR";
	OPIndex_R[0x27] = "NOR";
	//handle NOP

	//Build map Name_Type


	Name_Type["SW"] = I_LoadStore; //sw rt, offset(base) s
	Name_Type["LW"] = I_LoadStore; //s

	Name_Type["J"] = I_Jump; //jump target

	Name_Type["BEQ"] = I_Branch;  //BEQ rs, rt, offset
	Name_Type["BNE"] = I_Branch;

	Name_Type["BGEZ"] = I_Branch_2;
	Name_Type["BLTZ"] = I_Branch_2; // test rt bits to dis BGEZ and BLTZ

	Name_Type["BGTZ"] = I_Branch_3; //BGTZ rs offset;
	Name_Type["BLEZ"] = I_Branch_3;

	Name_Type["ADDI"] = I_ALUInstant; //ADDI rt, rs, immediate
	Name_Type["ADDIU"] = I_ALUInstant;
	Name_Type["SLTI"]  = I_ALUInstant;

	//R-format
	Name_Type["BREAK"] = R_Break; //BREAK

	Name_Type["SLL"] = R_Shift; //SLL rd rt sa
	Name_Type["SRL"] = R_Shift;
	Name_Type["SRA"] = R_Shift;

	Name_Type["SLT"] = R_ALU; // SLT rd, rs ,rt
	Name_Type["SUB"] = R_ALU; //SUB rd, rs, rt
	Name_Type["SUBU"] = R_ALU;
	Name_Type["ADD"] = R_ALU;
	Name_Type["ADDU"] = R_ALU;
	Name_Type["AND"] = R_ALU;
	Name_Type["OR"] = R_ALU;
	Name_Type["XOR"] = R_ALU;
	Name_Type["NOR"] = R_ALU;
	Name_Type["SLTU"] = R_ALU;
	// save Register here
	for(int i = 0; i < 32; i++)
	{
		Register[i] = 'R' + MIPsDis::Int2String((Instr)i);
	}
}