//-----------------------------------------------------------------------------------------------------------------------
// Titile: CDA5155 Computer Architecture project I
// Author: Zizhao Zhang
// UFID: 11338226
// Email: zizhaozhang@ufl.edu

// All rights reserved
//-----------------------------------------------------------------------------------------------------------------------
#ifndef INSPARSER_H
#define INSPARSER_H
#include "commonHeader.h"
#include"OPMap.h"
#include "Instructions.h"
#define Bit6 0x3f
#define Bit5 0x1f
#define Bit3 0x7
#define Bit16 0xffff
#define Op_shift 26
typedef unsigned int Instr;
extern int StartAddr;
//define split rules

typedef enum {R,I} OP_TYPE;
//extern string Int2String_signed(Instr target);
class Parser{
	public:
	static int isBreak;
    public:

		static void exParser(Instruction &, string &OP, Instr in);
		
	private:
		static void segmentParser_R(Instruction &, string &OP, Instr seg);
		
		static void segmentParser_I(Instruction &, string &OP, Instr seg);
	
};

#endif
