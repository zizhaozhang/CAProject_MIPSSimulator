#ifndef BTB_H
#define BTB_H
#include "commonHeader.h"
//#define useless -1
#include "Instructions.h"
enum BTBstate{

	addnew_BTB = 1,
	update_BTB //predictor wrong
};
class BTB_Entry{
public:
	BTB_Entry();

public:
	int instr_addr;
	int branch_addr;
	bool predictor; // bool due to 1-bit predictor
	bool mis; //  miss predictior set mis = 1
	bool flag; // if in use
	bool notset;
	
};
class BTB{
public:
	BTB();
	// return the Predicted PC return predictor result if addr find in BTB, -1 if not, result_PC give the result PC
	int makePrediction(const int cur_PC, int &result_PC);
	// In Ex stage, if needUpdate Table is true, means the just issed instruction is a taken branch, then update the BTB table based or LRU 
	void updateBTB(int addr, BTBstate, int, bool&);
	void addnew_notset(string, int, int);
public:
	int BTB_CAMPACITY; // 16
	BTB_Entry BTB_table[16];

	int needUpdateTable; // if the branch is a taken branch (set to 1 in ID stage),
// BTB should be update at the EX stage.

	int Cur_Size; // Cur_Size the BTB_has been used; need to update if add more
};
#endif 

