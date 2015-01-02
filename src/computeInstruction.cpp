#include "computeInstruction.h"
computeInstruction::computeInstruction()
{
	FunctionMapper["SW"] = &computeInstruction::SW;
	FunctionMapper["LW"] = &computeInstruction::LW;
	FunctionMapper["BEQ"] = &computeInstruction::BEQ;
	FunctionMapper["BNZ"] = &computeInstruction::BNZ;
	FunctionMapper["BGEZ"] = &computeInstruction::BGEZ;
	FunctionMapper["BLEZ"] = &computeInstruction::BLEZ;
	FunctionMapper["BLTZ"] = &computeInstruction::BLEZ;
	FunctionMapper["BGTZ"] = &computeInstruction::BGTZ;
	FunctionMapper["ADDI"] = &computeInstruction::ADDI;
	FunctionMapper["ADDIU"] = &computeInstruction::ADDIU;
	FunctionMapper["SLTI"] = &computeInstruction::SLTI;
	FunctionMapper["SLTU"] = &computeInstruction::SLTU;
	FunctionMapper["SLL"] = &computeInstruction::SLL;
	FunctionMapper["SLT"] = &computeInstruction::SLT;
	FunctionMapper["SRL"] = &computeInstruction::SRL;
	FunctionMapper["SRA"] = &computeInstruction::SRA;
	FunctionMapper["SUB"] = &computeInstruction::SUB;
	FunctionMapper["ADD"] = &computeInstruction::ADD;
	FunctionMapper["ADDU"] = &computeInstruction::ADDU;
	FunctionMapper["AND"] = &computeInstruction::AND;
	FunctionMapper["OR"] = &computeInstruction::OR;
	FunctionMapper["XOR"] = &computeInstruction::XOR;
	FunctionMapper["NOR"] = &computeInstruction::NOR;
	FunctionMapper["J"] = &computeInstruction::JUMP;
}
// rs->Vj rt->Vk
int computeInstruction::NOR(int Vj, int Vk, int A)
{
	return ~(Vj | Vk);
}
int computeInstruction::XOR(int Vj, int Vk, int A)
{
	return (Vj ^ Vk);
}
int computeInstruction::OR(int Vj, int Vk, int A)
{
	return (Vj | Vk);
}
int computeInstruction::AND(int Vj, int Vk, int A)
{
	return (Vk & Vk);
}
int computeInstruction::ADDU(int Vj, int Vk, int A)
{
	return (Vj + Vk);
}
int computeInstruction::ADD(int Vj, int Vk, int A)
{// how to handle overflow
	return (Vj + Vk);
}
int computeInstruction::SUB(int Vj, int Vk, int A)
{
	return (Vj - Vk);
}
int computeInstruction::SRA(int Vj, int Vk, int A)
{// duplicate the sign bit
	int si = (Vk < 0)? 1:0; // sign bits
	int b;
	if (si == 1){
		b = 2^A - 1;
		b = b << (sizeof(int) * 8 - A);
	}
		
	int d = Vk >> A;
	d += b; // set the high bits equal to sign bits
	return d;
}
int computeInstruction::SRL(int Vj, int Vk, int A)
{
	return (Vk >> A);	
}
int computeInstruction::SLT(int Vj, int Vk, int A)
{
	return (Vj < Vk);
}
int computeInstruction::SLL(int Vj, int Vk, int A)
{
	return (Vk << A);
}
int computeInstruction::SLTU(int Vj, int Vk, int A)
{
	return (Vj < Vk);
}
int computeInstruction::SLTI(int Vj, int Vk, int A)
{
	return (Vj < A);
}
int computeInstruction::ADDIU(int Vj, int Vk, int A)
{
	return (Vj + A);
}
int computeInstruction::ADDI(int Vj, int Vk, int A)
{// how to handle overflow
	return (Vj + A);
}

//for branch the object function compute the branch result;
int computeInstruction::BLTZ(int Vj, int Vk, int A)
{
	return 0;
}
int computeInstruction::BLEZ(int Vj, int Vk, int A)
{
	return 0;
}
int computeInstruction::BGEZ(int Vj, int Vk, int A)
{
	return 0;
}
int computeInstruction::BNZ(int Vj, int Vk, int A)
{

	return 0;
}
int computeInstruction::BEQ(int Vj, int Vk, int A)
{
	return (Vj == Vk);
}
int computeInstruction::BGTZ(int Vj, int Vk, int A)
{
	return 0 ;
}
int computeInstruction::SW(int Vj, int Vk, int A)
{
	return (Vj + A);
}
int computeInstruction::LW(int Vj, int Vk, int A)
{
	return (Vj + A);
}
int computeInstruction::JUMP(int Vj, int Vk, int A)
{
	return true;
}