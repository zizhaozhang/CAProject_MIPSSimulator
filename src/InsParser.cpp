#include "InsParser.h"
#include "MIPSdis.h"


int Parser::isBreak = 0;
void Parser::exParser(Instruction &instruction, string &OP, Instr in)
{

	Instr Oprand_op = (in >> Op_shift) & Bit6;
	OP_TYPE op_type;

	//handle NOP
	if(in == 0) 
	{
		// ifNOP++;
		// cout<<ifNOP;
		if(isBreak >= 1) {isBreak++; OP = '0';}
		else OP = "NOP";

		instruction.update_R(OP, OP, useless,useless,useless, useless);
	}
	else{

		if(Oprand_op == 0){ // R-format oprand
			op_type = R;	 
			segmentParser_R(instruction, OP, in);
		}
		else{//I-format oprand
			op_type = I;
			segmentParser_I(instruction, OP, in); 
		}
	}
}

void Parser::segmentParser_R(Instruction &instruction, string &OP, Instr seg)
{
	OprandMap Map;
	string oprand;
	Instr Oprand_op = (seg >> Op_shift) & Bit6; //get opcode
	Instr funt = seg & Bit6; // get funt
	oprand = Map.OPIndex_R[funt]; // get 
	if(oprand.empty())
	{
		cout<<"lack items!"<<endl;
		exit(1);
	}
	OP = OP + oprand; //Get opcode
	OprandMap::InsType OprandType;
	//Get register
	// R-format op rs rt rd shamt funct
	OprandType = Map.Name_Type[oprand];
	int dispNum_R[6] = {26, 21, 16, 11, 6, 0};

	// if(OprandType == NULL)
	// {
	// 	cout<<"lack items in Name_Type at "<<StartAddr<<endl;
	// 	//	cout<<OprandType<< " " <<oprand;
	// 	//update Instruction 

	// }

	if(OprandType == OprandMap::R_Break) // break 
	{
		isBreak ++; // when isBreak == 1, next 0 instruction will be 0 rather NOP
		//update Instruction 
		//void update(name, rd, rs, rt, sa);
		instruction.update_R(OP, oprand, useless, useless, useless, useless);
		return;
	}
	else if(OprandType == OprandMap::R_Shift) //SL rd, rt, sa
	{
		int rd_ind = (seg >> dispNum_R[3]) & Bit5;
		int rt_ind = (seg >> dispNum_R[2]) & Bit5;
		string rd = Map.Register[rd_ind];
		string rt = Map.Register[rt_ind];
		Instr sa = seg >> dispNum_R[4] & Bit5;
		OP = OP + " " + rd + ", " + rt + ", " + "#" + MIPsDis::Int2String(sa);

		//update Instruction
		//void update(name, rd, rs, rt, sa, base, target);
		instruction.update_R(OP, oprand, rd_ind, useless, rt_ind, sa);
		return;

	}
	else if(OprandType == OprandMap::R_ALU) //SLT rd, rs ,rt
	{
		int rd_ind = (seg >> dispNum_R[3]) & Bit5;
		int rt_ind = (seg >> dispNum_R[2]) & Bit5;
		int rs_ind = (seg >> dispNum_R[1]) & Bit5;

		string rd = Map.Register[(seg >> dispNum_R[3]) & Bit5];
		string rt = Map.Register[(seg >> dispNum_R[2]) & Bit5];
		string rs = Map.Register[(seg >> dispNum_R[1]) & Bit5];
		OP = OP + " " + rd + ", " + rs + ", " + rt;
		//void update(name, rd, rs, rt, sa, base, target);
		instruction.update_R(OP, oprand, rd_ind, rs_ind, rt_ind,useless);

		return ;
	}

}

void Parser::segmentParser_I(Instruction &instruction, string &OP, Instr seg)
{
	OprandMap Map;
	string oprand("");
	Instr Oprand_op = (seg >> Op_shift) & Bit6; //get opcode
	int dispNum_I[4] = {26, 21, 16, 0};
	oprand = Map.OPIndex_I[Oprand_op];

	if(oprand.empty())
	{
		cout<<"Lack OPIndex" <<endl;
		exit(1);
	}
	OP = OP + oprand;
	OprandMap::InsType OprandType;
	OprandType = Map.Name_Type[oprand];

	if(OprandType == NULL)
	{
		cout<<"lack Name_Type"<<endl;
		exit(1);
	}
	// begin generate Assambly language
	if(OprandType == OprandMap::I_LoadStore)//op base, rt, offset ==> SW rt, offset(base)
	{
		int base_ind = (seg >> dispNum_I[1]) & Bit5;
		int rt_ind = (seg >> dispNum_I[2]) & Bit5;

		string base = Map.Register[(seg >> dispNum_I[1]) & Bit5];
		string rt = Map.Register[(seg >> dispNum_I[2]) & Bit5];
		Instr offset = (seg >> dispNum_I[3]) & Bit16;
		OP = OP + " " + rt + ", " + MIPsDis::Int2String_signed(offset) + "(" + base + ")";

		//update Instruction 
		//void update_I(string name, RegisterID rs, RegisterID rt, RegisterID base, 
		//				int target, int offest, int immediate);
		instruction.update_I(OP, oprand, useless, rt_ind, base_ind,
			useless, MIPsDis::unsigned2sign(offset), useless);

		return ; 

	}
	else if(OprandType == OprandMap::I_Jump)//jump
	{
		int target = (seg & 0x3ffffff); // low 26bits
		OP = OP + " #" + MIPsDis::Int2String_signed(target<<2); //right shift 2 bits
		//update Instruction 
		//void update_I(string name, RegisterID rs, RegisterID rt, RegisterID base, 
		//				int target, int offest, int immediate);
		instruction.update_I(OP, oprand, useless, useless,													useless, MIPsDis::unsigned2sign(target<<2),useless, MIPsDis::unsigned2sign(target<<2));
		return;
	}
	else if(OprandType == OprandMap::I_Branch)//BEQ rs, rt, offset
	{
		int rs_ind = (seg >> dispNum_I[1]) & Bit5;
		int rt_ind = (seg >> dispNum_I[2]) & Bit5;

		string rs = Map.Register[(seg >> dispNum_I[1]) & Bit5];
		string rt = Map.Register[(seg >> dispNum_I[2]) & Bit5];
		Instr offset = (seg >> dispNum_I[3]) & Bit16;
		//update Instruction 
		//void update_I(string name, RegisterID rs, RegisterID rt, RegisterID base, 
		//				int target, int offest, int immediate);
		OP = OP + " " + rs + ", " + rt + ", " + "#" + MIPsDis::Int2String_signed(offset<<2);
		instruction.update_I(OP, oprand, rs_ind, rt_ind, useless,useless, MIPsDis::unsigned2sign(offset<<2),MIPsDis::unsigned2sign(offset<<2));
	
		return ;
	}
	else if (OprandType == OprandMap::I_Branch_3)
	{
		int rs_ind = (seg >> dispNum_I[1]) & Bit5;
		string rs = Map.Register[(seg >> dispNum_I[1]) & Bit5];
		Instr offset = (seg >> dispNum_I[3]) & Bit16;
		OP = OP + " " + rs + ", " + "#" + MIPsDis::Int2String_signed(offset<<2);
		//update Instruction 
		//void update_I(string name, RegisterID rs, RegisterID rt, RegisterID base, 
		//				int target, int offest, int immediate);
		instruction.update_I(OP, oprand, rs_ind, useless, useless,useless, MIPsDis::unsigned2sign(offset<<2), useless);
		return ; 
	}
	else if (OprandType == OprandMap::I_Branch_2)// handle BGEZ (rt=00001) and BLTZ (rt = 00000)
	{
		if(((seg >> dispNum_I[2]) & Bit5) == 0) // get BLTZ
		{
			oprand = "BLTZ";
		}
		else
		{
			oprand = "BGEZ";
		}
		int rs_ind = (seg >> dispNum_I[1]) & Bit5;
		string rs = Map.Register[(seg >> dispNum_I[1]) & Bit5];
		Instr offset = (seg >> dispNum_I[3]) & Bit16;
		OP = oprand;
		OP = OP + " " + rs + ", " + "#" + MIPsDis::Int2String_signed(offset);
		//update Instruction 
		//void update_I(string name, RegisterID rs, RegisterID rt, RegisterID base, 
		//				int target, int offest, int immediate);
		instruction.update_I(OP, oprand, rs_ind, useless, useless,useless, MIPsDis::unsigned2sign(offset), useless);

		OP = OP + " " + rs + ", " + "#" + MIPsDis::Int2String_signed(offset);
		return;
	}
	else if(OprandType == OprandMap::I_ALUInstant) // ADDI rt, rs, immediate
	{
		int rs_ind = (seg >> dispNum_I[1]) & Bit5;
		int rt_ind = (seg >> dispNum_I[2]) & Bit5;

		string rs = Map.Register[(seg >> dispNum_I[1]) & Bit5];
		string rt = Map.Register[(seg >> dispNum_I[2]) & Bit5];
		Instr immediate = (seg >> dispNum_I[3]) & Bit16;
		OP = OP + " " + rt + ", " + rs + ", " + '#' + MIPsDis::Int2String_signed(immediate);
		//update Instruction 
		//void update_I(string name, RegisterID rs, RegisterID rt, RegisterID base, 
		//				int target, int offest, int immediate);
		
		instruction.update_I(OP, oprand, rs_ind, rt_ind, useless,useless,											useless, MIPsDis::unsigned2sign(immediate));

		return;
	}
}