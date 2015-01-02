#ifndef COMPUTEINSTERUCTION_H
#define COMPUTEINSTERUCTION_H
#include "commonHeader.h"
#include "Instructions.h"
#include <map>
class computeInstruction
{
public:
	computeInstruction();

public:
	typedef int(computeInstruction::*func) (int, int, int);
	map<string, func> FunctionMapper;
	
	int SW(int, int, int);
	int LW(int, int , int);
	int BEQ(int, int , int);
	int BNZ(int, int , int);
	int BGEZ(int, int , int);
	int BLEZ(int, int , int);
	int BLTZ(int, int , int);
	int BGTZ(int, int, int);
	int ADDI(int, int , int);
	int	ADDIU(int, int , int);
	int SLTI(int, int , int);
	int SLTU(int, int , int);
	int SLL(int, int , int);
	int SLT(int, int , int);
	int SRL(int, int , int);
	int SRA(int, int , int);
	int SUB(int, int , int);
	int SUBU(int, int , int);
	int ADD(int, int, int);
	int ADDU(int, int, int);
	int AND(int, int, int);
	int OR (int, int, int);
	int XOR(int, int, int);
	int NOR (int, int, int);
	int JUMP(int, int, int);
	
};
#endif