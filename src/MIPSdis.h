//-----------------------------------------------------------------------------------------------------------------------
// Titile: CDA5155 Computer Architecture project I
// Author: Zizhao Zhang
// UFID: 11338226
// Email: zizhaozhang@ufl.edu

// All rights reserved
//-----------------------------------------------------------------------------------------------------------------------
#ifndef MISPDIS_H
#define MIPSDIS_H

#include "commonHeader.h"
#include"InsParser.h"
#include"OPMap.h"
#include "Instructions.h"
#include <stdlib.h>
#include <stdio.h>
typedef unsigned int Instr;
typedef char Byte;
#define Big_endian2Little_endian(x) ((x&0xff)<<24) + ((x&0xff00)<<8) + ((x&0xff0000)>>8) + ((x>>24)&0xff)
extern int StartAddr;


class MIPsDis{
public:
	MIPsDis();
	int CallMIPsDisassembler(vector<Instruction> &,  char *filenameInput, char* filenameOutput);
public:
	static string Int2String(Instr target);
	static string Int2String_signed(Instr target);
	static int unsigned2sign(Instr target);
	
private:
	string instructionParser(Instruction &, Instr in);
	string ConvertNbit(Instr target, int n);
	string Instr2Char(Instr target);
	void PrintOut(FILE * out, Instr machLan, string Assembly);
	int Disassembler(vector<Instruction> &, FILE *input, FILE *output);

};


#endif 
