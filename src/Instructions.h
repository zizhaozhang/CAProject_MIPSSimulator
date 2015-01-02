#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include "commonHeader.h"
#define useless -1
enum InsType {
	Load,
	Store,
	Jump,
	Branch,
	ALUInstant,
	Shift,
	Break,
	ALU,
	NOP

};
class Instruction{

public:
	Instruction();
	void update_R(string instr, string name, RegisterID rd, RegisterID rs, RegisterID rt, int sa);
	void update_I(string instr, string name, RegisterID rs, RegisterID rt, RegisterID base, 
					int target, int offest, int immediate);
public:
	// static int useless;
	RegisterID rd, rs, rt;
	int sa;
	RegisterID base;
	int target;
	string name;  // the name of instruction
	int immediate;
	int offset;

	string instr; // The whole instruction format
public:
	bool flag; //test if have instruction.

	int predictor;

public:
	std::map<string, InsType> InstrMap;
	void ParserInstruction(int &rs, int &rt, int &rd, int &immediate);
	void getBranch(int &rs, int &rt, int &rd, int &immediate);
	void getShift(int &rs, int &rt, int &rd, int &immediate);
	InsType getType();
	bool isALU();
	bool isLoad();
	bool isJump();
	static bool isTakenBrach(string, int &, const int, const int, int, const int);
	bool isBranch();
	bool isBranch(string);
	bool isMemoryOp(string );
	bool ClearSpecialInstrRS(string);

public:
	static void computeInstruction(int );
};
#endif 