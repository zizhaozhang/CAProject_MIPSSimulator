#include "Instructions.h"


// int Instruction::useless = -1;


Instruction::Instruction()
	: rd(useless), rs(useless), rt(useless), sa(useless), base(useless),
	target(useless), name(""), instr(""), immediate(useless), offset(useless)
{
	flag = 0;
	predictor = -2; // to distinguish from the validate value like -1 0 1

	InstrMap["SW"] = Store; //sw rt, offset(base) s
	InstrMap["LW"] = Load; //LW rt, offset(base)
	InstrMap["J"] = Jump; //jump target

	InstrMap["BEQ"] = Branch;  //BEQ rs, rt, offset
	InstrMap["BNE"] = Branch;  // rs, rt, offset

	InstrMap["BGEZ"] = Branch; //BGEZ rs, offset
	InstrMap["BLTZ"] = Branch; //rs, offset test rt bits to dis BGEZ and BLTZ

	InstrMap["BGTZ"] = Branch; //BGTZ rs offset;
	InstrMap["BLEZ"] = Branch;

	InstrMap["ADDI"] = ALUInstant; //ADDI rt, rs, immediate
	InstrMap["ADDIU"] = ALUInstant;

	InstrMap["SLTI"]  = Shift; //SLTI rt, rs, immediate
	InstrMap["SLL"] = Shift; //SLL rd rt sa
	InstrMap["SRL"] = Shift;
	InstrMap["SRA"] = Shift;
	//R-format
	InstrMap["BREAK"] = Break; //BREAK
	InstrMap["NOP"] = NOP;
	
	InstrMap["SLT"] = ALU; // SLT rd, rs ,rt
	InstrMap["SUB"] = ALU; //SUB rd, rs, rt
	InstrMap["SUBU"] = ALU;
	InstrMap["ADD"] = ALU;
	InstrMap["ADDU"] = ALU;
	InstrMap["AND"] = ALU;
	InstrMap["OR"] = ALU;
	InstrMap["XOR"] = ALU;
	InstrMap["NOR"] = ALU;
	InstrMap["SLTU"] = ALU;

}
void Instruction::update_R(string instr, string name, RegisterID rd, RegisterID rs, RegisterID rt, int sa)
{
	assert(name.length()>0);
	this->instr = instr;
	this->name = name;
	this->rd = rd;
	this->rs = rs;
	this->rt = rt;
	this->sa = sa;
	flag = 1;

}
void Instruction::update_I(string instr, string name, RegisterID rs, RegisterID rt, RegisterID base, 
	int target, int offest, int immediate)
{
	assert(name.length()>0);
	this->instr = instr;
	this->name = name;
	this->rs = rs;
	this->rt = rt;
	this->base = base;
	this->target = target;
	this->immediate = immediate;
	this->offset = offest;

	flag = 1;
}
void Instruction::getBranch(int &rs, int &rt, int &rd, int &immediate)
{
	string name = this->name;
	if(name == "BEQ" || name == "BNE")
	{
		rs = this->rs;
		rt = this->rt;
		immediate = this->offset;
	}
	else if(name == "BGEZ" || name == "BGTZ" || name == "BLEZ")
	{
		rs = this->offset;
		immediate = this->offset;
	}
	//else if(name == "J")
	//{

	//}
	
}
void Instruction::getShift(int &rs, int &rt, int &rd, int &immediate)
{
	string name = this->name;
	if(name == "SLT")
	{
		rd = this->rd;
		rs = this->rs;
		rt = this->rd;
	}
	else if(name == "SLL" || name == "SRL" || name == "SRA")
	{
		rd = this->rd;
		rt = this->rt;
		immediate = this->sa; // treat sa as immediate
	}
}
void Instruction::ParserInstruction(int &rs, int &rt, int &rd, int &immediate)
{
	InsType type = InstrMap[this->name];
	switch(type)
	{
	case Load: // rt, offset(base) regard base as rs
		rt = this->rt;
		rs = this->base;
		immediate = this->offset;
		break;
	case Store: //rt, offset(base) regard base as rs
		rt = this->rt;
		rs = this->base;
		immediate = this->offset;
		break;
	case Branch:
		getBranch(rs, rt, rd, immediate);
		break;
	case Jump:
		immediate = this->target;
		break;
	case ALUInstant:
		rt = this->rt;
		rs = this->rs;
		immediate = this->immediate;
		break;
	case Shift:
		getShift(rs, rt, rd, immediate);
		break;
	case Break:
		break;
	case ALU:
		rd = this->rd;
		rt = this->rt;
		rs = this->rs;
		break;

	}

	assert(rs < registerTotalNum);
	assert(rt < registerTotalNum);
	assert(rd < registerTotalNum);
}

InsType Instruction::getType()
{
	return InstrMap[this->name];
}

bool Instruction::isALU()
{
	return 0;

 
}
bool Instruction::isLoad()
{	
	if (InstrMap[this->name] == Load)
		return true;
	else
		return false;		
}
bool Instruction::isJump()
{
	if(InstrMap[this->name] == Jump)
		return true;
	else
		return false;
}
bool Instruction::isBranch()
{
	if(name == "BEQ" || name == "BNE" || name == "BGEZ" 
		|| name == "BGTZ" || name == "BLEZ" || name == "BLTZ" || name == "J")
		return true;
	else
		return false;

}
bool Instruction::isTakenBrach(string Op, int &nextPC, int Vj, int Vk, int immediate, const int PC)
{
	if (Op == "BEQ")
	{if (Vj == Vk) {nextPC = immediate + PC; return true;}}
	else if(Op == "BNE")
	{if(Vj != Vk){ nextPC = immediate + PC; return true;}}
	else if (Op == "BGEZ")
	{if(Vj >= 0){ nextPC = immediate+ PC; return true;}}
	else if (Op == "BGTZ")
	{if(Vj > 0) {nextPC = immediate+ PC; return true;}}
	else if (Op == "BLEZ")
	{if (Vj <= 0) {nextPC = immediate+ PC; return true;}}
	else if (Op == "J")
	{nextPC = immediate; return true;}

	return false;
	// keep the nextPC the same

}

void Instruction::computeInstruction(int name)
{
	
	
}

bool Instruction::isBranch(string name)
{
	InsType  type = InstrMap[name];
	if (type == Branch || type == Jump) return true;
	else return false;

}
bool Instruction::isMemoryOp(string name)
{
	InsType type = InstrMap[name];
	if(type == Load || type == Store)
		return true;
	else
		return false;

}
bool Instruction::ClearSpecialInstrRS(string name)
{
	return isBranch(name) || name == "SW";
}